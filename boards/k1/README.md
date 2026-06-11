# K1 / Muse Pi Pro Board

本课程中的 K1 指 [Muse Pi Pro](https://www.spacemit.com/spacemit-muse-pi-pro/)。

K1 / Muse Pi Pro 是课程目标板。当前 K1 暂时无法获取时，样章和早期实验验证临时使用 LicheePi 4A；QEMU 继续作为低硬件依赖内容的辅助环境。

课程正文应优先使用“目标板”这类通用表述。涉及板卡镜像、引脚、设备路径和外设连接的差异，统一放在 `boards/` 下说明，避免把主线课程写成多板适配手册。

## 关键文件

- `board-selection.md`：K1 / Muse Pi Pro 开发板选型说明

## 已找到的资料来源

- RuyiSDK Support Matrix：`https://github.com/ruyisdk/support-matrix/tree/main/Muse_Pi_Pro`
- Bianbu 测试记录：`https://github.com/ruyisdk/support-matrix/blob/main/Muse_Pi_Pro/Bianbu/README_zh.md`
- openEuler 测试记录：`https://github.com/ruyisdk/support-matrix/blob/main/Muse_Pi_Pro/openEuler/README_zh.md`
- SpacemiT 产品页：`https://www.spacemit.com/spacemit-muse-pi-pro/`

## 当前可确认信息

- RuyiSDK Support Matrix 中已有 Muse Pi Pro 目录，并包含 Bianbu 与 openEuler 测试记录。
- Bianbu 记录对应 Bianbu-Computer UEFI v1.3，基于 Bianbu Star 2.1。
- Bianbu 记录给出默认串口登录账号：`root`，默认密码：`bianbu`。
- openEuler 记录对应 openEuler 24.03-LTS-SP1，给出 SPI NOR 固件刷写、microSD 镜像写入和串口登录记录。
- SpacemiT 产品页给出目标板规格：M1 处理器、8GB/16GB LPDDR4X、64GB/128GB eMMC、千兆以太网、USB3.0、UART TTL 调试接口、40Pin GPIO、MIPI DSI/CSI、M.2、miniPCIe 等。

## 仍需实测

- 课程最终镜像选用 Bianbu、openEuler 还是其他系统。
- `ruyi device provision` 是否支持 Muse Pi Pro，以及完整命令记录。
- SSH/SCP/串口登录记录。
- `gpioinfo`、`gpiodetect`、`gpioset --version`、`gpioget --version` 输出。
- LED、按键、PWM、ADC、I2C/SPI、OLED、传感器、摄像头和麦克风的实际设备路径。

实测结果应保存为 `boards/k1/validation-YYYY-MM-DD.md`，并保留原始命令输出、截图或照片。

建议内容：

- 开发板规格
- 串口连接方式
- 镜像烧录步骤
- GPIO/UART/I2C/SPI 引脚说明
- 常见问题
