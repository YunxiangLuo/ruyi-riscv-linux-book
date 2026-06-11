# Class 2: GPIO 输出控制 LED

## 本章目标

- 理解 Linux 用户态 GPIO 输出的基本路径。
- 识别 `gpiochip`、GPIO line、输出电平和 LED 有效电平之间的关系。
- 使用 `gpiodetect`、`gpioinfo`、`gpioset` 完成 LED 手动亮灭验证。
- 构建并运行 Blink 程序，使 LED 按指定周期亮灭。
- 记录目标板 GPIO 参数、命令版本、运行现象和待实测缺口。

GPIO 输出是开发板课程中第一个可见的板级控制实验。程序写入一个电平值，目标板引脚产生电平变化，LED 电路把这个变化转换成亮、灭或闪烁现象。这个实验建立“程序 -> Linux 设备接口 -> GPIO line -> 外部电路 -> 可见现象”的基本对应关系。

## 前置条件

- 已能登录目标 Linux 系统，登录方式可以是 SSH、串口或本地终端。
- 已掌握基本工程操作，包括进入目录、执行 `make build`、查看命令输出和记录错误信息。
- 目标系统应能访问 `/dev/gpiochip*`，或者能明确记录当前镜像没有暴露 GPIO character device。
- 目标板应提供板载 LED 或已接好外接 LED。外接 LED 必须包含限流电阻，并确认电压等级、接线方向和共地关系。

本节最终验证依赖真实 Linux 开发板。QEMU 可以用于前置 Linux 命令和工程构建训练，但不能替代真实 GPIO、LED、电平和接线验证。

## 知识简介

GPIO 是 General Purpose Input/Output 的缩写，表示通用输入输出引脚。一个 GPIO 引脚可以被配置为输入或输出。本节只使用输出方向：程序把某条 GPIO line 设置为输出，并写入 `0` 或 `1`。如果这条 line 连接到 LED 电路，电平变化就会表现为 LED 亮灭。

本节包含三个核心知识点：

1. Linux 用户态 GPIO 输出路径。
2. `gpiochip`、line 和 LED 有效电平。
3. Blink 程序的周期输出机制。

### 知识点 1：Linux 用户态 GPIO 输出路径

在 Linux 系统中，应用程序通常不直接读写 SoC GPIO 寄存器。GPIO 控制器由内核管理，用户态程序通过内核提供的设备接口申请 GPIO line，再设置方向和输出值。

本章使用 GPIO character device 路径：

```text
用户态命令或 C 程序
  -> /dev/gpiochipN
  -> 内核 GPIO 子系统
  -> GPIO controller
  -> GPIO line
  -> LED 电路
  -> 亮、灭、闪烁
```

`/dev/gpiochipN` 表示一个 GPIO 控制器设备文件。`gpiochip` 下面包含若干条 line，每条 line 对应控制器中的一个可控制信号。程序控制 LED 时，实际写入的是某个 `gpiochip` 中的某条 line，而不是直接写入“LED”这个对象。

这条路径决定了排错顺序：

1. `/dev/gpiochip*` 是否存在。
2. `gpioinfo` 是否能列出目标 line。
3. 目标 line 是否空闲，是否被其他驱动或进程占用。
4. 写入的有效电平是否与 LED 电路一致。
5. LED 电路本身是否接线正确。

先验证设备接口，再验证 line 和电平，最后验证 C 程序，可以把板级问题、权限问题和代码问题分开。

### 知识点 2：`gpiochip`、line 和 LED 有效电平

不同开发板的 LED 连接方式不同。某块板子的 LED 可能位于 `gpiochip0` 的某条 line，也可能位于另一个 GPIO 控制器。即使 line 编号正确，LED 也可能是高电平点亮或低电平点亮。

本节用以下参数描述 LED：

| 参数 | 含义 | 示例格式 |
| --- | --- | --- |
| `LED_CHIP` | LED 所在 GPIO 控制器 | `gpiochip0` |
| `LED_LINE` | LED 对应 line 编号 | `12` |
| `LED_ACTIVE_LEVEL` | LED 点亮时写入的电平 | `0` 或 `1` |
| `LED_INACTIVE_LEVEL` | LED 熄灭时写入的电平 | `0` 或 `1` |
| `GPIO_ACCESS` | 访问 GPIO 是否需要 `sudo` 或权限配置 | `sudo` / 普通用户 |
| `LIBGPIOD_PROFILE` | 当前镜像中的 `gpioset` 命令口径 | `gpioset -c chip line=value` 等 |

高电平有效和低电平有效是两种常见情况：

```text
高电平有效：
  写入 1 -> LED 亮
  写入 0 -> LED 灭

低电平有效：
  写入 0 -> LED 亮
  写入 1 -> LED 灭
```

判断有效电平的可靠方法是分别写入 `0` 和 `1`，观察 LED 现象，并记录哪一个值对应点亮。只记录 line 编号是不完整的，因为 Blink 程序还需要知道点亮值和熄灭值。

