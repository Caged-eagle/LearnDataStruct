#pragma once

//==================================================================
// SqList —— 王道版顺序表：类型定义与全部接口声明
//
// 术语约定（全文统一，避免位序/下标混用）：
//   位序 —— 从 1 开始的逻辑位置，用于函数参数 pos、pos_low、pos_high
//   下标 —— 从 0 开始的数组索引，用于直接访问 L.data[i]（下标变量常用 i 或 idx）
//==================================================================

// 初始化时分配的默认容量
#define InitSize 100

typedef int ElemType;

// 顺序表：动态分配的一维数组 + 当前长度 + 已分配容量
typedef struct SqList {
	ElemType* data = nullptr;  // 数组首地址
	int len = 0;               // 当前元素个数，有效范围 [0, len)
	int maxsize = 0;           // 已分配的容量
}SqList;

//==================================================================
// SqList.cpp：基础接口
//==================================================================

// 判等辅助函数，同时作为 LocateElem 的默认比较器
bool IsEqual(ElemType e_1, ElemType e_2);

// 初始化：分配 InitSize 个元素的空间，len 置 0
// 注意：禁止对尚未 DestroyList 的表重复调用，否则会泄漏旧内存
bool InitList(SqList& L);

// 销毁：释放空间并把三个成员全部归零，之后可安全地再次 InitList
void DestroyList(SqList& L);

// 判空：len == 0 时返回 true
bool ListEmpty(SqList L);

// 求长：返回当前元素个数
int ListLength(SqList L);

// 按位查找：pos 为位序，合法范围 1 ~ len；找到的值经 e 返回
bool GetElem(SqList L, int pos, ElemType& e);

// 按值查找：返回第一个满足 compare 的位序，找不到返回 0
// compare 可省略，默认使用 IsEqual
int LocateElem(SqList L, ElemType e, bool compare(ElemType, ElemType) = IsEqual);

// 插入：在第 pos 个位序之前插入 e，合法范围 1 ~ len+1
// 表满（len == maxsize）时返回 false
bool ListInsert(SqList& L, int pos, ElemType e);

//==================================================================
// SqLIst_wangdao_2025p19.cpp：习题 01~08
//==================================================================

// 01 删除值最小的元素（假设唯一），被删的值经 e 返回
//    空出的位置由最后一个元素填补；表空时返回 false
bool DeleteMin(SqList& L, ElemType& e);

// 02 原地逆置整个顺序表，空间复杂度 O(1)
bool ListReverse(SqList& L);

// 03 删除所有值等于 x 的元素，时间复杂度 O(n)、空间复杂度 O(1)
bool DeleteX(SqList& L, ElemType x);

// 04 删除值落在闭区间 [s, t] 内的所有元素
//    要求 s < t 且表非空，否则返回 false
bool DeleteBetweenSAndT(SqList& L, ElemType s, ElemType t);

// 05 有序表去重：删除重复元素，使表中所有元素互不相同
bool ListDeduplicate(SqList& L);

// 06 归并两个有序表，结果写入 merged_L
//    要求 merged_L 已初始化且容量不小于 L1.len + L2.len
bool ListMerge(SqList& L1, SqList& L2, SqList& merged_L);

// 07 把数组中前 m 个与后 n 个元素整块互换
//    要求 m + n == L.len，即 L 恰好由这两段组成
bool PositionExchange(SqList& L, int m, int n);

// 08 在递增有序表中二分查找 x
//    找到则与后继元素交换位置（若是最后一个元素则不动）
//    没找到则插入 x，并使表仍保持递增有序
bool FindX(SqList& L, ElemType x);

//==================================================================
// SqLIst_wangdao_2025p19_real.cpp：模拟题/统考真题 09~14
//==================================================================

// 09 求三个等长递增序列的交集，结果逐行打印
//    三个表长度不等时返回 false
bool FindSameBetweenThree(SqList& L1, SqList& L2, SqList& L3);

// 10 循环左移 p 个位置，p 为下标，合法范围 1 ~ L.len-1
bool ListMoveForwardP(SqList& L, int p);

// 11 求两个等长升序序列合并后的中位数，结果经 result 返回
//    两个表均按值传入（只读，不做修改）
bool FindMid(SqList L1, SqList L2, ElemType& result);

// 12 求主元素（出现次数超过表长一半的元素）
//    不存在时把 result_e 置为 -1
bool FindMainElem(SqList& L, ElemType& result_e);

// 13 求数组中未出现的最小正整数，结果经 result 返回
bool FindMinPositiveIntNotExist(SqList& L, int& result);

// 14 求三元组 (a, b, c) 的最小距离
//    D = |a-b| + |b-c| + |c-a|，结果经 result_D 返回
bool FindMinD(SqList& L1, SqList& L2, SqList& L3, int& result_D);
