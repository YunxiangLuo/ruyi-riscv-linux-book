# Class 1: GPIO 输出与输入

## 本章目标

本课时对应 ch03 的 3.1 和 3.2，目标是让学生完成第一次真实板级交互：

- 能说明 GPIO 输出和输入在 Linux 用户态程序中的基本路径。
- 能使用 GPIO 输出控制 LED 亮灭，并完成 Blink 现象验证。
- 能使用 GPIO 输入读取按键状态，并完成 Button 现象验证。
- 能把“程序状态、GPIO 电平、引脚连接、可见现象”对应起来。
- 能记录板级参数，为 ch08 综合项目中的运行心跳、告警灯、手动触发和模式切换预留接口。

## 前置条件

- 已完成 ch01，能够登录目标系统或临时验证板。
- 已完成 ch02，能够理解 host、target、工程目录和基本构建流程。
- 已确认目标环境可以访问 Linux shell。
- 已准备 LED 输出资源和按键输入资源，资源可以是板载器件，也可以是外接 LED 与按键。
- 已阅读 `boards/licheepi4a/README.md` 中关于 LicheePi 4A 只是临时验证板的说明。
- 已阅读 `boards/` 中对应板卡说明，知道本课时最终 GPIO 参数必须由真实板卡实测记录补齐。

本课时不要求学生理解寄存器、pinmux 细节或内核驱动实现。若引脚不可用、GPIO 编号不确定、设备树未使能，应先记录为板级问题，不在课堂中临时展开成驱动课。

## 知识简介

GPIO 是 General Purpose Input/Output 的缩写，也就是通用输入输出。对应用开发课程来说，GPIO 的价值不在于背诵芯片寄存器，而在于建立最短的硬件反馈链路：程序写出一个输出值，LED 产生可见变化；程序读取一个输入值，按键动作能被软件感知。这个链路一旦建立，后续的串口日志、传感器采集、OLED 显示、多线程任务和联网项目才有真实的板级落点。

在 Linux 开发板上，GPIO 通常由内核驱动管理，用户态程序通过设备节点、命令行工具或库函数访问 GPIO。课程默认推荐使用现代 Linux 的 GPIO character device 方式，也就是通过 `gpiochip` 和 line 访问 GPIO；如果具体镜像暂时只提供旧的 sysfs GPIO，则作为兼容方案记录到板卡说明中。无论采用哪种接口，本课时都只关注三个问题：第一个，目标板上哪一个 GPIO 可以安全连接 LED；第二个，哪一个 GPIO 可以安全读取按键；第三个，学生如何用可观察现象证明程序确实控制了硬件。

GPIO 输出实验对应 Blink。学生让 LED 按固定周期亮灭，从而理解输出方向、有效电平、延时和循环。GPIO 输入实验对应 Button。学生读取按键状态并输出到终端，或用按键状态影响 LED，从而理解输入方向、上拉/下拉、有效电平和采样。消抖和状态机放到 class2，本课时只要求学生拿到稳定可观察的原始输入输出。

## 环境准备

### 目标环境

| 项目 | 要求 | 检查方式 |
| --- | --- | --- |
| 目标板 | K1 / Muse Pi Pro 为课程目标板；K1 暂不可用时可用 LicheePi 4A 临时验证 | 查看 `boards/` 中对应记录 |
| 系统 | 可进入 Linux shell | SSH、串口或本地终端登录成功 |
| GPIO 接口 | 优先确认 `/dev/gpiochip*`；如无该接口，再确认是否有 sysfs GPIO | 执行 `ls /dev/gpiochip*` 或检查 `/sys/class/gpio` |
| 工具 | 优先使用 `libgpiod` 命令行工具进行课堂验证 | 执行 `gpiodetect`、`gpioinfo` |
| 权限 | 当前用户能访问 GPIO 设备 | 直接执行 GPIO 命令，或确认是否需要 `sudo` |

### 接口选择规则

Linux 上同一个 LED 或按键不一定都适合用普通 GPIO line 访问。开课前先按以下顺序判断：

