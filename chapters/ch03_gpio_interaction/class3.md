# Class 3: GPIO 输入读取按键

## 本章目标

- 理解 Linux 用户态 GPIO 输入读取路径。
- 使用 GPIO 输入读取按键空闲和按下状态。
- 完成 Button 原始状态观察。
- 记录按键所在 `gpiochip`、line、有效电平和访问权限。

## 前置条件

- 已完成 ch03 class2 的 GPIO 输出实验。
- 已准备板载按键或外接按键。
- 已确认目标系统可以访问 GPIO character device 或可记录当前缺口。

本课时重点建立按键输入的观察方法：确认空闲电平、按下电平和状态变化日志，并如实记录按键可能出现的重复跳变现象。

## 知识简介

GPIO 输入让程序感知外部状态。按键实验的最小目标，是读取按键未按下和按下时的电平，并在状态变化时输出日志。学生需要理解空闲电平、有效电平、上拉/下拉和采样周期，而不是把某个开发板的 line 编号当成通用结论。

如果按键已经接入 Linux input 子系统，或者被其他驱动占用，应记录为板级差异。本课时优先使用明确可读的 GPIO line 或外接按键。

## 环境准备

| 项目 | 要求 | 检查方式 |
| --- | --- | --- |
| GPIO 接口 | 优先确认 `/dev/gpiochip*` | `ls /dev/gpiochip*` |
| 工具 | `gpioget` 可用 | `gpioget --help` |
| 按键 | 板载按键或外接按键 | 能确认连接位置和上拉/下拉 |
| 权限 | 当前用户可读取 GPIO | 直接读取或明确 `sudo` |

### 按键参数记录

| 参数 | 含义 | 当前状态 |
| --- | --- | --- |
| `BUTTON_CHIP` | 按键所在的 `gpiochip` | 待补 |
| `BUTTON_LINE` | 按键对应 line 编号 | 待补 |
| `BUTTON_ACTIVE_LEVEL` | 按键按下时读到的有效电平 | 待补 |
| `GPIO_ACCESS` | 是否需要 `sudo`、用户组或 udev 规则 | 待补 |
| `LIBGPIOD_PROFILE` | 当前镜像采用的 `gpioget` 命令口径 | 待补 |

## 操作步骤

### 步骤 1：确认 GPIO 输入访问方式

```bash
ls /dev/gpiochip*
gpiodetect
gpioinfo
gpioget --version
gpioget --help
```

### 步骤 2：读取按键空闲和按下电平

```bash
gpioget -c "$BUTTON_CHIP" "$BUTTON_LINE"
```

先在不按键时读取一次，再按住按键读取一次。若当前版本不支持 `-c/--chip`，按该版本帮助信息改写。

记录格式：

```text
未按下读数：
按下读数：
BUTTON_ACTIVE_LEVEL：
```

### 步骤 3：连续观察 Button 状态

```bash
cd chapters/ch03_gpio_interaction/code/class3
make build
./build/button_read "$BUTTON_CHIP" "$BUTTON_LINE" "$BUTTON_ACTIVE_LEVEL"
```

该程序用于记录按键状态变化。一次按键出现多次输出时，应保留日志，作为分析机械按键抖动和交互稳定性的实验证据。

## 运行验证

| 验证项 | 预期现象 | 记录 |
| --- | --- | --- |
| Button 参数确认 | 能写出 `BUTTON_CHIP`、`BUTTON_LINE`、`BUTTON_ACTIVE_LEVEL` |  |
| Button 单次读取 | 未按下和按下读数不同 |  |
| Button 连续观察 | 按下和松开时终端输出状态变化 |  |
| 原始抖动记录 | 若出现重复跳变，能如实记录 |  |

## 常见问题

- 按键读数一直不变：检查 line、接线、上拉/下拉、权限，以及按键是否接入 input 子系统。
- 按一次键输出多次：这是机械按键常见的抖动现象，保留日志并记录触发次数、持续时间和复现条件。
- `gpioget` 不存在：记录镜像和错误输出，等待教师补安装方式或替代接口。

## 本章成果

- 一份按键 GPIO 资源记录。
- 一份空闲/按下读数记录。
- 一个 Button 原始状态观察日志。
