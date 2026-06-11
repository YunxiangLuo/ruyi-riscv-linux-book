# ch03 class3 代码工程：Button

## 项目目标

本工程对应 ch03 class3 和 Lab 3，只完成 GPIO 输入验证：

- `button_read`：持续读取一个 GPIO line，在按键状态变化时打印 `pressed` 或 `released`。

本程序用于观察原始按键状态。一次按键出现多次输出时，应保留日志，作为分析机械按键抖动和交互稳定性的实验证据。

本工程自带输入所需的 GPIO 封装，不依赖 `code/class2` 的输出工程。

## 前置条件

- 目标系统是 Linux，并暴露 `/dev/gpiochip*`。
- 已确认按键对应的 GPIO 参数。
- 当前用户有 GPIO 访问权限；如果没有，运行时需要加 `sudo`。
- 外接按键必须有明确上拉或下拉。

需要填入的板级参数：

```bash
BUTTON_CHIP=gpiochipX
BUTTON_LINE=Y
BUTTON_ACTIVE_LEVEL=1
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

## 运行验证

| 验证项 | 预期现象 | 记录 |
| --- | --- | --- |
| 构建 | `build/button_read` 生成 |  |
| Button | 按下和松开时终端打印状态变化 |  |
| 清理 | `make clean` 后 `build/` 被删除 |  |

## 常见问题

| 现象 | 可能原因 | 处理方式 |
| --- | --- | --- |
| `linux/gpio.h: No such file or directory` | 编译环境不是 Linux 目标环境，或缺少内核头文件 | 在目标板上编译，或安装目标 sysroot/kernel headers |
| `open gpiochip: Permission denied` | 当前用户没有访问 `/dev/gpiochip*` 权限 | 临时使用 `sudo`，长期方案写入板级准备文档 |
| Button 没变化 | line 错误、按键接线错误、缺少上拉/下拉或按键已接入 input 子系统 | 复查 `gpioinfo`、`/dev/input` 和接线 |
| 按一次键输出多次 | 机械按键抖动 | 保留日志，记录触发次数、持续时间和复现条件 |
