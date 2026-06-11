# Ruyi RISC-V Linux Book

面向 RISC-V Linux 开发板与 QEMU 环境的课程仓库。课程以 RuyiSDK 为开发入口，从统一环境、交叉编译、板级外设、Linux 应用开发逐步推进到联网终端、视觉处理、TinyML 边缘推理和系统扩展。

## 课程主线

本仓库按照 `docs/course-outline.md` 组织内容，分为五个一级模块：

| 模块 | 章节范围 | 学习重点 |
| --- | --- | --- |
| 开发环境篇 | ch01-ch02 | 建立统一环境、掌握交叉编译和最小工程能力 |
| 开发板基础篇 | ch03-ch05 | 完成 GPIO、UART、常用外设和人机交互 |
| Linux 应用开发篇 | ch06-ch07 | 引入文件、配置、日志、线程和任务协同 |
| 综合项目篇 | ch08 | 汇总前置能力形成联网信息采集终端 |
| 扩展方向篇 | ch09-ch11 | 扩展 Camera/OpenCV、TinyML 边缘推理、Buildroot 和 Device Tree |

## 目录结构

```text
ruyi-riscv-linux-book/
├── README.md          # 项目说明
├── boards/            # 开发板说明和课程目标板边界
├── chapters/          # 分章节讲义、实验指导书和代码工程
├── docs/              # 课程大纲、课程计划、制作规范、模板和项目定义
└── slides/            # 授课 PPT
```

## 章节目录

课程当前包含 37 个 class 级小节。ch01-ch07 形成主线能力，ch08 汇总为联网信息采集终端，ch09-ch11 作为扩展项目方向。

| 小节名 | 章节 | 模块 | 主要实验/成果 |
| --- | --- | --- | --- |
| 1.1 RuyiSDK 支持矩阵、目标开发板与基础命令 | [ch01_ruyisdk_qemu](chapters/ch01_ruyisdk_qemu) | 开发环境篇 | 查询支持矩阵、记录目标板配置、安装 `ruyi` |
| 1.2 开发板配置、镜像烧录与启动登录 | [ch01_ruyisdk_qemu](chapters/ch01_ruyisdk_qemu) | 开发环境篇 | 查阅开发板说明、烧录或启动镜像、SSH/SCP/串口登录 |
| 1.3 文件传输、provision 与 systemd 边界 | [ch01_ruyisdk_qemu](chapters/ch01_ruyisdk_qemu) | 开发环境篇 | 文件传输、provision 查询、systemd 服务状态观察 |
| 2.1 Git 基础与工程工作区 | [ch02_toolchain_template](chapters/ch02_toolchain_template) | 开发环境篇 | Git 基础操作、实验记录管理 |
| 2.2 工具链、Hello World、ELF 观察与 benchmark 边界 | [ch02_toolchain_template](chapters/ch02_toolchain_template) | 开发环境篇 | Hello World、`file`/`readelf`/`ldd`、benchmark 选做说明 |
| 2.3 Makefile 控制编译与工程模板 | [ch02_toolchain_template](chapters/ch02_toolchain_template) | 开发环境篇 | Makefile 控制编译、运行、清理和调试记录 |
| 3.1 面包板、跳线与基础接线 | [ch03_gpio_interaction](chapters/ch03_gpio_interaction) | 开发板基础篇 | 面包板连通性、LED/电阻/按键方向和接线记录检查 |
| 3.2 GPIO 输出控制 LED | [ch03_gpio_interaction](chapters/ch03_gpio_interaction) | 开发板基础篇 | Blink |
| 3.3 GPIO 输入读取按键 | [ch03_gpio_interaction](chapters/ch03_gpio_interaction) | 开发板基础篇 | Button |
| 3.4 按键消抖、状态机与可复用交互模块 | [ch03_gpio_interaction](chapters/ch03_gpio_interaction) | 开发板基础篇 | 按键控制 LED、调试输入抖动和状态切换 |
| 4.1 串口连接、TTY 工具与通信参数 | [ch04_uart_debug](chapters/ch04_uart_debug) | 开发板基础篇 | 串口连接与参数记录 |
| 4.2 UART Echo 与收发缓冲 | [ch04_uart_debug](chapters/ch04_uart_debug) | 开发板基础篇 | UART Echo |
| 4.3 串口日志、命令回显与调试入口 | [ch04_uart_debug](chapters/ch04_uart_debug) | 开发板基础篇 | 串口日志、命令回显 |
| 5.1 PWM 输出与呼吸灯 | [ch05_peripherals_hmi](chapters/ch05_peripherals_hmi) | 开发板基础篇 | 呼吸灯 |
| 5.2 ADC 采样与模拟量观察 | [ch05_peripherals_hmi](chapters/ch05_peripherals_hmi) | 开发板基础篇 | ADC 采样 |
| 5.3 I2C/SPI 外设识别与 OLED 显示 | [ch05_peripherals_hmi](chapters/ch05_peripherals_hmi) | 开发板基础篇 | 识别外设、OLED 显示数值 |
| 5.4 DHT22 采集与本地可视化终端 | [ch05_peripherals_hmi](chapters/ch05_peripherals_hmi) | 开发板基础篇 | DHT22 采集、本地采集显示闭环 |
| 6.1 文件 I/O 与数据落盘 | [ch06_file_config_log](chapters/ch06_file_config_log) | Linux 应用开发篇 | 传感数据落盘 |
| 6.2 配置文件与参数化启动 | [ch06_file_config_log](chapters/ch06_file_config_log) | Linux 应用开发篇 | 配置化启动 |
| 6.3 日志、错误处理与可维护目录结构 | [ch06_file_config_log](chapters/ch06_file_config_log) | Linux 应用开发篇 | 日志记录、构造错误并定位、整理目录结构 |
| 7.1 pthread 任务拆分 | [ch07_thread_tasks](chapters/ch07_thread_tasks) | Linux 应用开发篇 | 双线程采样与显示 |
| 7.2 互斥同步与共享数据保护 | [ch07_thread_tasks](chapters/ch07_thread_tasks) | Linux 应用开发篇 | 互斥保护共享数据 |
| 7.3 生产者-消费者模型与线程调试 | [ch07_thread_tasks](chapters/ch07_thread_tasks) | Linux 应用开发篇 | 生产者-消费者队列、线程同步调试 |
| 8.1 组装前序模块与项目骨架 | [ch08_project_network_terminal](chapters/ch08_project_network_terminal) | 综合项目篇 | 建立项目骨架 |
| 8.2 采集和显示任务集成 | [ch08_project_network_terminal](chapters/ch08_project_network_terminal) | 综合项目篇 | 多线程采集和显示框架 |
| 8.3 日志、配置与运行状态管理 | [ch08_project_network_terminal](chapters/ch08_project_network_terminal) | 综合项目篇 | 项目日志、配置和状态记录 |
| 8.4 HTTP 状态页发布与综合验收 | [ch08_project_network_terminal](chapters/ch08_project_network_terminal) | 综合项目篇 | 联网环境监测终端 |
| 9.1 摄像头接入与设备检查 | [ch09_camera_opencv](chapters/ch09_camera_opencv) | 扩展方向篇 | 摄像头可用性检查 |
| 9.2 图像采集与保存 | [ch09_camera_opencv](chapters/ch09_camera_opencv) | 扩展方向篇 | 图像采集 |
| 9.3 OpenCV 编译链接与基础图像处理 | [ch09_camera_opencv](chapters/ch09_camera_opencv) | 扩展方向篇 | 灰度/边缘处理 |
| 10.1 KWS 控灯最小流程 | [ch10_tinyml_kws](chapters/ch10_tinyml_kws) | 扩展方向篇 | KWS 控灯最小流程 |
| 10.2 语音样本准备与预处理 | [ch10_tinyml_kws](chapters/ch10_tinyml_kws) | 扩展方向篇 | 语音样本准备 |
| 10.3 MFCC/int8 推理部署 | [ch10_tinyml_kws](chapters/ch10_tinyml_kws) | 扩展方向篇 | MFCC 与 `weights.h` 导出、板上推理 |
| 10.4 关键词控灯集成与验收 | [ch10_tinyml_kws](chapters/ch10_tinyml_kws) | 扩展方向篇 | 语音关键词识别控灯 |
| 11.1 Buildroot 产物识别 | [ch11_buildroot_devicetree](chapters/ch11_buildroot_devicetree) | 扩展方向篇 | 查看 Buildroot 输出 |
| 11.2 最小系统构建 | [ch11_buildroot_devicetree](chapters/ch11_buildroot_devicetree) | 扩展方向篇 | Buildroot 最小镜像 |
| 11.3 Device Tree、pinmux 与外设使能入门 | [ch11_buildroot_devicetree](chapters/ch11_buildroot_devicetree) | 扩展方向篇 | DT 修改使某外设可用 |

