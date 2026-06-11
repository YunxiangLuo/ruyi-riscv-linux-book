# Class 1: RuyiSDK 支持矩阵、目标开发板与基础命令

## 本章目标

- 了解 RuyiSDK 在 RISC-V Linux 开发中的作用。
- 完成 `ruyi` 的安装、版本检查和软件包缓存更新。
- 能根据 Support Matrix 判断目标设备或 QEMU 环境是否适合课程实验。
- 能引用 `boards/` 中的目标开发板说明，记录本课程使用开发板的基本配置。

## 前置条件

- 已能使用一台 Linux、macOS 或 Windows + WSL 主机。
- 已能打开终端并执行基本命令。
- 主机可以访问课程所需的软件源或镜像源。

## 知识简介

RuyiSDK 是面向 RISC-V 开发者的统一开发入口，提供包管理器、工具链、模拟器、系统镜像、源码包和设备支持信息。对于课程学习者来说，它的价值不是替代所有 Linux 知识，而是把“找工具、装工具、启动环境、查看板卡支持状态”这些容易分散精力的准备工作收束到一个入口。第一节课的目标是建立最小环境闭环：知道 RuyiSDK 由什么组成，能安装 `ruyi`，能更新本地软件包缓存，并能通过版本命令和 Support Matrix 判断当前环境是否可继续后续实验。

## 环境准备

| 项目 | 要求 | 检查方式 |
| --- | --- | --- |
| 主机环境 | Linux、macOS 或 Windows + WSL | 能打开终端 |
| 网络环境 | 能访问 RuyiSDK 软件源或镜像源 | 能执行 `ruyi update` |
| 工具依赖 | `ruyi` | 能执行 `ruyi version` |
| 资料依据 | RuyiSDK 官方文档、Support Matrix、`boards/` 板卡说明 | 阅读 RuyiSDK 组成、常用命令和目标板配置 |

## 操作步骤

### 步骤 1：确认当前主机环境

```bash
uname -a
pwd
```

记录当前系统类型和工作目录。如果使用 WSL，也要记录发行版名称。

### 步骤 2：安装或准备 `ruyi`

按照当前系统适用方式安装 `ruyi`。安装完成后执行：

```bash
ruyi version
```

如果命令找不到，优先检查 `PATH` 是否包含 `ruyi` 所在目录。

### 步骤 3：刷新软件包缓存

```bash
ruyi update
```

该命令用于获取软件包索引。首次执行可能需要较长时间。

### 步骤 4：查看 RuyiSDK 可用内容

```bash
ruyi list --name-contains qemu
ruyi list --name-contains gnu
ruyi list profiles
```

观察输出中是否包含模拟器、工具链和预置环境配置。

### 步骤 5：阅读 Support Matrix

结合课程目标，记录以下信息：

- 是否使用 QEMU `virt`。
- 是否使用 K1 / Muse Pi Pro 或其他真实 RISC-V Linux 开发板。
- 目标环境是否已有镜像、工具链和文档支持。

### 步骤 6：记录本课程目标开发板

阅读 `boards/README.md`、`boards/k1/README.md` 和 `boards/k1/board-selection.md`，整理本课程目标开发板的基本信息：

- 开发板名称、SoC、CPU 架构和内存配置。
- 课程使用它承担哪些实验。
- 哪些实验可以先用 QEMU 辅助完成。
- 板卡资料中已经给出的镜像、登录和烧录入口。

## 运行验证

| 验证项 | 预期现象 | 记录 |
| --- | --- | --- |
| `ruyi version` | 输出版本信息 |  |
| `ruyi update` | 能完成缓存更新或给出可解释错误 |  |
| `ruyi list profiles` | 能列出可用配置 |  |
| Support Matrix 阅读 | 能说明目标环境支持状态 |  |
| 目标板资料引用 | 能引用 `boards/` 中的目标板说明并记录关键配置 |  |

## 常见问题

### `ruyi: command not found`

现象：终端无法识别 `ruyi`。

原因：`ruyi` 未安装，或安装路径没有加入 `PATH`。

处理：确认安装路径，并重新打开终端或更新 `PATH`。

### `ruyi update` 失败

现象：更新软件包缓存失败。

原因：网络不可达、默认源访问不稳定，或代理设置不正确。

处理：检查网络；必要时按照 RuyiSDK 资料切换镜像源。

### 不确定该用 QEMU 还是开发板

现象：不知道后续实验该在哪个环境运行。

处理：前两章优先使用 QEMU 或开发板都可以；GPIO、PWM、ADC、OLED、DHT22 等真实外设实验应回到开发板。

## 本章成果

- 完成 `ruyi` 安装与版本检查。
- 完成软件包缓存更新。
- 能解释 RuyiSDK 的基本组成。
- 能说明目标 QEMU 或开发板环境的支持状态。
- 能记录本课程目标开发板的资料来源和基本配置。
