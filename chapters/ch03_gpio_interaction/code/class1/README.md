# ch03 class1 代码工程：Blink 与 Button

## 项目目标

本工程对应 ch03 class1 和 Lab 3-1，用两个最小程序完成 GPIO 输出和 GPIO 输入验证：

- `blink`：持续占用一个 GPIO line，让 LED 按固定周期亮灭。
- `button_read`：持续读取一个 GPIO line，在按键状态变化时打印 `pressed` 或 `released`。

代码使用 Linux GPIO character device ioctl 访问 `/dev/gpiochip*`，不依赖 `libgpiod-dev` 的 C API 版本。课堂上仍建议先用 `gpiodetect`、`gpioinfo`、`gpioset`、`gpioget` 确认板级参数和命令口径。

## 前置条件

- 目标系统是 Linux，并暴露 `/dev/gpiochip*`。
- 已根据 `boards/<board>/validation-YYYY-MM-DD.md` 或教师提供的板级表确认 GPIO 参数。
- 当前用户有 GPIO 访问权限；如果没有，运行时需要加 `sudo`。
- 已确认外接 LED 有限流电阻，外接按键有明确上拉或下拉。

需要填入的板级参数：

```bash
LED_CHIP=gpiochipX
LED_LINE=Y
LED_ACTIVE_LEVEL=1
LED_INACTIVE_LEVEL=0

BUTTON_CHIP=gpiochipX
BUTTON_LINE=Y
BUTTON_ACTIVE_LEVEL=1
```

上面的 `gpiochipX` 和 `Y` 只是占位格式，不是课程最终参数。

## 构建方法

在目标板或交叉编译环境中执行：

```bash
make build
```

默认使用 `cc`。交叉编译时可以覆盖编译器：

```bash
make build CC=riscv64-linux-gnu-gcc
```

## 运行方法

### Blink

```bash
./build/blink "$LED_CHIP" "$LED_LINE" "$LED_ACTIVE_LEVEL" "$LED_INACTIVE_LEVEL"
```

可选参数：

```bash
./build/blink "$LED_CHIP" "$LED_LINE" "$LED_ACTIVE_LEVEL" "$LED_INACTIVE_LEVEL" 500 500 10
```

含义依次是：

```text
chip line active_level inactive_level on_ms off_ms cycles
```

- `on_ms`：点亮保持时间，默认 500 ms。
- `off_ms`：熄灭保持时间，默认 500 ms。
- `cycles`：循环次数，默认 0，表示一直运行，按 `Ctrl-C` 停止。

程序退出前会尝试把 LED 设为无效电平。

### Button

```bash
./build/button_read "$BUTTON_CHIP" "$BUTTON_LINE" "$BUTTON_ACTIVE_LEVEL"
```

可选参数：

```bash
./build/button_read "$BUTTON_CHIP" "$BUTTON_LINE" "$BUTTON_ACTIVE_LEVEL" 100 20
```

含义依次是：

```text
chip line active_level poll_ms max_events
```

- `poll_ms`：轮询周期，默认 100 ms。
- `max_events`：最多打印的状态变化次数，默认 0，表示一直运行，按 `Ctrl-C` 停止。

本程序只观察原始按键状态，不做消抖。一次按键出现多次输出时，应记录为抖动现象，留到 Lab 3-2 处理。

## 运行验证

| 验证项 | 预期现象 | 记录 |
| --- | --- | --- |
| 构建 | `build/blink` 和 `build/button_read` 生成 |  |
| Blink | LED 按周期亮灭，停止后回到无效电平 |  |
| Button | 按下和松开时终端打印状态变化 |  |
| 清理 | `make clean` 后 `build/` 被删除 |  |

## 常见问题

| 现象 | 可能原因 | 处理方式 |
| --- | --- | --- |
| `linux/gpio.h: No such file or directory` | 编译环境不是 Linux 目标环境，或缺少内核头文件 | 在目标板上编译，或安装目标 sysroot/kernel headers |
| `open gpiochip: Permission denied` | 当前用户没有访问 `/dev/gpiochip*` 权限 | 临时使用 `sudo`，长期方案写入板级准备文档 |
| `GPIO_GET_LINEHANDLE_IOCTL failed` | line 编号错误、line 被其他驱动占用或内核不支持该 ABI | 用 `gpioinfo` 复查 line，换未占用 GPIO，记录板级问题 |
| LED 现象相反 | 有效电平填反 | 交换 `LED_ACTIVE_LEVEL` 和 `LED_INACTIVE_LEVEL` |
| Button 没变化 | line 错误、按键接线错误、缺少上拉/下拉或按键已接入 input 子系统 | 复查 `gpioinfo`、`/dev/input` 和接线 |
