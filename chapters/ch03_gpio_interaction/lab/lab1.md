# Lab 1: Blink 与 Button

## 实验目标

本实验对应 ch03 class1，围绕 GPIO 输出和 GPIO 输入完成两个可观察任务：

- 使用 GPIO 输出控制 LED，完成 Blink。
- 使用 GPIO 输入读取按键，完成 Button。
- 记录目标板或临时验证板上的 GPIO 资源参数。
- 为 ch08 综合项目预留运行心跳、告警灯、手动触发和模式切换资源。

本实验只做原始输入输出验证。按键消抖和 LED 模式状态机放到 Lab 3-2。

## 实验环境

| 项目 | 要求 |
| --- | --- |
| 主机环境 | 能通过 SSH、串口或本地终端操作目标系统 |
| 目标环境 | K1 / Muse Pi Pro 为目标板；K1 暂不可用时使用 LicheePi 4A 临时验证 |
| GPIO 接口 | 优先使用 `/dev/gpiochip*` 和 `libgpiod` 工具 |
| 硬件连接 | 板载或外接 LED；板载或外接按键 |
| 软件依赖 | `gpiodetect`、`gpioinfo`、`gpioset`、`gpioget`；若缺失需记录 |
| 权限 | 当前用户可访问 GPIO，或明确使用 `sudo` |

## 实验前必须确认

以下信息没有确认时，不要直接执行输出控制命令。

| 参数 | 说明 | 实验记录 |
| --- | --- | --- |
| 使用板卡 | K1 / Muse Pi Pro 或 LicheePi 4A 临时验证板 |  |
| 系统镜像 | 镜像名称、版本或下载来源 |  |
| 登录方式 | SSH / 串口 / 本地终端 |  |
| `LED_CHIP` | LED 对应 `gpiochip` |  |
| `LED_LINE` | LED 对应 line 编号 |  |
| `LED_ACTIVE_LEVEL` | LED 有效电平，填 `1` 或 `0` |  |
| `BUTTON_CHIP` | 按键对应 `gpiochip` |  |
| `BUTTON_LINE` | 按键对应 line 编号 |  |
| `BUTTON_ACTIVE_LEVEL` | 按键按下时读数，填 `1` 或 `0` |  |
| 是否需要 `sudo` | 普通用户 / `sudo` / 其他权限方式 |  |
| `LIBGPIOD_PROFILE` | `-c/--chip` 口径 / 旧口径 / 其他 |  |

当前项目资料中尚未补齐 K1 / Muse Pi Pro 和 LicheePi 4A 的最终 GPIO 参数。教师需要保存真实板卡命令输出、连接方式和现象记录，再补齐本表；建议记录为 `boards/<board>/validation-YYYY-MM-DD.md`。

## 实验任务

### 任务 1：确认 GPIO 设备和板级参数

学生需要确认系统是否暴露 GPIO character device，并记录可用的 `gpiochip`。

```bash
ls /dev/gpiochip*
gpiodetect
gpioinfo
gpioset --version
gpioget --version
gpioset --help
gpioget --help
```

如果命令不存在，记录失败输出，不要跳过。失败本身说明课程镜像或板级准备还缺少 GPIO 工具。

根据 `gpioset --help` 和 `gpioget --help`，先确定命令口径：

```text
LIBGPIOD_PROFILE=
是否支持 -c/--chip：
是否支持 -t/--toggle 或超时/保持参数：
```

### 任务 2：完成 LED 手动控制

学生需要用 GPIO 输出让 LED 出现至少一次明确的亮灭变化，并记录写入值和 LED 现象。

若当前 `gpioset` 支持 `-c/--chip`：

```bash
gpioset -c "$LED_CHIP" "$LED_LINE=$LED_ACTIVE_LEVEL"
gpioset -c "$LED_CHIP" "$LED_LINE=$LED_INACTIVE_LEVEL"
```

若当前版本不支持 `-c/--chip`，按该版本帮助信息改写，例如：

```bash
gpioset "$LED_CHIP" "$LED_LINE=$LED_ACTIVE_LEVEL"
gpioset "$LED_CHIP" "$LED_LINE=$LED_INACTIVE_LEVEL"
```

若 LED 低电平有效，应反向记录：

```text
LED_ACTIVE_LEVEL=0
点亮：写 0
熄灭：写 1
```

### 任务 3：完成 Blink

学生需要让 LED 按固定周期闪烁，建议周期为 1 秒。

命令行快速验证：

```bash
LED_CHIP=gpiochipX
LED_LINE=Y
LED_ACTIVE_LEVEL=1
LED_INACTIVE_LEVEL=0

gpio_set() {
    gpioset -c "$1" "$2=$3"
}

while true; do
    gpio_set "$LED_CHIP" "$LED_LINE" "$LED_ACTIVE_LEVEL"
    sleep 0.5
    gpio_set "$LED_CHIP" "$LED_LINE" "$LED_INACTIVE_LEVEL"
    sleep 0.5
done
```

