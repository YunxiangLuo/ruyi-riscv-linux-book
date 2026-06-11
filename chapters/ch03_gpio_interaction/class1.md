# Class 1: GPIO 输出控制 LED

## 本章目标

完成本课后，学生应能够：

- 说明 Linux 用户态程序控制 GPIO 输出的大致路径。
- 使用 `libgpiod` 命令确认 LED 所在 `gpiochip`、line 和有效电平。
- 构建并运行 Blink 程序，让 LED 按固定周期亮灭。
- 记录 GPIO 参数、运行命令、可见现象和仍需真实板卡验证的缺口。

本课聚焦 GPIO 输出，把程序写值、电平变化和 LED 可见现象对应起来，先建立最小的板级输出控制能力。

## 前置条件

开始本课前，应具备以下基础：

- 已完成 ch01，能够通过 SSH、串口或本地终端进入目标 Linux shell。
- 已完成 ch02，能够理解工程目录、`Makefile`、`make build` 和基本命令记录。
- 已确认目标系统能访问 `/dev/gpiochip*`，或能把当前缺口记录为板级问题。
- 已准备板载 LED 或外接 LED。外接 LED 必须有限流电阻，并确认电压等级、接线方向和共地关系。

本课的最终验证需要真实 Linux 目标环境。普通 macOS 主机可以用来阅读文档、查看代码和整理记录，但不能直接完成 GPIO character device 程序的最终构建验证。

## 知识简介

GPIO 是 General Purpose Input/Output 的缩写，可以理解为“可由软件控制或读取的通用引脚”。在本课中，我们只使用输出方向：程序向某个 GPIO line 写入 `0` 或 `1`，外部电路把这个电平变化表现为 LED 亮灭。

本课围绕三个知识点展开：

1. Linux 用户态 GPIO 输出路径。
2. LED 参数表和有效电平。
3. Blink 程序的构建、运行和现象验证。

### 知识点 1：Linux 用户态 GPIO 输出路径

在裸机程序中，控制 GPIO 往往意味着直接读写寄存器。在 Linux 系统中，应用程序不应直接操作 SoC 寄存器，而是通过内核提供的设备接口访问 GPIO。课程默认采用 GPIO character device 方式，也就是通过 `/dev/gpiochip*` 访问 GPIO 控制器，再指定控制器里的某一条 line。

可以把路径理解为：

```text
用户命令或 C 程序
  -> /dev/gpiochipN
  -> GPIO controller
  -> GPIO line
  -> 板载或外接 LED
  -> 亮、灭、闪烁
```

图 3-1 建议在 PPT 中画成一张数据流图：左侧是 `gpioset` 或 `blink` 程序，中间是 `/dev/gpiochip*` 和 line，右侧是 LED 可见现象。学生需要建立的关键认识是：程序不是“直接控制 LED”，而是控制某个 GPIO line，LED 只是这个 line 外接电路的可见结果。

课堂上先使用 `gpiodetect`、`gpioinfo` 和 `gpioset` 做手动验证，再运行代码工程。这有两个好处：第一，能先确认系统是否暴露 GPIO 接口；第二，能把板级参数和程序问题分开。如果手动 `gpioset` 都无法改变 LED 状态，就不应急着调试 C 程序。

### 知识点 2：LED 参数表和有效电平

LED 实验最容易出错的地方不是代码，而是参数。课程文档不能把某个未验证的 GPIO 编号写成确定答案。每块板子的 LED 可能连接在不同的 GPIO controller 和 line 上，LED 也可能是高电平点亮或低电平点亮。

本课使用以下参数表记录板级事实：

| 参数 | 含义 | 当前状态 |
| --- | --- | --- |
| `LED_CHIP` | LED 所在的 `gpiochip`，例如 `gpiochip0` | 待补 |
| `LED_LINE` | LED 对应 line 编号，例如 `12` | 待补 |
| `LED_ACTIVE_LEVEL` | LED 点亮时写入的电平，通常是 `0` 或 `1` | 待补 |
| `LED_INACTIVE_LEVEL` | LED 熄灭时写入的电平，通常与有效电平相反 | 待补 |
| `GPIO_ACCESS` | 是否需要 `sudo`、用户组或 udev 规则 | 待补 |
| `LIBGPIOD_PROFILE` | 当前镜像采用的 `gpioset` 命令口径 | 待补 |

有效电平是本课必须讲清楚的概念。高电平点亮的 LED，`LED_ACTIVE_LEVEL=1`，`LED_INACTIVE_LEVEL=0`。低电平点亮的 LED，`LED_ACTIVE_LEVEL=0`，`LED_INACTIVE_LEVEL=1`。如果学生写入 `1` 时 LED 灭、写入 `0` 时 LED 亮，这通常不是程序“反了”，而是 LED 电路采用低电平有效。

图 3-2 建议画成两种 LED 连接方式对比图：一种是高电平有效，一种是低电平有效。图中只需展示程序写入值和 LED 现象的对应关系，不需要展开复杂电路分析。

