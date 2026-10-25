# API 目录

本文件是 **导航用的索引**，按数据结构分类，方便快速查找 "有没有这个函数、它是干什么的"。

> **权威说明在各头文件的声明处**（如 `LinearList/SqList/SqList.h`），
> 每个函数前都写了「功能 / 参数合法范围 / 返回值 / 副作用」。
> 本文件只写 "这函数是干什么的"，**不重复参数范围等细节**——
> 细节写两处必然会漂移，而过期的文档比没有文档更糟。

**维护约定**：改接口时同步更新这三处——
头文件（详细）、本文件（一行索引）、`CHANGELOG.md` 的「变更」（记一笔）。

---

## 线性表

### 顺序表 SqList

头文件：`LinearList/SqList/SqList.h`
实现：`SqList.cpp`、`SqLIst_wangdao_2025p19.cpp`、`SqLIst_wangdao_2025p19_real.cpp`

#### 基础接口

| 函数 | 签名 | 用途 |
| --- | --- | --- |
| `IsEqual` | `bool IsEqual(ElemType, ElemType)` | 判等辅助，同时作为 `LocateElem` 的默认比较器 |
| `InitList` | `bool InitList(SqList&)` | 分配空间并把表置空；禁止对未销毁的表重复调用 |
| `DestroyList` | `void DestroyList(SqList&)` | 释放空间、成员归零，之后可安全再次 `InitList` |
| `ListEmpty` | `bool ListEmpty(SqList)` | 判空 |
| `ListLength` | `int ListLength(SqList)` | 返回当前元素个数 |
| `GetElem` | `bool GetElem(SqList, int pos, ElemType& e)` | 按位查找，pos 为位序（1 起） |
| `LocateElem` | `int LocateElem(SqList, ElemType, bool(*)(ElemType,ElemType) = IsEqual)` | 按值查找，返回位序；找不到返回 0 |
| `ListInsert` | `bool ListInsert(SqList&, int pos, ElemType)` | 在第 pos 个位序之前插入；表满返回 false |

#### 习题 01~08（王道《数据结构》p19）

| 题号 | 函数 | 用途 |
| --- | --- | --- |
| 01 | `DeleteMin` | 删除值最小的元素，空位由最后一个元素填补 |
| 02 | `ListReverse` | 原地逆置整个表，空间 O(1) |
| 03 | `DeleteX` | 删除所有值等于 x 的元素，时间 O(n)、空间 O(1) |
| 04 | `DeleteBetweenSAndT` | 删除值落在闭区间 [s, t] 内的元素（要求 s < t） |
| 05 | `ListDeduplicate` | 有序表去重，使元素互不相同 |
| 06 | `ListMerge` | 归并两个有序表，结果写入 `merged_L` |
| 07 | `PositionExchange` | 数组前 m 个与后 n 个元素整块互换（要求 m + n == len） |
| 08 | `FindX` | 二分查找 x：找到则与后继交换，否则插入并保持有序 |

#### 模拟题/统考真题 09~14

| 题号 | 函数 | 用途 |
| --- | --- | --- |
| 09 | `FindSameBetweenThree` | 求三个等长递增序列的交集，结果逐行打印 |
| 10 | `ListMoveForwardP` | 循环左移 p 个位置，三次逆置实现，空间 O(1) |
| 11 | `FindMid` | 求两个等长升序序列合并后的中位数 |
| 12 | `FindMainElem` | 求主元素（出现次数超过一半），不存在则置 -1 |
| 13 | `FindMinPositiveIntNotExist` | 求未出现的最小正整数 |
| 14 | `FindMinD` | 求三元组 (a, b, c) 的最小距离 D |

---

## 术语约定（全文统一）

- **位序**：从 1 起的逻辑位置，用于函数参数 `pos`、`pos_low`、`pos_high`
- **下标**：从 0 起的数组索引，用于直接访问 `L.data[i]`；下标变量常用 `i` 或 `idx`

两者不得混用——这是本项目最容易踩的坑。