按 `Ctrl-C` 停止后，执行一次安全输出：

```bash
gpio_set "$LED_CHIP" "$LED_LINE" "$LED_INACTIVE_LEVEL"
```

如果当前 `gpioset` 不支持 `-c/--chip`，将 `gpio_set()` 改为该版本帮助信息对应的旧口径。若当前 `gpioset` 会保持 line、阻塞等待，或命令退出后不能保持输出状态，应记录工具版本和现象，后续改用该版本支持的保持/超时参数，或改用代码工程中的持续占用 line 的 Blink 程序。

### 任务 4：完成 Button 读数

学生需要读取按键未按下和按下时的读数。

```bash
# 不按键时执行一次
gpioget -c "$BUTTON_CHIP" "$BUTTON_LINE"

# 按住按键时再执行一次
gpioget -c "$BUTTON_CHIP" "$BUTTON_LINE"
```

如果当前 `gpioget` 不支持 `-c/--chip`，按该版本帮助信息改写，例如 `gpioget "$BUTTON_CHIP" "$BUTTON_LINE"`。

记录格式：

```text
未按下读数：
按下读数：
BUTTON_ACTIVE_LEVEL：
```

若读数没有变化，先不要改实验目标，按常见问题排查。

### 任务 5：完成 Button 状态观察

学生需要连续观察按键状态变化。可以先用命令行循环完成课堂验证。

```bash
BUTTON_CHIP=gpiochipX
BUTTON_LINE=Y

gpio_get() {
    gpioget -c "$1" "$2"
}

last=""
while true; do
    now=$(gpio_get "$BUTTON_CHIP" "$BUTTON_LINE")
    if [ "$now" != "$last" ]; then
        date +"%H:%M:%S button=$now"
        last="$now"
    fi
    sleep 0.1
done
```

按下和松开按键时，终端应输出不同状态。若一次按键出现多次跳变，记录为“按键抖动现象”，下一次实验再处理。

## 实验步骤

### 步骤 1：登录目标系统

记录使用的登录方式。

```bash
hostname
uname -a
whoami
```

实验记录：

```text
板卡：
系统：
登录方式：
当前用户：
```

### 步骤 2：检查 GPIO 工具

```bash
which gpiodetect
which gpioinfo
which gpioset
which gpioget
gpiodetect
gpioset --version
gpioget --version
gpioset --help
gpioget --help
```

如果 `which` 查不到工具，记录缺失工具名称，并联系教师补充镜像工具安装方式。

### 步骤 3：填写 GPIO 参数

从教师提供的板级表、`gpioinfo` 输出或实测记录中填写：

```bash
LED_CHIP=gpiochipX
LED_LINE=Y
LED_ACTIVE_LEVEL=1
LED_INACTIVE_LEVEL=0

BUTTON_CHIP=gpiochipX
BUTTON_LINE=Y
BUTTON_ACTIVE_LEVEL=1

gpio_set() {
    gpioset -c "$1" "$2=$3"
}

gpio_get() {
    gpioget -c "$1" "$2"
}
```

注意：上面的值只是填写格式，不是课程给出的真实引脚参数。`gpio_set()` 和 `gpio_get()` 是当前 shell 会话中的临时函数，后续步骤应在同一个终端中执行；如果重新打开终端，需要重新执行本步骤。如果当前 `gpioset`/`gpioget` 不支持 `-c/--chip`，需要按 `--help` 输出改写 `gpio_set()` 和 `gpio_get()`。

### 步骤 4：验证 LED 输出

```bash
gpio_set "$LED_CHIP" "$LED_LINE" "$LED_ACTIVE_LEVEL"
sleep 1
gpio_set "$LED_CHIP" "$LED_LINE" "$LED_INACTIVE_LEVEL"
```

记录：

```text
写入有效电平后的 LED 现象：
写入无效电平后的 LED 现象：
```

### 步骤 5：运行 Blink

```bash
while true; do
    gpio_set "$LED_CHIP" "$LED_LINE" "$LED_ACTIVE_LEVEL"
    sleep 0.5
    gpio_set "$LED_CHIP" "$LED_LINE" "$LED_INACTIVE_LEVEL"
    sleep 0.5
done
```

观察 5 次以上亮灭周期后按 `Ctrl-C` 停止，并执行：

```bash
gpio_set "$LED_CHIP" "$LED_LINE" "$LED_INACTIVE_LEVEL"
```

记录：

```text
LED 是否稳定闪烁：
闪烁周期是否接近 1 秒：
停止后 LED 是否回到安全状态：
```

如果当前 `gpioset` 的保持行为导致命令行循环不可用，改用代码工程验证：

```bash
cd chapters/ch03_gpio_interaction/code/class1
make build
./build/blink "$LED_CHIP" "$LED_LINE" "$LED_ACTIVE_LEVEL" "$LED_INACTIVE_LEVEL"
```

观察 5 次以上亮灭周期后按 `Ctrl-C` 停止。该程序直接使用 Linux GPIO character device ioctl，退出前会尝试把 LED 恢复到无效电平。

