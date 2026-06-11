# LicheePi 4A Temporary Validation Board

LicheePi 4A 是当前 K1 / Muse Pi Pro 暂时无法获取时使用的临时验证板。

它的作用是帮助样章和早期课程内容尽快落地，优先验证 Linux 用户态开发流程、构建运行、远程登录、文件日志、线程、网络和部分板级外设实验。它不是课程默认目标板，也不改变课程最终以 K1 / Muse Pi Pro 为目标板的口径。

## 使用边界

| 内容 | 是否适合临时验证 | 说明 |
| --- | --- | --- |
| ch01 环境、登录、文件传输 | 适合 | 用于验证进入系统和部署流程 |
| ch02 Hello World、ELF、Makefile | 适合 | 用于验证交叉编译或本机编译流程 |
| ch03 GPIO、LED、按键 | 视资料而定 | 需要确认 GPIO 编号、LED/按键连接方式和权限 |
| ch04 串口调试 | 适合 | 需要确认串口设备名、波特率和登录方式 |
| ch05 PWM、ADC、OLED、传感器 | 视资料而定 | 需要确认实际外设和设备路径 |
| ch06 文件、配置、日志 | 适合 | 与具体板卡关系较弱 |
| ch07 pthread、多任务协同 | 适合 | 与具体板卡关系较弱 |
| ch08 联网信息采集终端 | 适合做最小闭环 | 传感器不可用时可使用模拟数据 |
| ch09 Camera/OpenCV | 视摄像头而定 | 可先验证软件流程 |
| ch10 TinyML KWS | 视音频输入和 LED 而定 | 可先使用预录音频和终端输出 |
| ch11 Buildroot/Device Tree | 仅作参考 | 最终应回到 K1 / Muse Pi Pro |

## 需要补齐

- 系统镜像来源和烧录步骤。
- 默认用户名、密码、网络配置和 SSH 登录方式。
- 串口设备名、波特率、接线方式和权限说明。
- GPIO、UART、PWM、ADC、I2C/SPI 设备路径。
- LED、按键、OLED、传感器、麦克风和摄像头的实际连接方式。
- 已完成验证的命令记录、截图或日志。

## 已找到的资料来源

- Sipeed 开箱上手：`https://wiki.sipeed.com/hardware/zh/lichee/th1520/lpi4a/2_unbox.html`
- Sipeed 镜像烧录：`https://wiki.sipeed.com/hardware/zh/lichee/th1520/lpi4a/4_burn_image.html`
- Sipeed 外设使用：`https://wiki.sipeed.com/hardware/zh/lichee/th1520/lpi4a/6_peripheral.html`
- RuyiSDK Support Matrix：`https://github.com/ruyisdk/support-matrix/tree/main/LicheePi4A`

## 当前可确认信息

- 默认镜像账号包含 `debian` / `debian` 和 `sipeed` / `licheepi`，`root` 默认没有设置密码。
- 系统串口是 `UART0`，引出为 `U0-RX`、`U0-TX`，需要交叉连接并接 GND。
- 系统串口波特率为 `115200`。
- 烧录模式入口：按住板上 `BOOT` 键，再插入 USB-C 线缆上电。
- Linux 主机可用 `lsusb` 识别 `ID 2345:7654 T-HEAD USB download gadget`。
- 外设页给出 sysfs GPIO、libgpiod、UART、I2C、SPI、USB 摄像头和 CSI 摄像头的使用线索。

## 仍需实测

- 课程实际使用镜像的名称、版本、下载路径和烧录日志。
- SSH、SCP、串口登录记录。
- `libgpiod` 命令版本和参数口径。
- ch03 class1 的 LED/Button GPIO chip、line 和 active level。
- 板载 LED 是否走 `/sys/class/leds/`，板载按键是否走 `/dev/input/event*`。

实测结果应保存为 `boards/licheepi4a/validation-YYYY-MM-DD.md`，并保留原始命令输出、截图或照片。
