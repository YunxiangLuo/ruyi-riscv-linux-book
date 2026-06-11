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

| 章节 | 主题 | 对应模块 |
| --- | --- | --- |
| `ch01_ruyisdk_qemu` | RuyiSDK、开发板与 QEMU 上手 | 开发环境篇 |
| `ch02_toolchain_template` | 工具链、工程模板与第一个程序 | 开发环境篇 |
| `ch03_gpio_interaction` | GPIO 输入输出与交互 | 开发板基础篇 |
| `ch04_uart_debug` | 串口通信与板级调试 | 开发板基础篇 |
| `ch05_peripherals_hmi` | 常用外设与人机交互 | 开发板基础篇 |
| `ch06_file_config_log` | 文件、配置与日志 | Linux 应用开发篇 |
| `ch07_thread_tasks` | 线程与任务协同 | Linux 应用开发篇 |
| `ch08_project_network_terminal` | 综合项目一 联网信息采集终端 | 综合项目篇 |
| `ch09_camera_opencv` | Camera 与 OpenCV 应用扩展 | 扩展方向篇 |
| `ch10_tinyml_kws` | TinyML 语音关键词识别控灯 | 扩展方向篇 |
| `ch11_buildroot_devicetree` | Buildroot、Device Tree 与板级扩展 | 扩展方向篇 |

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
10. 进入 `chapters/ch03_gpio_interaction`，完成 GPIO Blink、Button 和状态机交互实验。
11. 按章节顺序推进到 ch08，形成联网信息采集终端主线作品。
12. 根据兴趣选择 ch09 到 ch11 的扩展方向。
