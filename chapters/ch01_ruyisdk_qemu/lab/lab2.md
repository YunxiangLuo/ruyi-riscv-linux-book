# Lab 2: 开发板配置、镜像烧录与启动登录

## 实验目标

- 启动 QEMU `virt` 或真实开发板。
- 完成 SSH/SCP/串口登录路径验证。
- 观察目标系统基础状态和 systemd 服务状态。

## 实验环境

| 项目 | 要求 |
| --- | --- |
| 目标环境 | QEMU `virt` 或 RISC-V Linux 开发板 |
| 登录工具 | SSH、SCP 或串口工具 |
| 软件依赖 | 按课程环境准备 |

## 实验任务

1. 启动目标系统。
2. 登录目标系统。
3. 执行 `hostname`、`uname -a`、`cat /etc/os-release`、`ip addr`、`pwd`，记录目标系统状态。
4. 向目标系统传输一个测试文件。
5. 执行 `systemctl --version` 和 `systemctl status`，如果当前系统不使用 systemd，则记录错误输出并说明 init 系统差异。
6. 执行 `ruyi device provision --help` 或在主机侧查询 provision 向导，记录可用的镜像部署入口。
7. 记录启动日志和登录方式。

## 运行验证

| 验证项 | 预期现象 | 是否通过 |
| --- | --- | --- |
| 系统启动 | 能进入 Linux shell |  |
| 远程登录 | SSH 或串口登录成功 |  |
| 文件传输 | 测试文件出现在目标系统 |  |
| 目标系统状态 | 能记录系统版本、网络地址和当前目录 |  |
| systemd 状态 | 能看到 systemd 版本或记录当前系统不支持 systemd 的原因 |  |
| provision 边界 | 能说明本节是否需要实际刷写设备 |  |

## 提交要求

- 启动日志摘要。
- 登录命令和结果。
- 文件传输命令和结果。
- 目标系统基础命令输出。
- systemd/provision 查询记录。