### 步骤 6：验证 Button 输入

```bash
echo "release:"
gpio_get "$BUTTON_CHIP" "$BUTTON_LINE"

echo "press:"
gpio_get "$BUTTON_CHIP" "$BUTTON_LINE"
```

第二条命令执行时按住按键。记录未按下和按下的读数。

### 步骤 7：连续观察 Button 状态

```bash
last=""
while true; do
    now=$(gpio_get "$BUTTON_CHIP" "$BUTTON_LINE")
    if [ "$now" != "$last" ]; then
        if [ "$now" = "$BUTTON_ACTIVE_LEVEL" ]; then
            date +"%H:%M:%S pressed"
        else
            date +"%H:%M:%S released"
        fi
        last="$now"
    fi
    sleep 0.1
done
```

按下、松开按键各 3 次，观察终端是否有对应输出。

也可以使用代码工程中的 Button 程序完成同样验证：

```bash
cd chapters/ch03_gpio_interaction/code/class1
make build
./build/button_read "$BUTTON_CHIP" "$BUTTON_LINE" "$BUTTON_ACTIVE_LEVEL"
```

该程序只打印原始状态变化，不做消抖。若一次按键出现多次输出，记录为抖动现象。

### 步骤 8：整理 ch08 复用记录

将本实验最终确认的 GPIO 资源写入实验记录：

```text
ch08 可复用 GPIO 资源：
- 心跳灯/告警灯候选：
- 手动触发/模式切换按键候选：
- 已知限制：
- 是否需要权限处理：
```

## 运行验证

| 验证项 | 预期现象 | 是否通过 |
| --- | --- | --- |
| GPIO 工具检查 | 能执行 `gpiodetect` 和 `gpioinfo`，或明确记录缺失工具 |  |
| LED 参数确认 | 能写出 `LED_CHIP`、`LED_LINE`、`LED_ACTIVE_LEVEL` |  |
| LED 手动控制 | 写入有效电平和无效电平时 LED 状态不同 |  |
| Blink | LED 至少完成 5 次稳定亮灭周期 |  |
| Button 参数确认 | 能写出 `BUTTON_CHIP`、`BUTTON_LINE`、`BUTTON_ACTIVE_LEVEL` |  |
| Button 单次读取 | 未按下和按下读数不同 |  |
| Button 连续观察 | 按下和松开时终端输出状态变化 |  |
| ch08 复用记录 | 明确本实验输出和输入如何进入综合项目 |  |

## 常见问题

| 现象 | 可能原因 | 处理方式 |
| --- | --- | --- |
| `gpiodetect: command not found` | 镜像未安装 `libgpiod` 工具 | 记录镜像和错误输出，等待教师补安装方式或替代接口 |
| `/dev/gpiochip*` 不存在 | 内核配置、设备树或镜像不暴露 GPIO character device | 记录为板级问题，不能硬填引脚 |
| LED 不亮 | line 不对、有效电平相反、外接 LED 极性错误、未串限流电阻、权限不足 | 先检查板级参数和接线，再分别写 `0`、`1` 观察 |
| LED 一直亮或一直灭 | `gpioset` 退出后释放 line，或 LED 被其他功能占用 | 记录工具版本，改用保持模式或后续 Blink 程序 |
| 按键读数不变 | line 不对、按键接线错误、缺少上拉/下拉、按键不是普通 GPIO | 检查板级资料，必要时换外接按键 |
| 按一次键输出多次 | 机械按键抖动 | 记录现象，Lab 3-2 使用轮询与消抖处理 |
| 普通用户权限不足 | `/dev/gpiochip*` 权限未配置 | 临时使用 `sudo`，长期方案写入板级准备文档 |

## 提交要求

- 实验记录：填写板卡、系统、登录方式、GPIO 工具检查结果。
- 运行截图或日志：包含 `gpiodetect`/`gpioinfo` 输出摘要、Blink 运行记录、Button 状态输出。
- 代码工程记录：如果使用代码工程，提交 `make build` 输出摘要、`./build/blink` 和 `./build/button_read` 的运行日志。
- GPIO 参数表：包含 LED 和 Button 的 chip、line、有效电平、访问权限。
- 现象说明：说明 LED 如何亮灭、按键如何读数、是否出现抖动。
- 问题清单：列出尚未解决的板级资料缺口或环境问题。
- ch08 复用说明：说明本实验的 LED 和按键分别准备用在综合项目的哪个位置。

## 需要教师补充的资料

为了把本实验从 V1 变成可直接发给学生的定稿，需要补齐以下资料：

- K1 / Muse Pi Pro 的 LED 和按键 GPIO 参数。
- LicheePi 4A 临时验证板的 LED 和按键 GPIO 参数。
- 当前课程镜像是否预装 `libgpiod` 工具。
- GPIO 访问是否需要 `sudo`、用户组或 udev 规则。
- 如果使用外接 LED 和按键，需要提供接线图、限流电阻建议、上拉/下拉说明和实物照片。