### 知识点 3：Blink 程序从一次写值到周期输出

手动 `gpioset` 只能证明“写入某个电平会改变 LED 状态”。Blink 程序进一步证明“程序可以持续占用 line，并按时间参数反复切换输出值”。这一步把一次性命令升级为可维护的代码工程。

本课代码工程位于：

```text
chapters/ch03_gpio_interaction/code/class1
```

核心程序是 `blink`。它接收 LED 的 chip、line、有效电平和无效电平作为参数，再按固定周期切换输出。运行时学生应观察三件事：

- 构建是否生成 `build/blink`。
- LED 是否按周期亮灭。
- 停止程序后 LED 是否回到无效电平，或是否需要手动恢复。

代码阅读时不要求学生掌握所有 `ioctl` 细节，但应能理解三层结构：

| 层次 | 文件或函数 | 作用 |
| --- | --- | --- |
| 参数层 | `src/blink.c` 的参数解析 | 把命令行里的 chip、line 和电平转换为程序变量 |
| GPIO 封装层 | `src/gpio_chardev.c` | 打开 `/dev/gpiochip*`，申请 line，写入电平 |
| 实验行为层 | blink 循环 | 按 `on_ms` 和 `off_ms` 周期切换 LED 状态 |

代码页建议只展示参数解析、写入 active level、写入 inactive level 三个片段。完整代码以仓库源码为准，课堂不需要把所有错误处理逐行讲完。

## 环境准备

| 项目 | 要求 | 检查方式 |
| --- | --- | --- |
| 目标板 | K1 / Muse Pi Pro；K1 暂不可用时可用 LicheePi 4A 临时验证 | 查看 `boards/` 中对应记录 |
| 系统 | 可进入 Linux shell | SSH、串口或本地终端登录成功 |
| GPIO 接口 | 优先确认 `/dev/gpiochip*` | `ls /dev/gpiochip*` |
| 工具 | 优先使用 `libgpiod` 命令行工具 | `gpiodetect`、`gpioinfo`、`gpioset` |
| 编译环境 | Linux 目标环境或可用 RISC-V Linux sysroot | `uname -a`、`make --version`、`cc --version` |
| LED | 板载 LED 或外接 LED | 能确认连接位置和安全电路 |

进入实验前，建议先记录目标系统信息：

```bash
hostname
uname -a
cat /etc/os-release
whoami
pwd
```

这些记录看似简单，但在板卡、镜像、工具版本不断变化时非常有用。后续如果 LED 参数或命令口径不同，教师和助教可以根据这些信息判断差异来自板卡、镜像还是权限。

## 操作步骤

### 步骤 1：确认 GPIO 输出访问方式

先确认系统中是否有 GPIO character device：

```bash
ls /dev/gpiochip*
```

再检查 `libgpiod` 工具：

```bash
which gpiodetect
which gpioinfo
which gpioset
gpiodetect
gpioinfo
gpioset --version
gpioset --help
```

如果 `gpioset` 不存在，记录镜像名称和错误输出，不要直接假设 GPIO 编号。若 `gpioset` 存在，也要看 `--help`，因为不同版本命令参数可能不同。

### 步骤 2：确认 LED 输出资源

确认 LED 是板载 LED 还是外接 LED。外接 LED 必须确认限流电阻、电压等级、接线方向和共地关系。

记录格式：

```text
板卡：
系统镜像：
登录方式：
LED 类型：
连接位置：
LED_CHIP：
LED_LINE：
LED_ACTIVE_LEVEL：
LED_INACTIVE_LEVEL：
是否需要 sudo：
LIBGPIOD_PROFILE：
```

这里的参数必须来自真实板卡验证、板级资料和课堂观察。没有验证时应写“待补”或“待实测”，不要写猜测值。

### 步骤 3：手动控制 LED 亮灭

若当前 `gpioset --help` 支持 `-c/--chip`：

```bash
gpioset -c "$LED_CHIP" "$LED_LINE=$LED_ACTIVE_LEVEL"
sleep 1
gpioset -c "$LED_CHIP" "$LED_LINE=$LED_INACTIVE_LEVEL"
```

若当前版本不支持 `-c/--chip`，按该版本帮助信息改写，例如：

```bash
gpioset "$LED_CHIP" "$LED_LINE=$LED_ACTIVE_LEVEL"
sleep 1
gpioset "$LED_CHIP" "$LED_LINE=$LED_INACTIVE_LEVEL"
```

记录写入有效电平和无效电平后的 LED 现象。如果现象相反，优先检查 `LED_ACTIVE_LEVEL` 和 `LED_INACTIVE_LEVEL` 是否填反。

### 步骤 4：构建 Blink 程序

进入代码工程：

```bash
cd chapters/ch03_gpio_interaction/code/class1
make clean
make build
```

如果在 macOS 主机上看到 `linux/gpio.h` 相关错误，这是预期限制。该代码使用 Linux GPIO character device 头文件，需要在目标 Linux 或交叉编译环境中构建。