- 板载 LED 如果已经出现在 `/sys/class/leds/`，可先按 LED 子系统记录；普通 GPIO 输出练习优先使用外接 LED 或明确未被驱动占用的 GPIO line。
- 板载按键如果已经出现在 `/dev/input/event*` 或 `/proc/bus/input/devices`，可先按 input 子系统记录；普通 GPIO 输入练习优先使用外接按键或明确可读的 GPIO line。
- 旧 sysfs GPIO 只作为兼容路径记录到板卡说明中，新课程默认用 GPIO character device 和 `libgpiod`。

### 板级参数

以下参数目前不能硬填。开课前必须由板卡验证记录补齐。

| 参数 | 含义 | 当前状态 |
| --- | --- | --- |
| `LED_CHIP` | LED 所在的 `gpiochip`，例如 `gpiochip0` | 待补 |
| `LED_LINE` | LED 对应 line 编号 | 待补 |
| `LED_ACTIVE_LEVEL` | LED 有效电平，`1` 表示高电平亮，`0` 表示低电平亮 | 待补 |
| `BUTTON_CHIP` | 按键所在的 `gpiochip` | 待补 |
| `BUTTON_LINE` | 按键对应 line 编号 | 待补 |
| `BUTTON_ACTIVE_LEVEL` | 按键按下时读到的有效电平 | 待补 |
| `GPIO_ACCESS` | 是否需要 `sudo`、用户组或 udev 规则 | 待补 |
| `LIBGPIOD_PROFILE` | 当前镜像采用的 `gpioset`/`gpioget` 命令口径 | 待补 |

如果这些参数没有确认，本课时可以讲解原理和流程，但不能宣称已经完成真实板级验证。

### 资料来源

- `docs/course-outline.md`：ch03 目标、三级小节、配套实验和章节成果。
- `docs/course-plan.md`：3.1、3.2 的交付要求和 ch08 复用点。
- Linux kernel GPIO character device 文档：Linux 用户态 GPIO 访问模型。
- libgpiod 官方文档：`gpiodetect`、`gpioinfo`、`gpioset`、`gpioget` 命令口径。
- `boards/k1/README.md`：K1 / Muse Pi Pro 目标板资料、验证边界和待补项。
- `boards/licheepi4a/README.md`：LicheePi 4A 临时验证边界和待补板级资料。

## 操作步骤

### 步骤 1：确认 GPIO 访问方式

先判断当前目标系统是否暴露 GPIO character device。

```bash
ls /dev/gpiochip*
gpiodetect
gpioinfo
gpioset --version
gpioget --version
gpioset --help
gpioget --help
```

若能看到 `gpiochip` 列表，课堂优先使用 `libgpiod` 工具链。教师需要带领学生从 `gpioinfo` 输出中找到 LED 和按键所在 line，并记录到实验表中。

如果 `gpiodetect` 不存在，说明镜像可能没有预装 `libgpiod` 工具。此时先记录为环境缺口，不要直接假设 GPIO 编号。后续可选择安装工具、换用镜像已提供的 GPIO 工具，或在板级 README 中登记 sysfs 兼容路径。

`libgpiod` 1.x 和 2.x 的命令参数可能不同。课程材料只给出模板，最终命令必须以当前系统的 `gpioset --help` 和 `gpioget --help` 为准。若帮助信息支持 `-c/--chip`，优先采用 `-c "$CHIP" "$LINE=$VALUE"` 口径；若不支持，则采用该版本文档中的 `"$CHIP" "$LINE=$VALUE"` 口径。

### 步骤 2：确认 LED 输出资源

确认 LED 是板载 LED 还是外接 LED。外接 LED 必须确认限流电阻、电压等级和接线方向。

课堂上只要求学生完成以下记录：

| 记录项 | 示例写法 | 学生填写 |
| --- | --- | --- |
| LED 类型 | 板载 LED / 外接 LED |  |
| 连接位置 | `gpiochipX line Y` 或板卡丝印 |  |
| 有效电平 | 高电平亮 / 低电平亮 |  |
| 访问权限 | 普通用户 / `sudo` |  |

在未确认有效电平之前，不能把“写 1 等于点亮”写成固定结论。不同开发板和不同外接电路可能相反。

### 步骤 3：手动控制 LED 亮灭

使用 `gpioset` 做最小输出验证。下面命令中的 `LED_CHIP`、`LED_LINE` 和电平值必须替换为板级参数。

