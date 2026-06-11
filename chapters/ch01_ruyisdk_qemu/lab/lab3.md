# Lab 3: 文件传输、provision 与 systemd 边界

## 实验目标

- 完成一次主机到目标系统的文件传输。
- 查询 `ruyi device provision` 的支持边界。
- 记录 systemd 服务状态或不支持原因。

## 实验环境

| 项目 | 要求 |
| --- | --- |
| 主机环境 | 已安装 `ruyi` |
| 目标环境 | QEMU 或开发板 Linux |
| 软件依赖 | SSH/SCP 或串口替代路径 |

## 实验任务

### 任务 1：传输文件

创建 `test.txt` 并传输到目标系统 `/tmp/`。

### 任务 2：查询 provision

记录 `ruyi device provision --help` 和向导中目标设备出现情况。

### 任务 3：观察 systemd

执行 `systemctl --version` 和 `systemctl status`，记录输出。

## 实验步骤

```bash
scp ./test.txt 用户名@目标IP:/tmp/
ruyi device provision --help
systemctl --version
systemctl status
```

## 运行验证

| 验证项 | 预期现象 | 是否通过 |
| --- | --- | --- |
| 文件传输 | 目标系统可看到测试文件 |  |
| provision | 能说明是否支持当前目标 |  |
| systemd | 能记录状态或错误原因 |  |

## 常见问题

| 现象 | 可能原因 | 处理方式 |
| --- | --- | --- |
| SCP 失败 | 网络或权限问题 | 改用 `/tmp/` 或串口传输替代记录 |
| provision 无目标板 | 当前版本暂不支持 | 记录版本和替代烧录路径 |

## 提交要求

- 文件传输命令和结果。
- provision 查询记录。
- systemd 状态观察记录。