## 样章入口

- [ch03 class2：GPIO 输出控制 LED](chapters/ch03_gpio_interaction/class2.md)
- [ch03 lab2：GPIO 输出控制 LED 实验](chapters/ch03_gpio_interaction/lab/lab2.md)
- [ch03 class2 授课 PPT](slides/ch03_gpio_class2_gpio_output_led.pptx)

## 推荐学习路径

1. 阅读 [课程大纲](docs/course-outline.md)。
2. 阅读 [课程制作规范 V1.0](docs/course-production-spec-v1.md)，确认讲义、PPT、实验和代码标准。
3. 阅读 [课程计划表](docs/course-plan.md)，确认三级小节、交付物和运行验证。
4. 阅读 [K1 / Muse Pi Pro 开发板选型说明](boards/k1/board-selection.md)，确认课程目标板、LicheePi 4A 临时验证板和 QEMU 辅助环境的边界。
5. 使用 [课程文档模板](docs/templates/course-document-template.md) 编写每章 `class*.md`。
6. 使用 [PPT 模板](docs/templates/ppt-template.md) 设计每节课 PPT。
7. 使用 [实验指导书模板](docs/templates/lab-template.md) 编写每章 `lab/lab*.md`。
8. 从 `chapters/ch01_ruyisdk_qemu` 开始，完成环境准备、系统启动和登录。
9. 继续完成 `chapters/ch02_toolchain_template`，建立工具链、Makefile 和最小工程能力。
10. 进入 `chapters/ch03_gpio_interaction`，完成面包板接线、GPIO Blink、Button 和状态机交互实验。
11. 按章节顺序推进到 ch08，形成联网信息采集终端主线作品。
12. 根据兴趣选择 ch09 到 ch11 的扩展方向。