若当前 `gpioset --help` 支持 `-c/--chip`：

```bash
# 设置为有效电平，观察 LED 是否点亮
gpioset -c "$LED_CHIP" "$LED_LINE=1"

# 设置为无效电平，观察 LED 是否熄灭
gpioset -c "$LED_CHIP" "$LED_LINE=0"
```

若当前版本不支持 `-c/--chip`，按该版本帮助信息改写，例如：

```bash
gpioset "$LED_CHIP" "$LED_LINE=1"
gpioset "$LED_CHIP" "$LED_LINE=0"
```

若 LED 的有效电平是低电平，应将上面的 `1` 和 `0` 对调。教学重点是让学生记录“写入值”和“LED 现象”的对应关系，而不是记住某一个开发板的固定编号。

### 步骤 4：完成 Blink 输出验证

Blink 的核心逻辑是循环切换 GPIO 输出值，并在两次切换之间等待一小段时间。

```text
初始化 GPIO 为输出
循环执行：
    写入 LED 有效电平
    等待 500 ms
    写入 LED 无效电平
    等待 500 ms
退出时释放 GPIO
```

如果使用命令行做课堂快速验证，可以先使用以下形式观察现象：

```bash
LED_CHIP=gpiochipX
LED_LINE=Y
LED_ACTIVE_LEVEL=1
LED_INACTIVE_LEVEL=0

while true; do
    gpioset -c "$LED_CHIP" "$LED_LINE=$LED_ACTIVE_LEVEL"
    sleep 0.5
    gpioset -c "$LED_CHIP" "$LED_LINE=$LED_INACTIVE_LEVEL"
    sleep 0.5
done
```

如果当前 `gpioset` 不支持 `-c/--chip`，将循环中的两条 `gpioset -c ...` 改为本机帮助信息对应的旧口径。若 `gpioset` 在当前版本中会保持 line、阻塞等待，或在命令退出后释放 line 导致 LED 状态不能保持，应记录工具版本和现象，改用该版本支持的保持/超时模式，或直接使用代码工程中的 Blink 程序。

代码工程路径：

```bash
cd chapters/ch03_gpio_interaction/code/class1
make build
./build/blink "$LED_CHIP" "$LED_LINE" "$LED_ACTIVE_LEVEL" "$LED_INACTIVE_LEVEL"
```

`blink` 程序会持续占用 GPIO line，并在退出前尝试把 LED 恢复到无效电平。它直接使用 Linux GPIO character device ioctl，不依赖 `libgpiod-dev` 的 C API 版本；因此仍应先用 `gpiodetect`、`gpioinfo`、`gpioset`、`gpioget` 完成板级参数确认。

### 步骤 5：确认按键输入资源

按键输入要关注三件事：line 编号、空闲电平、按下电平。

```bash
gpioget -c "$BUTTON_CHIP" "$BUTTON_LINE"
```

如果当前 `gpioget` 不支持 `-c/--chip`，按该版本帮助信息改写，例如 `gpioget "$BUTTON_CHIP" "$BUTTON_LINE"`。先在不按键时读取一次，再按下按键读取一次。学生应记录两次读数。

| 状态 | 读数 | 结论 |
| --- | --- | --- |
| 未按下 |  | 空闲电平 |
| 按下 |  | 有效电平 |

如果两次读数没有变化，优先检查按键是否接在预期 GPIO、引脚复用是否正确、按键是否需要外部上拉/下拉、电路是否共地，以及当前用户是否有权限读取 GPIO。

### 步骤 6：完成 Button 输入验证

Button 的最小目标是让按键状态在终端上可观察。程序可以每隔一小段时间读取一次 GPIO，并打印状态变化。

```text
初始化 GPIO 为输入
记录上一次按键状态
循环执行：
    读取当前按键状态
    如果状态变化：
        打印 pressed 或 released
    等待 100 ms
退出时释放 GPIO
```

本课时只观察原始状态，不要求消抖。若学生看到一次按键产生多次打印，应把现象留到 class2 讨论，不在本课时临时补复杂逻辑。

代码工程路径：

```bash
cd chapters/ch03_gpio_interaction/code/class1
make build
./build/button_read "$BUTTON_CHIP" "$BUTTON_LINE" "$BUTTON_ACTIVE_LEVEL"
```

