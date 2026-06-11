# K1 / Muse Pi Pro 开发板选型说明

## 结论

本课程中的 K1 指 [Muse Pi Pro](https://www.spacemit.com/spacemit-muse-pi-pro/)。课程目标板选择 K1 / Muse Pi Pro。QEMU 作为辅助环境，仅用于低硬件依赖的入门和 Linux 应用实验。

当前 K1 暂时无法获取时，样章和早期实验验证临时使用 LicheePi 4A。LicheePi 4A 是阶段性验证板，不改变课程目标板选择，也不把课程主线改成多板适配路线。

## 选型原则

- 优先选择最容易完成课程的板子。
- 优先选择教师熟悉、资料可控、调试路径清晰的板子。
- 不为了低成本牺牲教学稳定性。
- 不同时维护多块板卡主线；临时验证板只承担验证任务。
- 能支撑 GPIO、UART、常用外设、Camera、AI 和系统扩展。

## 为什么选择 K1 / Muse Pi Pro

| 维度 | 说明 |
| --- | --- |
| 教学稳定性 | 作为唯一主线板卡，减少适配复杂度 |
| AI 扩展 | 适合承接 TinyML 边缘推理、视觉 AI 和后续大模型方向 |
| 成长曲线 | 可从 GPIO 逐步推进到多线程、联网、AI、系统扩展 |
| 课程包 | 可与传感器、OLED、摄像头和教材组成教学套件 |
| 后续空间 | 可拓展 Vector、AI、本地大模型或智能家居控制 |

## 默认实验包建议

| 类别 | 建议器件 |
| --- | --- |
| 主板 | K1 / Muse Pi Pro |
| 基础连接 | 电源、数据线、串口线、网线 |
| GPIO | LED、按键、面包板、杜邦线 |
| 常用外设 | PWM LED、ADC 输入模块、OLED |
| 传感器 | DHT22 或同类温湿度传感器 |
| 视觉 | USB 或板卡适配摄像头 |
| AI | 板卡官方稳定轻量 AI 示例所需资源 |

## 当前验证策略

| 环境 | 定位 | 使用边界 |
| --- | --- | --- |
| K1 / Muse Pi Pro | 课程目标板 | 最终课程包、真实外设、Camera、AI、系统扩展和板级配置 |
| LicheePi 4A | 临时验证板 | K1 到位前验证样章、Linux 用户态流程、部分 GPIO/UART/文件/线程/网络实验 |
| QEMU `virt` | 辅助环境 | ch01、ch02、文件、线程等低硬件依赖内容的前置验证 |

板级差异处理原则：

- 课程正文尽量描述通用 Linux 应用开发流程。
- 镜像烧录、登录账号、串口设备、GPIO 编号、PWM/ADC/I2C/SPI 路径等差异放入 `boards/`。
- 样章验证结果需要标注实际运行环境，例如 `LicheePi 4A 临时验证` 或 `QEMU virt`。
- K1 到位后，必须补齐 K1 / Muse Pi Pro 的最终验证记录。

## QEMU、临时验证板与目标板分工

| 内容 | 默认环境 |
| --- | --- |
| RuyiSDK 安装 | 主机 |
| QEMU `virt` 启动 | QEMU |
| Hello World、ELF、Makefile | QEMU、LicheePi 4A 或 K1 / Muse Pi Pro |
| 文件、配置、日志 | QEMU、LicheePi 4A 或 K1 / Muse Pi Pro |
| 线程与任务协同 | QEMU、LicheePi 4A 或 K1 / Muse Pi Pro |
| GPIO、UART、PWM、ADC、OLED、DHT22 | LicheePi 4A 临时验证，最终回到 K1 / Muse Pi Pro |
| Camera/OpenCV | LicheePi 4A 可先验证流程，最终回到 K1 / Muse Pi Pro |
| TinyML | LicheePi 4A 可先验证 KWS 流程，最终回到 K1 / Muse Pi Pro |
| Buildroot/Device Tree 验证 | QEMU + K1 / Muse Pi Pro，LicheePi 4A 只作阶段参考 |

## 不采用多板主线的原因

暂不把 Duo、Duo256、K230、BPI-F3 等开发板同时纳入主线。LicheePi 4A 只作为当前阶段临时验证板，不进入课程默认板卡主线。

原因：

- 会增加课件、代码、实验和答疑成本。
- 会导致学生运行结果不一致。
- 会拖慢样章生产和后续课程落地。
- 当前没有甲方要求必须支持多板。

后续如需支持多板，应作为附录或拓展适配，不影响 K1 / Muse Pi Pro 主线。

## 待补充信息

- K1 / Muse Pi Pro 具体规格、系统镜像和 RuyiSDK 支持状态。
- K1 / Muse Pi Pro 镜像烧录步骤。
- K1 / Muse Pi Pro 串口连接方式。
- K1 / Muse Pi Pro GPIO/UART/I2C/SPI/PWM/ADC 引脚表。
- K1 / Muse Pi Pro 摄像头和 AI 示例适配说明。
- LicheePi 4A 临时验证环境记录，包括镜像、登录方式、串口、网络、GPIO 和可用外设。
