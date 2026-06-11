# 图片与图表规则

## 存放位置

每章图片、截图和图表统一放在本章的 `assets/` 目录。

```text
chapters/chXX_topic/
├── assets/
├── class1.md
├── class2.md
└── lab/
```

## 命名规则

建议使用：

```text
fig-章节号-序号-说明.png
```

示例：

```text
fig-01-01-ruyi-version.png
fig-03-02-gpio-led-connection.png
fig-08-03-http-status-page.png
```

## 引用规则

正文中引用图片时，必须有编号和解释：

```md
![图 1-1 ruyi version 输出](assets/fig-01-01-ruyi-version.png)

图 1-1 展示了 `ruyi version` 的输出结果，用于确认 `ruyi` 已正确安装。
```

## 审核要求

- 图片应来自本机或目标环境验证后的截图。
- 图片中关键区域应框出或标注。
- 多张图片应使用统一编号。
- 代码应使用源码文本，不使用代码截图。
- 引用第三方图片或资料时，应注明来源。

