# Lab 2: GPIO 输出控制 LED

## 实验目标

- 使用 GPIO 输出控制 LED。
- 完成 LED 手动亮灭和 Blink 验证。
- 记录目标板或临时验证板上的 LED GPIO 参数。
- 区分主机环境、目标环境和板级待验证参数。

## 实验环境

| 项目 | 要求 | 记录 |
| --- | --- | --- |
| Host | Linux、macOS、WSL 或课程指定主机；用于编辑、传输和记录 |  |
| Target | K1 / Muse Pi Pro；K1 暂不可用时可用 LicheePi 4A 临时验证 |  |
| 登录方式 | SSH、串口或本地终端 |  |
| GPIO 接口 | 优先使用 `/dev/gpiochip*` |  |
| 工具 | `gpiodetect`、`gpioinfo`、`gpioset` |  |
| 编译环境 | 目标 Linux 或可用的 RISC-V Linux sysroot |  |

说明：本代码工程使用 Linux GPIO character device ioctl，不能在普通 macOS 主机上直接完成最终构建验证。macOS 可用于阅读代码和整理记录，最终 `make build` 需要在目标 Linux 或交叉编译环境中验证。

## 实验任务

1. 登录目标系统并记录系统信息。
2. 检查 `/dev/gpiochip*` 和 `libgpiod` 工具。
3. 填写 `LED_CHIP`、`LED_LINE`、`LED_ACTIVE_LEVEL`、`LED_INACTIVE_LEVEL`。
4. 使用 `gpioset` 完成 LED 手动亮灭。
5. 使用 `code/class2` 中的 Blink 程序完成闪烁验证。

## 实验步骤

### 任务 1：记录目标系统

```bash
hostname
uname -a
cat /etc/os-release
whoami
pwd
```

记录：

```text
板卡：
系统镜像：
登录方式：
当前用户：
是否需要 sudo：
```

### 任务 2：检查 GPIO 工具和命令口径

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

如果 `gpioset --help` 支持 `-c/--chip`，本实验使用 `gpioset -c "$LED_CHIP" "$LED_LINE=$VALUE"`。如果不支持，按当前工具帮助信息改写，并在记录中写明 `LIBGPIOD_PROFILE`。

### 任务 3：填写 LED 参数

```bash
LED_CHIP=gpiochipX
LED_LINE=Y
LED_ACTIVE_LEVEL=1
LED_INACTIVE_LEVEL=0
```

注意：上面的 `gpiochipX` 和 `Y` 只是填写格式，不是课程给出的真实参数。最终参数必须来自 K1 / Muse Pi Pro 实测记录，或明确标注为 LicheePi 4A 临时验证记录。

### 任务 4：手动点亮和熄灭 LED

```bash
gpioset -c "$LED_CHIP" "$LED_LINE=$LED_ACTIVE_LEVEL"
sleep 1
gpioset -c "$LED_CHIP" "$LED_LINE=$LED_INACTIVE_LEVEL"
```

记录：

```text
写入有效电平后的 LED 现象：
写入无效电平后的 LED 现象：
```

### 任务 5：构建 Blink 程序

```bash
cd chapters/ch03_gpio_interaction/code/class2
make clean
make build
```

如果在 macOS 主机上看到 `This project must be built on Linux because it uses linux/gpio.h.`，这是预期限制，应改到目标 Linux 或交叉编译环境中执行。

### 任务 6：运行 Blink

```bash
./build/blink "$LED_CHIP" "$LED_LINE" "$LED_ACTIVE_LEVEL" "$LED_INACTIVE_LEVEL"
```

观察 5 次以上亮灭周期后按 `Ctrl-C` 停止。也可以指定有限循环次数：

```bash
./build/blink "$LED_CHIP" "$LED_LINE" "$LED_ACTIVE_LEVEL" "$LED_INACTIVE_LEVEL" 500 500 10
```

### 任务 7：修改周期并复测

```bash
./build/blink "$LED_CHIP" "$LED_LINE" "$LED_ACTIVE_LEVEL" "$LED_INACTIVE_LEVEL" 100 900 10
```

记录 LED 亮灭节奏是否随参数变化。

## 运行验证

| 验证项 | 预期现象 | 是否通过 |
| --- | --- | --- |
| GPIO 工具检查 | 能执行 `gpiodetect` 和 `gpioinfo`，或明确记录缺失工具 |  |
| LED 参数确认 | 能写出 LED chip、line、有效电平 |  |
| LED 手动控制 | 写入有效电平和无效电平时 LED 状态不同 |  |
| Blink | LED 至少完成 5 次稳定亮灭周期 |  |
| 安全停止 | 停止后 LED 回到无效电平或有明确说明 |  |
| 周期修改 | 修改 `on_ms` / `off_ms` 后现象同步变化 |  |

## 常见问题

| 现象 | 可能原因 | 处理方式 |
| --- | --- | --- |
| `gpioset: command not found` | 镜像缺少 `libgpiod` 工具 | 记录镜像和错误输出，补安装方式或换用已验证镜像 |
| `/dev/gpiochip*` 不存在 | 内核配置、设备树或镜像未暴露 GPIO character device | 记录为板级问题，不硬填 GPIO 参数 |
| LED 不亮 | line 错误、有效电平相反、外接 LED 极性错误、未串限流电阻、权限不足 | 分别写 `0` 和 `1`，复查接线和权限 |
| `Permission denied` | 当前用户不能访问 GPIO 设备 | 临时使用 `sudo`，长期方案写入板级准备文档 |
| macOS 构建失败 | 代码依赖 Linux `linux/gpio.h` | 在目标 Linux 或交叉编译环境中构建 |
| 停止后 LED 状态异常 | 程序退出前未能恢复无效电平或 line 被占用 | 记录现象，手动写入无效电平并排查占用 |

## 提交要求

- 板卡、系统、登录方式和当前用户记录。
- GPIO 工具检查结果。
- LED GPIO 参数表。
- LED 手动亮灭记录。
- Blink 运行日志或截图。
- 修改周期后的现象记录。
- 尚未解决的板级资料缺口。
