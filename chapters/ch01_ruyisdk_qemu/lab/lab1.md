# Lab 1: RuyiSDK 支持矩阵、目标开发板与基础命令

## 实验目标

- 安装或准备 `ruyi`。
- 完成版本检查和软件包缓存更新。
- 练习最小 Linux 命令记录，为后续实验建立命令行基础。
- 查阅并记录本课程目标开发板的资料来源和基本配置。

## 实验环境

| 项目 | 要求 |
| --- | --- |
| 主机环境 | Linux、macOS 或 Windows + WSL |
| 网络环境 | 可访问 RuyiSDK 软件源或镜像源 |
| 软件依赖 | `ruyi` |

## 实验任务

1. 执行 `pwd`、`ls`、`uname -a`、`whoami`、`which ruyi`，记录当前主机环境。
2. 执行 `ruyi version`。
3. 执行 `ruyi update`。
4. 执行 `ruyi list profiles`。
5. 使用 `mkdir`、`cd`、`touch`、`cat` 创建并查看一个实验记录文件。
6. 阅读 `boards/README.md`、`boards/k1/README.md` 和 `boards/k1/board-selection.md`。
7. 记录目标开发板名称、SoC、CPU 架构、内存配置、课程用途和可引用资料。
8. 记录输出结果和遇到的问题。

## 运行验证

| 验证项 | 预期现象 | 是否通过 |
| --- | --- | --- |
| `ruyi version` | 输出版本信息 |  |
| `ruyi update` | 完成缓存更新 |  |
| `ruyi list profiles` | 列出预置配置 |  |
| Linux 基础命令 | 能说明当前目录、当前用户、系统信息和记录文件内容 |  |
| 目标板资料 | 能引用 `boards/` 中的说明并记录目标板基本配置 |  |

## 提交要求

- 命令输出记录。
- 基础 Linux 命令记录。
- 目标开发板资料引用和基本配置记录。
- 遇到问题时的错误信息。
- 对当前环境是否可进入下一节的判断。
