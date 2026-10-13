# 更新日志（CHANGELOG）

SqList_2（王道版顺序表练习及王道 p19 习题）的变更记录。格式参照 [Keep a Changelog](https://keepachangelog.com/)，
每条对应一次 git commit（细节用 `git show <commit>` 查）。

## [0.9] - 2026-10-11

### 已知问题

- `ListInsert` 从未写入 e（缺 `L.data[pos-1] = e;`；编译器 unused parameter 警告即证据）
- `DeleteX` L50 分号陷阱：C++14 编译错误 / C++17 读作 if-with-initializer，静默丢条件
- `DeleteX` 主循环缺 `i--`，实测 [1,2,2,3] 删 2 得 [1,2]（3 丢失、2 残留）
- `ListMerge`：L133/L143/L144 三处 L1↔L2 笔误 + `merged_L.len` 从未设置
- `PositionExchange`：结果未拷回、malloc 未判空、未 free（ASan 泄漏 400B）、未校验 m+n==len
- `FindX`：插入分支缺 `L.len++`、插入位应用 i 而非 mid、末元素交换越界、空表 mid 初值为 0
- `ListReverse`：`i < L.len/2 - 1` 少走一半
- `DeleteBetweenSAndT`：s>=t 缺 `return false`
- `ListDeduplicate`：`data[i+cnt+1]` 越界读，结果随表外残留变化
- `GetElem` 返回类型应为 bool 而非 ElemType
- `InitList` 重复初始化泄漏
