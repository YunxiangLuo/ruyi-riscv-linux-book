# Class 2: Makefile、统一工程模板与基准测试

## 本章目标

- 建立统一工程目录。
- 编写基础 Makefile。
- 理解 CoreMark/Dhrystone 的用途和边界。

## 前置条件

- 已能手动编译 Hello World。
- 已理解 host、target、sysroot。

## 知识简介

手动输入编译命令适合入门，但不适合维护课程中的多个实验。统一工程模板和 Makefile 可以让学生把注意力放在代码和运行验证上，而不是每次重新组织目录和命令。基准测试可以帮助学生观察不同环境的运行差异，但它只是教学对比工具，不代表完整性能评估。

## 环境准备

| 项目 | 要求 | 检查方式 |
| --- | --- | --- |
| Make | 可执行 `make` | `make --version` |
| 工程目录 | 有 `src/`、`build/` 或课程指定结构 | `find . -maxdepth 2 -type d` |

## 操作步骤

1. 建立统一工程目录。
2. 编写 Makefile。
3. 添加 build、run、clean 目标。
4. 运行 CoreMark 或 Dhrystone 并记录结果。

## 运行验证

| 验证项 | 预期现象 | 记录 |
| --- | --- | --- |
| `make build` | 生成可执行文件 |  |
| `make clean` | 清理构建产物 |  |
| benchmark | 输出可记录结果 |  |

## 常见问题

- Makefile 路径错误。
- 交叉编译器变量未配置。
- benchmark 结果被误解为完整性能结论。

## 本章成果

- 形成可复用最小工程模板。

