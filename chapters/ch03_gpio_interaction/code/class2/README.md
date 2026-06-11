# ch03 class2 代码工程：Blink

## 项目目标

本工程对应 ch03 class2 和 Lab 2，只完成 GPIO 输出验证：

- `blink`：持续占用一个 GPIO line，让 LED 按固定周期亮灭。

代码使用 Linux GPIO character device ioctl 访问 `/dev/gpiochip*`，不依赖 `libgpiod-dev` 的 C API 版本。课堂上仍建议先用 `gpiodetect`、`gpioinfo` 和 `gpioset` 确认板级参数和命令口径。

## 前置条件

- 目标系统是 Linux，并暴露 `/dev/gpiochip*`。
- 已确认 LED 对应的 GPIO 参数。
- 当前用户有 GPIO 访问权限；如果没有，运行时需要加 `sudo`。
- 外接 LED 必须有限流电阻，并确认电压等级和接线方向。

需要填入的板级参数：

```bash
LED_CHIP=gpiochipX
LED_LINE=Y
LED_ACTIVE_LEVEL=1
LED_INACTIVE_LEVEL=0
```

## 构建方法

```bash
make build
```

交叉编译时可以覆盖编译器：

```bash
make build CC=riscv64-linux-gnu-gcc
```

## 运行方法

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

程序退出前会尝试把 LED 设为无效电平。

## 运行验证

| 验证项 | 预期现象 | 记录 |
| --- | --- | --- |
| 构建 | `build/blink` 生成 |  |
| Blink | LED 按周期亮灭，停止后回到无效电平 |  |
| 清理 | `make clean` 后 `build/` 被删除 |  |

## 常见问题

| 现象 | 可能原因 | 处理方式 |
| --- | --- | --- |
| `linux/gpio.h: No such file or directory` | 编译环境不是 Linux 目标环境，或缺少内核头文件 | 在目标板上编译，或安装目标 sysroot/kernel headers |
| `open gpiochip: Permission denied` | 当前用户没有访问 `/dev/gpiochip*` 权限 | 临时使用 `sudo`，长期方案写入板级准备文档 |
| `GPIO_GET_LINEHANDLE_IOCTL failed` | line 编号错误、line 被其他驱动占用或内核不支持该 ABI | 用 `gpioinfo` 复查 line，换未占用 GPIO，记录板级问题 |
| LED 现象相反 | 有效电平填反 | 交换 `LED_ACTIVE_LEVEL` 和 `LED_INACTIVE_LEVEL` |