### 知识点 3：Blink 程序的周期输出机制

手动写入一次电平只能验证 LED 是否可控。Blink 程序把这个动作放入循环中，按时间参数重复写入点亮值和熄灭值，从而形成闪烁现象。

`code/class2` 中的 Blink 程序使用 Linux GPIO character device ioctl 访问 `/dev/gpiochip*`。程序不依赖 `libgpiod-dev` 的 C API，但实验仍使用 `gpiodetect`、`gpioinfo` 和 `gpioset` 先确认板级参数。

代码工程位置：

```text
chapters/ch03_gpio_interaction/code/class2
```

程序运行参数：

```text
chip line active_level inactive_level [on_ms] [off_ms] [cycles]
```

核心执行过程：

```text
解析命令行参数
  -> 打开 /dev/gpiochipN
  -> 申请目标 line 为输出
  -> 写入 active_level
  -> 等待 on_ms
  -> 写入 inactive_level
  -> 等待 off_ms
  -> 重复 cycles 次或持续运行
  -> 退出前写回 inactive_level
```

Blink 运行正确时，LED 现象应与时间参数一致。例如 `500 500` 表示亮约 500 ms、灭约 500 ms；`100 900` 表示亮的时间短、灭的时间长。

## 环境准备

| 项目 | 要求 | 检查方式 |
| --- | --- | --- |
| 目标板 | K1 / Muse Pi Pro；K1 暂不可用时可用 LicheePi 4A 临时验证 | 查看 `boards/` 中对应记录 |
| 系统 | 可进入 Linux shell | SSH、串口或本地终端登录成功 |
| GPIO 接口 | 暴露 `/dev/gpiochip*` | `ls /dev/gpiochip*` |
| 工具 | 有 `libgpiod` 命令行工具 | `gpiodetect`、`gpioinfo`、`gpioset` |
| 编译环境 | Linux 目标环境或 RISC-V Linux 交叉编译环境 | `uname -a`、`make --version`、`cc --version` |
| LED | 板载 LED 或外接 LED | 记录连接位置、电平和安全电路 |

目标系统记录：

```bash
hostname
uname -a
cat /etc/os-release
whoami
pwd
```

记录项：

```text
板卡：
系统镜像：
登录方式：
当前用户：
是否需要 sudo：
```

这些信息用于区分板卡差异、镜像差异、工具版本差异和权限差异。

## 操作步骤

### 步骤 1：检查 GPIO 设备和工具

```bash
ls /dev/gpiochip*
which gpiodetect
which gpioinfo
which gpioset
gpiodetect
gpioinfo
gpioset --version
gpioset --help
```

检查结果应能回答三个问题：

- 系统是否暴露 GPIO character device。
- 当前镜像是否安装 `libgpiod` 命令行工具。
- 当前 `gpioset` 使用哪一种参数格式。

如果 `gpioset --help` 支持 `-c/--chip`，后续命令使用 `gpioset -c "$LED_CHIP" "$LED_LINE=$VALUE"`。如果当前版本不支持该写法，按 `--help` 输出记录实际命令格式。

### 步骤 2：填写 LED 参数

根据板级资料、`gpioinfo` 输出和实际现象填写：

```bash
LED_CHIP=gpiochipX
LED_LINE=Y
LED_ACTIVE_LEVEL=1
LED_INACTIVE_LEVEL=0
```

`gpiochipX` 和 `Y` 只是占位格式，不是真实参数。真实参数必须来自目标板资料或实测记录。

记录表：

| 项目 | 记录 |
| --- | --- |
| LED 类型 |  |
| 连接位置 |  |
| `LED_CHIP` |  |
| `LED_LINE` |  |
| `LED_ACTIVE_LEVEL` |  |
| `LED_INACTIVE_LEVEL` |  |
| `GPIO_ACCESS` |  |
| `LIBGPIOD_PROFILE` |  |

### 步骤 3：手动控制 LED 亮灭

支持 `-c/--chip` 的 `gpioset`：

```bash
gpioset -c "$LED_CHIP" "$LED_LINE=$LED_ACTIVE_LEVEL"
sleep 1
gpioset -c "$LED_CHIP" "$LED_LINE=$LED_INACTIVE_LEVEL"
```

不支持 `-c/--chip` 的 `gpioset`：

```bash
gpioset "$LED_CHIP" "$LED_LINE=$LED_ACTIVE_LEVEL"
sleep 1
gpioset "$LED_CHIP" "$LED_LINE=$LED_INACTIVE_LEVEL"
```

现象记录：

```text
写入 LED_ACTIVE_LEVEL 后：
写入 LED_INACTIVE_LEVEL 后：
```

如果现象与预期相反，交换 `LED_ACTIVE_LEVEL` 和 `LED_INACTIVE_LEVEL` 后重新验证。