### 步骤 5：运行 Blink 输出验证

使用必需参数运行：

```bash
./build/blink "$LED_CHIP" "$LED_LINE" "$LED_ACTIVE_LEVEL" "$LED_INACTIVE_LEVEL"
```

观察 5 次以上亮灭周期后按 `Ctrl-C` 停止。也可以指定有限循环次数：

```bash
./build/blink "$LED_CHIP" "$LED_LINE" "$LED_ACTIVE_LEVEL" "$LED_INACTIVE_LEVEL" 500 500 10
```

参数含义为：

```text
chip line active_level inactive_level on_ms off_ms cycles
```

运行时应记录命令、终端输出、LED 现象和停止方式。若程序退出前把 LED 设为无效电平，应记录停止后的 LED 状态。

### 步骤 6：修改周期并比较现象

把亮和灭的时间改成明显不对称：

```bash
./build/blink "$LED_CHIP" "$LED_LINE" "$LED_ACTIVE_LEVEL" "$LED_INACTIVE_LEVEL" 100 900 10
```

如果 LED 亮得短、灭得长，说明程序参数和可见现象对应正确。如果现象没有变化，应检查参数是否传入、程序是否重新运行、line 是否被其他进程占用。

## 运行验证

| 验证项 | 预期现象 | 记录 |
| --- | --- | --- |
| 目标系统记录 | 能写出板卡、镜像、登录方式和当前用户 |  |
| GPIO 设备识别 | 能列出 `gpiochip`，或明确记录当前镜像缺口 |  |
| `libgpiod` 工具检查 | 能执行 `gpiodetect`、`gpioinfo`、`gpioset --help`，或明确记录缺失工具 |  |
| LED 参数确认 | 能写出 `LED_CHIP`、`LED_LINE`、`LED_ACTIVE_LEVEL`、`LED_INACTIVE_LEVEL` |  |
| LED 手动控制 | 写入有效电平和无效电平时 LED 状态不同 |  |
| Blink 构建 | `build/blink` 生成 |  |
| Blink 运行 | LED 至少完成 5 次稳定亮灭周期 |  |
| 周期修改 | 修改 `on_ms` 和 `off_ms` 后，亮灭节奏同步变化 |  |
| 安全停止 | 停止后 LED 回到无效电平，或有明确恢复命令和记录 |  |

课堂练习：

1. 把 `on_ms/off_ms` 从 `500/500` 改为 `100/900`，记录现象变化。
2. 故意交换 `LED_ACTIVE_LEVEL` 和 `LED_INACTIVE_LEVEL`，观察现象是否相反，再恢复正确参数。
3. 用自己的话解释：为什么课程先用 `gpioset` 手动验证，再运行 C 程序？

## 常见问题

| 现象 | 可能原因 | 处理方式 |
| --- | --- | --- |
| `gpiodetect` 或 `gpioset` 不存在 | 镜像未安装 `libgpiod` 命令行工具 | 记录镜像和错误输出，等待补安装方式或替代接口 |
| `/dev/gpiochip*` 不存在 | 内核配置、设备树或镜像未暴露 GPIO character device | 记录为板级问题，不硬填 GPIO 参数 |
| 找不到 LED line | 板级资料不足，或 LED 被其他驱动管理 | 查阅 `boards/` 和真实验证记录，用 `gpioinfo` 辅助判断 |
| LED 不亮 | line 错误、有效电平错误、外接 LED 极性错误、限流电阻缺失、权限不足或 pinmux 不匹配 | 分别写 `0` 和 `1`，复查接线、权限和板级资料 |
| LED 现象相反 | 有效电平填反 | 交换 `LED_ACTIVE_LEVEL` 和 `LED_INACTIVE_LEVEL` 后重新验证 |
| `Permission denied` | 当前用户没有访问 `/dev/gpiochip*` 权限 | 临时使用 `sudo`，长期方案写入板级准备文档 |
| `linux/gpio.h: No such file or directory` | 编译环境不是 Linux 目标环境，或缺少内核头文件 | 在目标板上编译，或安装目标 sysroot/kernel headers |
| 程序运行后 LED 不受 `gpioset` 控制 | line 被正在运行的程序占用 | 停止 Blink 程序后再执行手动命令 |

## 本章成果

完成本课后，应提交以下成果：

- 一份目标系统记录，包含板卡、镜像、登录方式和当前用户。
- 一份 LED GPIO 资源记录，包含 chip、line、有效电平、无效电平和权限口径。
- 一次 LED 手动亮灭验证记录，包含命令和可见现象。
- 一个 Blink 构建和运行记录，包含运行命令、终端输出、LED 现象和停止方式。
- 一次周期修改对比记录，说明参数变化和 LED 现象之间的关系。
- 尚未解决的板级资料缺口，例如 K1 / Muse Pi Pro 最终 LED line、权限和 `gpioset` 命令版本。