`button_read` 只按固定周期轮询并打印状态变化，不做消抖。一次按键出现多次 `pressed` / `released` 输出时，应记录为机械按键抖动现象。

### 步骤 7：形成 ch08 可复用记录

本课时结束前，每组学生应给出一份 GPIO 资源记录。ch08 综合项目会复用这些信息实现运行心跳、告警灯、手动触发或模式切换。

```text
目标环境：
LED_CHIP：
LED_LINE：
LED_ACTIVE_LEVEL：
BUTTON_CHIP：
BUTTON_LINE：
BUTTON_ACTIVE_LEVEL：
访问权限：
Blink 现象：
Button 现象：
遗留问题：
```

## 运行验证

| 验证项 | 预期现象 | 记录 |
| --- | --- | --- |
| GPIO 设备识别 | 能列出 `gpiochip` 或明确记录当前镜像缺少的 GPIO 接口 |  |
| LED 手动控制 | 写入有效电平时 LED 状态变化 |  |
| Blink | LED 按固定周期亮灭，停止程序后能恢复到安全状态 |  |
| Button 空闲读数 | 不按键时读数稳定 |  |
| Button 按下读数 | 按下时读数发生变化 |  |
| Button 状态输出 | 终端能看到 pressed/released 或等价状态输出 |  |
| ch08 复用记录 | LED 和按键资源记录完整，可供后续项目引用 |  |

## 常见问题

### `gpiodetect` 或 `gpioinfo` 不存在

现象：执行命令提示 `command not found`。

原因：当前镜像没有安装 `libgpiod` 工具，或工具名称与发行版包不同。

处理：先记录镜像名称和命令输出；不要自行填写 GPIO 编号。教师应补充安装方式、替代工具或 sysfs 兼容方案。

### 找到了 `gpiochip`，但不知道哪一个 line 接 LED

现象：`gpioinfo` 输出很多 line，无法判断目标 line。

原因：缺少板级引脚表、原理图或已验证记录。

处理：查阅 K1 / Muse Pi Pro 或 LicheePi 4A 的板级资料；若仍无法确认，将该项列为资料缺口，不能在讲义中硬写。

### 写入 GPIO 后 LED 没有变化

现象：命令执行无报错，但 LED 不亮或不灭。

原因：可能是 GPIO line 不正确、LED 有效电平相反、引脚被其他功能复用、外接电路错误、未共地或权限不足。

处理：先用手动控制命令分别写 `0` 和 `1`；确认电路和权限；再检查设备树或 pinmux 是否把该引脚配置为 GPIO。

### 按键读数一直不变

现象：按下和松开按键时，`gpioget` 读数相同。

原因：可能是按键接线错误、缺少上拉/下拉、line 编号不对、按键实际接入 input 子系统而不是普通 GPIO。

处理：先确认按键硬件路径。如果该按键是 Linux input 设备，应记录为板级差异，本课时改用可访问的 GPIO 输入或外接按键。

### 按一次键打印多次

现象：按键只按了一次，终端输出多次 pressed/released。

原因：机械按键抖动或轮询周期过短。

处理：这是 class2 的内容。本课时只记录现象，下一课时用轮询与消抖解决。

### 需要 `sudo` 才能访问 GPIO

现象：普通用户执行 GPIO 命令提示权限不足。

原因：`/dev/gpiochip*` 权限或用户组未配置。

处理：课堂上可以临时使用 `sudo` 完成验证；课程镜像应在板级准备阶段统一处理权限，避免学生每一步都被权限问题打断。

## 本章成果

完成本课时后，学生应提交：

- 一份目标环境 GPIO 资源记录。
- 一次 LED 手动亮灭验证记录。
- 一个 Blink 运行记录，包含命令、现象和停止方式。
- 一个 Button 读取记录，包含空闲读数、按下读数和终端输出。
- 一份遗留问题清单，特别是 GPIO 编号、有效电平、权限、libgpiod/sysfs 差异。

本课时成果支撑 ch08 的状态指示和人工触发能力。只要 LED 输出和按键输入被准确记录，后续 class2 的消抖、状态机，以及 ch08 的告警灯、运行心跳和模式切换就可以在同一套资源上继续扩展。
