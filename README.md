# LearnDataStruct

考研数据结构（408 / 王道）的手写实现与练习仓库，C++ 实现。  
以 "想清楚" 为优先，不追求工程化封装；错误与修正都记在 `CHANGELOG.md` 里。

## 当前进度

- **线性表 → 顺序表 `SqList`**
  - 基础接口：初始化 / 销毁 / 判空 / 求长 / 按位查找 / 按值查找 / 插入
  - 王道《数据结构》p19 综合应用题 01~08
  - 模拟题/统考真题 09~14
  - 全套测试：114 项断言，实测全部通过

## 目录结构

```text
LearnDataStruct.slnx
CHANGELOG.md
README.md
LinearList/
└── SqList/
    ├── SqList.h                          顺序表定义 + 全部接口声明
    ├── SqList.cpp                        基础接口实现
    ├── SqLIst_wangdao_2025p19.cpp        习题 01~08
    ├── SqLIst_wangdao_2025p19_real.cpp  模拟题/统考真题 09~14
    └── SqList_main.cpp                   测试入口
```

## 构建与运行

**Visual Studio 2026**：打开 `LearnDataStruct.slnx`，把 `SqList` 设为启动项目后运行。  
语言标准 C++17（实测通过）。

**命令行**（MSVC 环境）：

```bat
cl /std:c++17 /utf-8 /EHsc ^
   SqList.cpp SqLIst_wangdao_2025p19.cpp SqLIst_wangdao_2025p19_real.cpp SqList_main.cpp
```

测试输出逐条打印 `[PASS]` / `[FAIL]`，结尾汇总 "共 N 项，通过 X，失败 Y"。  
**失败数会作为进程退出码返回**（0 = 全绿），方便日后挂到 CI。

## 代码约定

- **位序** 从 1 起（函数参数 `pos`、`pos_low`、`pos_high`），  
  **下标** 从 0 起（`L.data[i]`，下标变量常用 `i` 或 `idx`）——两者不得混用，这是本项目最容易踩的坑
- 顺序表用 `malloc` / `free` 手动管理，必须配对使用：  
  `InitList` 之后要 `DestroyList`，且 **禁止在未销毁时重复 `InitList`**（会泄漏旧内存）
- 每道算法题上方保留题目原文与 "设计思想" 笔记；  
  注释里的踩坑记录是刻意留的，属于学习过程的一部分
- 测试不看函数返回值，而是核对最终的表长与全部元素——因为函数完全可以 "谎报成功"

## 接口文档

- **索引**：[`docs/API.md`](docs/API.md) —— 按数据结构分类的接口目录，用来快速查找
- **权威说明**：各头文件的声明处（如 `LinearList/SqList/SqList.h`）——  
  每个函数前都写了「功能 / 参数合法范围 / 返回值 / 副作用」

`docs/API.md` 只写 "这函数是干什么的"，参数范围、返回值之类的细节一律以头文件为准。

> 约定：改接口时同步更新三处——  
> 头文件（详细）、`docs/API.md`（一行索引）、`CHANGELOG.md` 的「变更」（记一笔）。

## 说明

本仓库为个人学习笔记，代码以弄懂原理为目标。  
仓库设为公开是为了方便查阅。