### 步骤 4：构建 Blink 程序

```bash
cd chapters/ch03_gpio_interaction/code/class2
make clean
make build
```

构建成功后应生成：

```text
build/blink
```

如果出现 `linux/gpio.h` 相关错误，说明当前构建环境不是合适的 Linux 目标环境，或缺少目标 sysroot / kernel headers。该问题不能通过修改 LED 参数解决。

### 步骤 5：运行 Blink

使用默认周期持续闪烁：

```bash
./build/blink "$LED_CHIP" "$LED_LINE" "$LED_ACTIVE_LEVEL" "$LED_INACTIVE_LEVEL"
```

运行有限次数：

```bash
./build/blink "$LED_CHIP" "$LED_LINE" "$LED_ACTIVE_LEVEL" "$LED_INACTIVE_LEVEL" 500 500 10
```

观察并记录：

```text
运行命令：
终端输出：
LED 现象：
停止方式：
停止后 LED 状态：
```

### 步骤 6：修改周期并复测

```bash
./build/blink "$LED_CHIP" "$LED_LINE" "$LED_ACTIVE_LEVEL" "$LED_INACTIVE_LEVEL" 100 900 10
```

记录亮灭节奏是否变为“短亮、长灭”。如果现象没有变化，检查参数是否传入、程序是否重新运行、目标 line 是否被其他进程占用。

## 运行验证

| 验证项 | 预期现象 | 记录 |
| --- | --- | --- |
| 目标系统记录 | 板卡、镜像、登录方式、当前用户记录完整 |  |
| GPIO 设备识别 | 能列出 `/dev/gpiochip*` |  |
| `libgpiod` 工具检查 | 能执行 `gpiodetect`、`gpioinfo`、`gpioset --help`，或明确记录缺失工具 |  |
| LED 参数确认 | 能写出 `LED_CHIP`、`LED_LINE`、`LED_ACTIVE_LEVEL`、`LED_INACTIVE_LEVEL` |  |
| LED 手动控制 | 写入点亮值和熄灭值时 LED 状态不同 |  |
| Blink 构建 | `build/blink` 生成 |  |
| Blink 运行 | LED 至少完成 5 次稳定亮灭周期 |  |
| 周期修改 | 修改 `on_ms` 和 `off_ms` 后，亮灭节奏同步变化 |  |
| 安全停止 | 停止后 LED 回到无效电平，或有明确恢复命令和记录 |  |

练习：

1. 将 Blink 参数从 `500 500 10` 改为 `100 900 10`，记录现象变化。
2. 交换 `LED_ACTIVE_LEVEL` 和 `LED_INACTIVE_LEVEL`，观察现象是否相反，再恢复正确参数。
3. 比较 `gpioset` 手动控制和 `blink` 程序控制：两者使用的是同一组 GPIO 参数，但一个是单次写值，一个是周期写值。

## 常见问题

| 现象 | 可能原因 | 处理方式 |
| --- | --- | --- |
| `gpiodetect` 或 `gpioset` 不存在 | 镜像未安装 `libgpiod` 命令行工具 | 记录镜像和错误输出，补安装方式或使用已验证镜像 |
| `/dev/gpiochip*` 不存在 | 内核配置、设备树或镜像未暴露 GPIO character device | 记录为板级问题，不填写猜测参数 |
| 找不到 LED line | 板级资料不足，或 LED 被其他驱动管理 | 查阅板级记录，用 `gpioinfo` 辅助定位 |
| LED 不亮 | line 错误、有效电平错误、外接 LED 极性错误、限流电阻缺失、权限不足或 pinmux 不匹配 | 分别写 `0` 和 `1`，复查接线、权限和板级资料 |
| LED 现象相反 | 有效电平填反 | 交换 `LED_ACTIVE_LEVEL` 和 `LED_INACTIVE_LEVEL` 后重新验证 |
| `Permission denied` | 当前用户没有访问 `/dev/gpiochip*` 权限 | 临时使用 `sudo`，长期方案写入板级准备文档 |
| `linux/gpio.h: No such file or directory` | 编译环境不是 Linux 目标环境，或缺少内核头文件 | 在目标板上编译，或安装目标 sysroot/kernel headers |
| 程序运行后 LED 不受 `gpioset` 控制 | line 被正在运行的程序占用 | 停止 Blink 程序后再执行手动命令 |

## 本章成果

- 目标系统记录：板卡、镜像、登录方式、当前用户和权限情况。
- LED GPIO 资源记录：chip、line、点亮电平、熄灭电平和 `gpioset` 命令口径。
- LED 手动亮灭记录：命令、终端输出和可见现象。
- Blink 构建记录：构建命令、生成文件和错误处理。
- Blink 运行记录：运行命令、亮灭周期、停止方式和停止后状态。
- 板级待补项：K1 / Muse Pi Pro 最终 LED line、有效电平、权限和 `libgpiod` 命令版本。
