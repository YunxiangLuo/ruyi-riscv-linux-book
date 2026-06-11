# Class 3: 文件传输、provision 与 systemd 边界

## 本章目标

- 使用 SCP 或等效方式完成主机与目标系统文件传输。
- 查询 `ruyi device provision` 支持状态并说明适用边界。
- 观察 systemd 服务状态，知道服务部署属于后续 Linux 应用化内容。

## 前置条件

- 已完成 ch01 class1 和 class2。
- 能登录 QEMU 或真实开发板 Linux shell。

## 知识简介

进入系统以后，课程需要建立“能传文件、能查询部署能力、能观察服务状态”的基础能力。文件传输服务于代码运行和日志回收；`ruyi device provision` 服务于镜像部署；systemd 用于观察系统服务状态，为后续应用部署建立基本概念。

## 环境准备

| 项目 | 要求 | 检查方式 |
| --- | --- | --- |
| 登录路径 | SSH 或串口可用 | 能进入目标 shell |
| 文件传输 | SCP 或替代路径可用 | 能把测试文件放到 `/tmp/` |
| systemd | 目标系统可能支持 | `systemctl --version` |

## 操作步骤

### 步骤 1：传输测试文件

```bash
scp ./test.txt 用户名@目标IP:/tmp/
```

### 步骤 2：查询 provision 边界

```bash
ruyi device provision --help
ruyi device provision
```

### 步骤 3：观察服务状态

```bash
systemctl --version
systemctl status
```

如果目标系统不使用 systemd，记录错误输出即可。

## 运行验证

| 验证项 | 预期现象 | 记录 |
| --- | --- | --- |
| 文件传输 | `/tmp/test.txt` 存在 |  |
| provision 查询 | 能说明是否需要刷写和是否支持目标板 |  |
| systemd 状态 | 能看到服务状态或记录不支持原因 |  |

## 常见问题

### `systemctl` 不存在

现象：命令找不到或提示系统未使用 systemd。

处理：记录输出，不在本章展开 init 系统差异。

## 本章成果

- 能传输文件。
- 能说明 provision 使用边界。
- 能观察 systemd 状态但不提前展开部署。
