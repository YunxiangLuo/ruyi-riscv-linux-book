# ch03 class4 GPIO 状态机示例

本目录提供按键消抖和 LED 模式切换示例。程序通过 GPIO chardev 读取按键 line，并控制 LED line 在三种模式之间切换：

- `off`：LED 关闭。
- `on`：LED 常亮。
- `blink`：LED 周期闪烁。

## 构建

```bash
make build
```

## 运行

方式一：直接传参。

```bash
./build/gpio_state_demo <led_chip> <led_line> <led_active> <led_inactive> <button_chip> <button_line> <button_active> [poll_ms] [debounce_ms] [max_events]
```

方式二：使用环境变量。

```bash
export LED_CHIP=gpiochip0
export LED_LINE=12
export LED_ACTIVE_LEVEL=1
export LED_INACTIVE_LEVEL=0
export BUTTON_CHIP=gpiochip0
export BUTTON_LINE=13
export BUTTON_ACTIVE_LEVEL=1
make run
```

## 记录要求

- 记录按键 raw 值和稳定事件。
- 记录 `off -> on -> blink -> off` 的状态转移。
- 修改 `debounce_ms` 后比较误触发情况。
