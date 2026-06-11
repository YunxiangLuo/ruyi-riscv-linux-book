# Class 2: QEMU/开发板启动与登录

## 本章目标

- 使用 QEMU `virt` 或开发板进入 RISC-V Linux 系统。
- 完成 SSH/SCP/串口登录路径验证。
- 理解 `ruyi device provision` 的适用边界。
- 观察 systemd 服务状态，知道部署服务是后续 Linux 应用化内容。

## 前置条件

- 已完成 `class1.md`。
- 已安装或准备 `ruyi`。
- 已明确本节使用 QEMU 还是真实开发板。
- 已阅读 `boards/k1/README.md` 和 `boards/licheepi4a/README.md` 中的板级资料、验证边界和待补实测项。

## 知识简介

本节课关注“能进入系统”的最小闭环。QEMU `virt` 适合承担低硬件依赖的入门任务，可以帮助学生先完成 Linux 启动、登录和文件传输；真实开发板用于 GPIO、UART、外设和板级配置实验。`ruyi device provision` 适合设备镜像部署，systemd 可用于观察系统服务状态，例如确认系统是否使用 systemd、服务是否处于 running/failed 状态。本节课要让学生分清 QEMU、开发板、SSH、SCP、串口、镜像部署和服务状态观察之间的关系。

## 环境准备

| 项目 | 要求 | 检查方式 |
| --- | --- | --- |
| QEMU 或开发板 | 至少一种可用 | 能看到系统启动日志 |
| 登录工具 | SSH、SCP、串口工具 | 能登录目标系统 |
| 资料依据 | `docs/course-plan.md`、RuyiSDK 官方文档、RuyiSDK Support Matrix、`boards/` 板卡说明 | 对照 ch01 三级小节 |

### 当前已找到的板级资料

| 板卡 | 可引用资料 | 当前用途 | 仍需实测 |
| --- | --- | --- | --- |
| K1 / Muse Pi Pro | RuyiSDK Support Matrix 中有 Bianbu 与 openEuler 测试记录；Bianbu 记录给出 `root` / `bianbu` 串口登录；openEuler 记录给出镜像、固件和串口登录流程 | ch01 可说明目标板镜像和登录资料来源 | 课程最终镜像、SSH/SCP/串口完整记录、`ruyi device provision` 支持情况 |
| LicheePi 4A | Sipeed wiki 给出默认账号、烧录模式、系统串口和外设线索；RuyiSDK Support Matrix 有 LicheePi4A 目录 | K1 暂不可用时的临时验证板 | 当前镜像版本、SSH/SCP/串口完整记录、GPIO/libgpiod 实测输出 |

LicheePi 4A 已找到的串口线索：系统串口是 `UART0`，使用 `U0-RX`、`U0-TX` 交叉连接并接 GND，波特率为 `115200`。默认镜像账号可先按 `debian` / `debian`、`sipeed` / `licheepi` 记录；`root` 默认未设置密码。实际课堂仍以当前镜像为准。

## 操作步骤

### 步骤 1：启动 QEMU 或开发板

按课程指定方式启动目标系统，并保存启动日志。

```bash
# 按课程环境补充具体启动命令
```

### 步骤 2：确认目标系统状态

登录后执行：

```bash
hostname
uname -a
cat /etc/os-release
whoami
pwd
ip addr
systemctl --version
systemctl status
```

如果系统不使用 systemd，记录命令失败输出即可，不在本节展开 init 系统差异。

### 步骤 3：验证 SSH 登录

```bash
ssh 用户名@目标IP
```

### 步骤 4：验证 SCP 文件传输

```bash
scp ./test.txt 用户名@目标IP:/tmp/
```

### 步骤 5：说明 `ruyi device provision` 边界

先在主机侧查询 provision 帮助信息：

```bash
ruyi --version
ruyi device --help
ruyi device provision --help
ruyi device provision
```

进入向导后记录设备列表、镜像候选和烧录方式。如果本节只是确认支持情况，不要实际刷写设备。

记录以下问题的答案：

- 当前实验是否需要刷写镜像？
- 当前设备是否在 `ruyi device provision` 向导中出现？
- 如果出现，支持哪些镜像和烧录方式？
- 如果没有出现，替代路径是什么？
- 如果只是 QEMU 用户态练习，是否需要 provision？

判断口径：

- Support Matrix 中有测试记录，说明有资料来源，但不能直接等同于当前 `ruyi device provision` 向导一定支持。
- `ruyi device provision` 适合镜像部署；SSH/SCP、GPIO、UART 等应用实验不应每次都重新 provision。
- 如果设备暂不在向导中，课程应采用官方镜像下载、`dd`、`fastboot` 或板卡厂商文档作为替代路径，并把原因写入板级记录。

## 运行验证

| 验证项 | 预期现象 | 记录 |
| --- | --- | --- |
| 系统启动 | 能进入 Linux shell |  |
| SSH 登录 | 能远程登录 |  |
| SCP 传输 | `/tmp/test.txt` 存在 |  |
| systemd 状态观察 | 能看到 systemd 版本/服务状态，或记录当前系统不支持 systemd 的原因 |  |
| provision 查询 | 能记录当前 `ruyi` 版本、向导中是否出现目标设备、是否需要实际刷写 |  |
| provision 边界 | 能说明适用、不适用场景和替代路径 |  |

## 常见问题

### SSH 连接失败

现象：无法连接目标 IP。

处理：检查目标系统是否启动、网络是否连通、用户名是否正确、SSH 服务是否开启。

### SCP 失败

现象：登录可行但文件传输失败。

处理：检查目标路径权限，优先传到 `/tmp/`。

### provision 向导中没有目标板

现象：`ruyi device provision` 的设备列表中没有 K1 / Muse Pi Pro 或当前使用的临时验证板。

处理：记录 `ruyi --version`、向导截图或文本输出；不要强行选择相近板卡。课程可改用官方镜像和板卡厂商烧录流程，并在 `boards/` 下说明替代路径。

## 本章成果

- 能启动 QEMU 或开发板系统。
- 能完成 SSH/SCP/串口至少一种登录或传输路径。
- 能说明 `ruyi device provision` 的使用边界，并能记录“支持、待确认或不支持”的依据。
- 能观察 systemd 服务状态，但不在本章展开服务部署。
