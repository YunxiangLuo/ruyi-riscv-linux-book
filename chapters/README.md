# Chapters

本目录按课程大纲组织 11 个二级章节。每章 README 包含本章目标、三级知识点、配套实验和章节成果。

## 章节列表

| 章节目录 | 二级章节 | 本章目标 |
| --- | --- | --- |
| `ch01_ruyisdk_qemu` | RuyiSDK、开发板与 QEMU 上手 | 建立统一开发环境和最小运行闭环 |
| `ch02_toolchain_template` | 工具链、工程模板与第一个程序 | 说明程序如何编译、链接、运行 |
| `ch03_gpio_interaction` | GPIO 输入输出与交互 | 形成最早的板卡控制能力 |
| `ch04_uart_debug` | 串口通信与板级调试 | 建立板卡调试与交互通道 |
| `ch05_peripherals_hmi` | 常用外设与人机交互 | 完成“采集—显示”局部闭环 |
| `ch06_file_config_log` | 文件、配置与日志 | 使程序具备持久化与可维护性 |
| `ch07_thread_tasks` | 线程与任务协同 | 让程序具备稳定运行能力 |
| `ch08_project_network_terminal` | 综合项目一 联网信息采集终端 | 汇总 ch01-ch07 成果形成课程作品一 |
| `ch09_camera_opencv` | Camera 与 OpenCV 应用扩展 | 引入视觉输入与基础图像处理 |
| `ch10_tinyml_kws` | TinyML 语音关键词识别控灯 | 形成课程作品二或选修项目 |
| `ch11_buildroot_devicetree` | Buildroot、Device Tree 与板级扩展 | 建立系统认识，不改变全书主线 |

## 章节内部结构

```text
chXX_topic/
├── README.md
├── class1.md      # 第 1 次课课程文档
├── class2.md      # 第 2 次课课程文档
├── class3.md
├── class4.md      # 内容较多章节可选
├── lab/
│   ├── lab1.md    # 第 1 个实验指导书
│   ├── lab2.md    # 第 2 个实验指导书
│   ├── lab3.md
│   └── lab4.md    # 内容较多章节可选
└── assets/        # 图片、截图和图表
```

## 写作规则

- 每个 `class*.md` 应包含目标、前置条件、知识简介、环境准备、操作步骤、运行验证、常见问题和本章成果。
- 每个 `lab/lab*.md` 应包含实验目标、实验任务、运行验证和提交要求。
- 图片、截图和图表放入本章 `assets/`，命名应能体现章节、实验和用途。
- 章节内容必须以 `docs/course-outline.md` 为基准，并参考 `docs/course-plan.md` 的三级小节、交付物和运行验证。
- ch08 默认实现 HTTP 状态页；MQTT/Socket 只作为拓展。
- ch10 默认实现 TinyML KWS 语音关键词识别控灯；Face Detection、YOLO 只作为备选拓展。
