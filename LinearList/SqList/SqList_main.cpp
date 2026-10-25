#include <stdio.h>
#include <stdlib.h>
#include "SqList.h"

//==================================================================
// SqList_2 测试入口
//
// 设计说明（方便以后自己加用例）：
// 1) 建表时把 maxsize 个格子全部填成哨兵 SENTINEL。
//    这样一旦哪个函数读/写到 len 之外，输出里就会出现 -9999，
//    越界一眼可见，不用开调试器。
// 2) 每个用例都给出"期望值"，不看函数返回值是不是 true，
//    而是核对最终的表长和元素——因为函数完全可以"谎报成功"。
// 3) 结尾汇总通过/失败数，并作为进程退出码返回。
//==================================================================

#define SENTINEL (-9999)

static int g_total = 0;
static int g_failed = 0;

//---- 断言：比较整个顺序表 ----
static void CheckList(const char* name, const SqList& L,
	const int* exp, int expLen) {
	g_total++;
	bool ok = (L.len == expLen);
	for (int i = 0; ok && i < expLen; i++) {
		ok = (L.data[i] == exp[i]);
	}
	if (ok) {
		printf("  [PASS] %s\n", name);
		return;
	}
	g_failed++;
	printf("  [FAIL] %s\n", name);
	printf("         实际:");
	for (int i = 0; i < L.len; i++) printf(" %d", L.data[i]);
	printf("   (len=%d)\n", L.len);
	printf("         期望:");
	for (int i = 0; i < expLen; i++) printf(" %d", exp[i]);
	printf("   (len=%d)\n", expLen);
	for (int i = 0; i < L.len; i++) {
		if (L.data[i] == SENTINEL) {
			printf("         注意: data[%d] 是哨兵值，读到了 len 之外的内存\n", i);
			break;
		}
	}
}

//---- 断言：比较单个整数 ----
static void CheckInt(const char* name, int actual, int expected) {
	g_total++;
	if (actual == expected) {
		printf("  [PASS] %s\n", name);
		return;
	}
	g_failed++;
	printf("  [FAIL] %s  实际=%d 期望=%d\n", name, actual, expected);
}

//---- 断言：布尔 ----
static void CheckBool(const char* name, bool actual, bool expected) {
	g_total++;
	if (actual == expected) {
		printf("  [PASS] %s\n", name);
		return;
	}
	g_failed++;
	printf("  [FAIL] %s  实际=%s 期望=%s\n",
		name, actual ? "true" : "false", expected ? "true" : "false");
}

//---- 辅助：按数组建表，其余格子填哨兵 ----
static SqList Make(const int* arr, int n) {
	SqList L;
	InitList(L);
	for (int i = 0; i < L.maxsize; i++) {
		L.data[i] = SENTINEL;
	}
	for (int i = 0; i < n; i++) {
		L.data[i] = arr[i];
	}
	L.len = n;
	return L;
}

//---- 自定义比较：大于 ----
static bool Greater(ElemType a, ElemType b) {
	return a > b;
}

//==================================================================
// 基础接口
//==================================================================
static void TestBasic(void) {
	printf("---- 基础接口 ----\n");

	SqList L;
	CheckBool("InitList 返回 true", InitList(L), true);
	CheckInt("初始 len", L.len, 0);
	CheckInt("初始 maxsize", L.maxsize, InitSize);
	CheckBool("初始为空表", ListEmpty(L), true);

	const int a1[] = { 10,20,30 };
	SqList L1 = Make(a1, 3);
	const int e1[] = { 99,10,20,30 };
	ListInsert(L1, 1, 99);
	CheckList("ListInsert 头插 pos=1", L1, e1, 4);

	SqList L2 = Make(a1, 3);
	const int e2[] = { 10,99,20,30 };
	ListInsert(L2, 2, 99);
	CheckList("ListInsert 中间插 pos=2", L2, e2, 4);

	SqList L3 = Make(a1, 3);
	const int e3[] = { 10,20,30,99 };
	ListInsert(L3, 4, 99);
	CheckList("ListInsert 尾插 pos=len+1", L3, e3, 4);

	SqList L4 = Make(a1, 3);
	CheckBool("ListInsert pos=0 非法", ListInsert(L4, 0, 1), false);
	CheckBool("ListInsert pos=len+2 非法", ListInsert(L4, 5, 1), false);
	const int e4[] = { 10,20,30 };
	CheckList("非法插入后表不变", L4, e4, 3);

	SqList L5;
	InitList(L5);
	for (int i = 0; i < L5.maxsize; i++) {
		ListInsert(L5, i + 1, i);
	}
	CheckInt("填满后 len==maxsize", L5.len, L5.maxsize);
	CheckBool("表满插入返回 false", ListInsert(L5, 1, 1), false);
	DestroyList(L5);

	SqList L6 = Make(a1, 3);
	ElemType e = 0;
	CheckBool("GetElem(1) 成功", GetElem(L6, 1, e), true);
	CheckInt("GetElem(1) 取值", e, 10);
	CheckBool("GetElem(3) 成功", GetElem(L6, 3, e), true);
	CheckInt("GetElem(3) 取值", e, 30);
	CheckBool("GetElem(0) 失败", GetElem(L6, 0, e), false);
	CheckBool("GetElem(len+1) 失败", GetElem(L6, 4, e), false);

	CheckInt("LocateElem(20) 返回位序2", LocateElem(L6, 20), 2);
	CheckInt("LocateElem(99) 找不到返回0", LocateElem(L6, 99), 0);
	CheckInt("LocateElem 用 Greater(25)", LocateElem(L6, 25, Greater), 3);

	CheckInt("ListLength", ListLength(L6), 3);
	CheckBool("非空表 ListEmpty 为 false", ListEmpty(L6), false);

	DestroyList(L6);
	CheckInt("DestroyList 后 len", L6.len, 0);
	CheckInt("DestroyList 后 maxsize", L6.maxsize, 0);
	CheckBool("DestroyList 后 data 置空", L6.data == nullptr, true);

	DestroyList(L1);
	DestroyList(L2);
	DestroyList(L3);
	DestroyList(L4);
	DestroyList(L);
}

//==================================================================
// 01 删除最小值
//==================================================================
static void TestDeleteMin(void) {
	printf("---- 01 DeleteMin ----\n");

	const int a[] = { 5,2,8,1,9 };
	const int e[] = { 5,2,8,9 };
	SqList L = Make(a, 5);
	ElemType got = 0;
	CheckBool("DeleteMin 返回 true", DeleteMin(L, got), true);
	CheckInt("返回被删的最小值", got, 1);
	CheckList("空位由最后一个元素填补", L, e, 4);
	DestroyList(L);

	const int b[] = { 5,3,1 };
	const int eb[] = { 5,3 };
	SqList L2 = Make(b, 3);
	DeleteMin(L2, got);
	CheckInt("最小值在末尾时返回值", got, 1);
	CheckList("最小值在末尾", L2, eb, 2);
	DestroyList(L2);

	const int c[] = { 7 };
	SqList L3 = Make(c, 1);
	DeleteMin(L3, got);
	CheckInt("单元素时返回该元素", got, 7);
	CheckInt("单元素删除后 len", L3.len, 0);
	DestroyList(L3);

	SqList L4;
	InitList(L4);
	CheckBool("空表 DeleteMin 返回 false", DeleteMin(L4, got), false);
	DestroyList(L4);
}

//==================================================================
// 02 逆置
//==================================================================
static void TestListReverse(void) {
	printf("---- 02 ListReverse ----\n");

	const int a[] = { 1,2,3,4 };
	const int ea[] = { 4,3,2,1 };
	SqList L = Make(a, 4);
	CheckBool("ListReverse 返回 true", ListReverse(L), true);
	CheckList("偶数长逆置", L, ea, 4);
	DestroyList(L);

	const int b[] = { 1,2,3 };
	const int eb[] = { 3,2,1 };
	SqList L2 = Make(b, 3);
	ListReverse(L2);
	CheckList("奇数长逆置", L2, eb, 3);
	DestroyList(L2);

	const int c[] = { 1,2 };
	const int ec[] = { 2,1 };
	SqList L3 = Make(c, 2);
	ListReverse(L3);
	CheckList("长度2逆置", L3, ec, 2);
	DestroyList(L3);

	const int d[] = { 9 };
	SqList L4 = Make(d, 1);
	ListReverse(L4);
	CheckList("单元素逆置后不变", L4, d, 1);
	DestroyList(L4);

	SqList L5;
	InitList(L5);
	CheckBool("空表 ListReverse 返回 false", ListReverse(L5), false);
	DestroyList(L5);
}

//==================================================================
// 03 删除所有值为 x 的元素
//==================================================================
static void TestDeleteX(void) {
	printf("---- 03 DeleteX ----\n");

	const int a[] = { 1,2,3,2,4 };
	const int ea[] = { 1,3,4 };
	SqList L = Make(a, 5);
	CheckBool("DeleteX 返回 true", DeleteX(L, 2), true);
	CheckList("删除中间的 x", L, ea, 3);
	DestroyList(L);

	const int b[] = { 2,2,3 };
	const int eb[] = { 3 };
	SqList L2 = Make(b, 3);
	DeleteX(L2, 2);
	CheckList("开头连续两个 x", L2, eb, 1);
	DestroyList(L2);

	const int c[] = { 2,2,2 };
	SqList L3 = Make(c, 3);
	DeleteX(L3, 2);
	CheckInt("全表都是 x 后 len 归零", L3.len, 0);
	DestroyList(L3);

	const int d[] = { 1,3,5 };
	SqList L4 = Make(d, 3);
	DeleteX(L4, 2);
	CheckList("表中没有 x 时不变", L4, d, 3);
	DestroyList(L4);

	const int f[] = { 1,2,3 };
	const int ef[] = { 2,3 };
	SqList L5 = Make(f, 3);
	DeleteX(L5, 1);
	CheckList("删除首元素", L5, ef, 2);
	DestroyList(L5);

	const int g[] = { 1,2,3 };
	const int eg[] = { 1,2 };
	SqList L6 = Make(g, 3);
	DeleteX(L6, 3);
	CheckList("删除尾元素", L6, eg, 2);
	DestroyList(L6);

	SqList L7;
	InitList(L7);
	CheckBool("空表 DeleteX 返回 false", DeleteX(L7, 1), false);
	DestroyList(L7);
}

//==================================================================
// 04 删除区间 [s, t] 内的元素
//==================================================================
static void TestDeleteBetweenSAndT(void) {
	printf("---- 04 DeleteBetweenSAndT ----\n");

	const int a[] = { 1,5,8,3 };
	const int ea[] = { 1,8 };
	SqList L = Make(a, 4);
	CheckBool("返回 true", DeleteBetweenSAndT(L, 2, 6), true);
	CheckList("删除区间内元素", L, ea, 2);
	DestroyList(L);

	const int b[] = { 5,5,1 };
	const int eb[] = { 1 };
	SqList L2 = Make(b, 3);
	DeleteBetweenSAndT(L2, 2, 6);
	CheckList("区间内元素连续", L2, eb, 1);
	DestroyList(L2);

	const int c[] = { 1,5,8,3 };
	SqList L3 = Make(c, 4);
	CheckBool("s>=t 非法返回 false", DeleteBetweenSAndT(L3, 8, 2), false);
	CheckList("非法输入后表不变", L3, c, 4);
	DestroyList(L3);

	const int d[] = { 7,8,9 };
	SqList L4 = Make(d, 3);
	DeleteBetweenSAndT(L4, 2, 6);
	CheckList("无元素在区间内时不变", L4, d, 3);
	DestroyList(L4);

	const int f[] = { 2,5,8,3 };
	const int ef[] = { 8 };
	SqList L5 = Make(f, 4);
	DeleteBetweenSAndT(L5, 2, 6);
	CheckList("首元素也在区间内", L5, ef, 1);
	DestroyList(L5);

	SqList L6;
	InitList(L6);
	CheckBool("空表返回 false", DeleteBetweenSAndT(L6, 2, 6), false);
	DestroyList(L6);
}

//==================================================================
// 05 有序表去重
//==================================================================
static void TestListDeduplicate(void) {
	printf("---- 05 ListDeduplicate ----\n");

	const int a[] = { 1,1,2,2,3 };
	const int ea[] = { 1,2,3 };
	SqList L = Make(a, 5);
	CheckBool("返回 true", ListDeduplicate(L), true);
	CheckList("常规去重", L, ea, 3);
	DestroyList(L);

	const int b[] = { 1,1,1 };
	const int eb[] = { 1 };
	SqList L2 = Make(b, 3);
	ListDeduplicate(L2);
	CheckList("全部相同", L2, eb, 1);
	DestroyList(L2);

	const int c[] = { 1,2,3 };
	SqList L3 = Make(c, 3);
	ListDeduplicate(L3);
	CheckList("无重复时不变", L3, c, 3);
	DestroyList(L3);

	const int d[] = { 1,2,2 };
	const int ed[] = { 1,2 };
	SqList L4 = Make(d, 3);
	ListDeduplicate(L4);
	CheckList("尾部重复", L4, ed, 2);
	DestroyList(L4);

	const int f[] = { 1,1,2 };
	const int ef[] = { 1,2 };
	SqList L5 = Make(f, 3);
	ListDeduplicate(L5);
	CheckList("头部重复", L5, ef, 2);
	DestroyList(L5);

	const int g[] = { 7 };
	SqList L6 = Make(g, 1);
	ListDeduplicate(L6);
	CheckList("单元素", L6, g, 1);
	DestroyList(L6);

	SqList L7;
	InitList(L7);
	CheckBool("空表返回 false", ListDeduplicate(L7), false);
	DestroyList(L7);
}

//==================================================================
// 06 两个有序表归并
//==================================================================
static void TestListMerge(void) {
	printf("---- 06 ListMerge ----\n");

	const int a[] = { 1,3,5 }, b[] = { 2,4,6 };
	const int e[] = { 1,2,3,4,5,6 };
	SqList L1 = Make(a, 3), L2 = Make(b, 3), M;
	InitList(M);
	CheckBool("返回 true", ListMerge(L1, L2, M), true);
	CheckList("归并两个交错的有序表", M, e, 6);
	DestroyList(M);

	const int c[] = { 1,2,3 }, d[] = { 4,5,6 };
	const int ec[] = { 1,2,3,4,5,6 };
	SqList L3 = Make(c, 3), L4 = Make(d, 3), M2;
	InitList(M2);
	ListMerge(L3, L4, M2);
	CheckList("两个表首尾相接", M2, ec, 6);
	DestroyList(M2);

	const int f[] = { 1,3,5 }, g[] = { 2,3,5 };
	const int ef[] = { 1,2,3,3,5,5 };
	SqList L5 = Make(f, 3), L6 = Make(g, 3), M3;
	InitList(M3);
	ListMerge(L5, L6, M3);
	CheckList("有相等元素时都保留", M3, ef, 6);
	DestroyList(M3);

	const int h[] = { 1 }, k[] = { 2 };
	const int eh[] = { 1,2 };
	SqList L7 = Make(h, 1), L8 = Make(k, 1), M4;
	InitList(M4);
	ListMerge(L7, L8, M4);
	CheckList("各一个元素", M4, eh, 2);
	DestroyList(M4);

	const int m[] = { 1,1,1 }, n[] = { 1,1,1 };
	const int em[] = { 1,1,1,1,1,1 };
	SqList L9 = Make(m, 3), L10 = Make(n, 3), M5;
	InitList(M5);
	ListMerge(L9, L10, M5);
	CheckList("两表元素全部相等", M5, em, 6);
	DestroyList(M5);

	SqList E1, M6;
	InitList(E1); InitList(M6);
	CheckBool("有空表时返回 false", ListMerge(E1, L2, M6), false);
	DestroyList(E1); DestroyList(M6);

	DestroyList(L1); DestroyList(L2);
	DestroyList(L3); DestroyList(L4);
	DestroyList(L5); DestroyList(L6);
	DestroyList(L7); DestroyList(L8);
	DestroyList(L9); DestroyList(L10);
}

//==================================================================
// 07 两块位置互换
//==================================================================
static void TestPositionExchange(void) {
	printf("---- 07 PositionExchange ----\n");

	const int a[] = { 1,2,3,4,5,6,7 };
	const int ea[] = { 4,5,6,7,1,2,3 };
	SqList L = Make(a, 7);
	CheckBool("返回 true", PositionExchange(L, 3, 4), true);
	CheckList("m=3 n=4 互换", L, ea, 7);
	DestroyList(L);

	const int b[] = { 1,2,3,4 };
	const int eb[] = { 3,4,1,2 };
	SqList L2 = Make(b, 4);
	PositionExchange(L2, 2, 2);
	CheckList("m=2 n=2 互换", L2, eb, 4);
	DestroyList(L2);

	const int c[] = { 1,2,3,4,5 };
	SqList L3 = Make(c, 5);
	CheckBool("m+n != len 返回 false", PositionExchange(L3, 2, 2), false);
	CheckBool("m=0 返回 false", PositionExchange(L3, 0, 5), false);
	CheckList("非法输入后表不变", L3, c, 5);
	DestroyList(L3);

	const int d[] = { 1,2 };
	const int ed[] = { 2,1 };
	SqList L4 = Make(d, 2);
	PositionExchange(L4, 1, 1);
	CheckList("m=1 n=1 互换", L4, ed, 2);
	DestroyList(L4);
}

//==================================================================
// 08 二分查找 x：找到则与后继交换，否则插入并保持有序
//==================================================================
static void TestFindX(void) {
	printf("---- 08 FindX ----\n");

	const int a[] = { 1,3,5,7,9 };
	const int ea[] = { 1,3,7,5,9 };
	SqList L = Make(a, 5);
	CheckBool("找到 x=5 返回 true", FindX(L, 5), true);
	CheckList("x=5 与后继交换", L, ea, 5);
	DestroyList(L);

	SqList L2 = Make(a, 5);
	FindX(L2, 9);
	CheckList("x 是最后一个元素时应保持原样", L2, a, 5);
	DestroyList(L2);

	const int eb[] = { 1,3,4,5,7,9 };
	SqList L3 = Make(a, 5);
	FindX(L3, 4);
	CheckList("x=4 插入到中间", L3, eb, 6);
	DestroyList(L3);

	const int ec[] = { 0,1,3,5,7,9 };
	SqList L4 = Make(a, 5);
	FindX(L4, 0);
	CheckList("x=0 插到最前面", L4, ec, 6);
	DestroyList(L4);

	const int ed[] = { 1,3,5,7,9,11 };
	SqList L5 = Make(a, 5);
	FindX(L5, 11);
	CheckList("x=11 插到最后面", L5, ed, 6);
	DestroyList(L5);

	const int ee[] = { 1,2,3,5,7,9 };
	SqList L6 = Make(a, 5);
	FindX(L6, 2);
	CheckList("x=2 插到下标1", L6, ee, 6);
	DestroyList(L6);

	const int ef[] = { 5 };
	SqList L7;
	InitList(L7);
	CheckBool("空表插入返回 true", FindX(L7, 5), true);
	CheckList("空表插入后 len=1", L7, ef, 1);
	DestroyList(L7);
}

//==================================================================
// 09 三个等长序列的交集
//==================================================================
static void TestFindSameBetweenThree(void) {
	printf("---- 09 FindSameBetweenThree ----\n");

	const int a[] = { 1,2,3 };
	const int b[] = { 2,3,4 };
	const int c[] = { -1,0,2 };
	SqList L1 = Make(a, 3), L2 = Make(b, 3), L3 = Make(c, 3);

	//长度不等应拒绝
	SqList Lshort = Make(a, 2);
	CheckBool("长度不等时返回 false", FindSameBetweenThree(L1, Lshort, L3), false);
	DestroyList(Lshort);

	//教材示例 A={1,2,3} B={2,3,4} C={-1,0,2}，共同元素是 2
	printf("  （教材示例，下面应打印 2）\n");
	FindSameBetweenThree(L1, L2, L3);

	//改用三个相同的表：j 与 k 始终同步，可以跑完，应依次打印 2 3 4
	const int s[] = { 2,3,4 };
	SqList S1 = Make(s, 3), S2 = Make(s, 3), S3 = Make(s, 3);
	printf("  （三个相同的表 {2,3,4}，下面应依次打印 2 3 4）\n");
	FindSameBetweenThree(S1, S2, S3);

	DestroyList(S1); DestroyList(S2); DestroyList(S3);
	DestroyList(L1); DestroyList(L2); DestroyList(L3);
}

//==================================================================
// 10 循环左移 p 个位置（p 为下标）
//==================================================================
static void TestListMoveForwardP(void) {
	printf("---- 10 ListMoveForwardP ----\n");

	const int a[] = { 1,2,3,4,5 };
	const int ea[] = { 3,4,5,1,2 };
	SqList L = Make(a, 5);
	CheckBool("左移 p=2 返回 true", ListMoveForwardP(L, 2), true);
	CheckList("左移 p=2 结果", L, ea, 5);
	DestroyList(L);

	//p=0 时不应变化
	SqList L2 = Make(a, 5);
	ListMoveForwardP(L2, 0);
	CheckList("p=0 时不变", L2, a, 5);
	DestroyList(L2);

	//p 越界
	SqList L3 = Make(a, 5);
	CheckBool("p 越界返回 false", ListMoveForwardP(L3, 5), false);
	CheckList("非法输入后表不变", L3, a, 5);
	DestroyList(L3);

	//单元素
	const int one[] = { 9 };
	SqList L4 = Make(one, 1);
	ListMoveForwardP(L4, 0);
	CheckList("单元素左移 0 位", L4, one, 1);
	DestroyList(L4);
}

//==================================================================
// 11 两个等长升序序列的中位数
//==================================================================
static void TestFindMid(void) {
	printf("---- 11 FindMid ----\n");

	//教材示例：合并后为 2,4,6,8,11,13,15,17,19,20，第 ceil(10/2)=5 个是 11
	const int a[] = { 11,13,15,17,19 };
	const int b[] = { 2,4,6,8,20 };
	SqList L1 = Make(a, 5), L2 = Make(b, 5);
	ElemType r = 0;
	CheckBool("教材示例返回 true", FindMid(L1, L2, r), true);
	CheckInt("教材示例中位数", r, 11);
	DestroyList(L1); DestroyList(L2);

	//合并后 1,2,3,4,5,6，第 ceil(6/2)=3 个是 3
	const int c[] = { 1,3,5 };
	const int d[] = { 2,4,6 };
	SqList L3 = Make(c, 3), L4 = Make(d, 3);
	r = 0;
	FindMid(L3, L4, r);
	CheckInt("等长3的中位数", r, 3);
	DestroyList(L3); DestroyList(L4);

	//长度不等应拒绝
	const int e[] = { 1,2 };
	SqList L5 = Make(c, 3), L6 = Make(e, 2);
	CheckBool("长度不等返回 false", FindMid(L5, L6, r), false);
	DestroyList(L5); DestroyList(L6);

	SqList L7;
	InitList(L7);
	CheckBool("空表返回 false", FindMid(L7, L7, r), false);
	DestroyList(L7);
}

//==================================================================
// 12 主元素（出现次数超过一半），否则为 -1
//==================================================================
static void TestFindMainElem(void) {
	printf("---- 12 FindMainElem ----\n");

	//教材示例1：5 出现 5 次，n=8，5 > 4，主元素是 5
	const int a[] = { 0,5,5,3,5,7,5,5 };
	SqList L = Make(a, 8);
	ElemType r = 0;
	CheckBool("返回 true", FindMainElem(L, r), true);
	CheckInt("教材示例1 主元素", r, 5);
	DestroyList(L);

	//教材示例2：5 只出现 4 次，不超过一半，无主元素
	const int b[] = { 0,5,5,3,5,1,5,7 };
	SqList L2 = Make(b, 8);
	r = 0;
	FindMainElem(L2, r);
	CheckInt("教材示例2 无主元素", r, -1);
	DestroyList(L2);

	//单元素
	const int c[] = { 7 };
	SqList L3 = Make(c, 1);
	r = 0;
	FindMainElem(L3, r);
	CheckInt("单元素即为自身", r, 7);
	DestroyList(L3);

	//全部相同
	const int d[] = { 4,4,4 };
	SqList L4 = Make(d, 3);
	r = 0;
	FindMainElem(L4, r);
	CheckInt("全部相同", r, 4);
	DestroyList(L4);

	SqList L5;
	InitList(L5);
	CheckBool("空表返回 false", FindMainElem(L5, r), false);
	DestroyList(L5);
}

//==================================================================
// 13 未出现的最小正整数
//==================================================================
static void TestFindMinPositiveIntNotExist(void) {
	printf("---- 13 FindMinPositiveIntNotExist ----\n");

	//教材示例1：{-5,3,2,3} 缺 1
	const int a[] = { -5,3,2,3 };
	SqList L = Make(a, 4);
	int r = -1;
	CheckBool("返回 true", FindMinPositiveIntNotExist(L, r), true);
	CheckInt("教材示例1", r, 1);
	DestroyList(L);

	//教材示例2：{1,2,3} 缺 4
	const int b[] = { 1,2,3 };
	SqList L2 = Make(b, 3);
	r = -1;
	FindMinPositiveIntNotExist(L2, r);
	CheckInt("教材示例2", r, 4);
	DestroyList(L2);

	//没有 1
	const int c[] = { 2,3,4 };
	SqList L3 = Make(c, 3);
	r = -1;
	FindMinPositiveIntNotExist(L3, r);
	CheckInt("缺 1 时应返回 1", r, 1);
	DestroyList(L3);

	SqList L4;
	InitList(L4);
	CheckBool("空表返回 false", FindMinPositiveIntNotExist(L4, r), false);
	DestroyList(L4);
}

//==================================================================
// 14 三元组最小距离
//==================================================================
static void TestFindMinD(void) {
	printf("---- 14 FindMinD ----\n");

	//教材示例：S1={-1,0,9} S2={-25,-10,10,11} S3={2,9,17,30,41}
	//最优三元组 (9,10,9)，距离 |9-10|+|10-9|+|9-9| = 2
	const int a[] = { -1,0,9 };
	const int b[] = { -25,-10,10,11 };
	const int c[] = { 2,9,17,30,41 };
	SqList L1 = Make(a, 3), L2 = Make(b, 4), L3 = Make(c, 5);
	int d = -1;
	CheckBool("返回 true", FindMinD(L1, L2, L3, d), true);
	CheckInt("教材示例最小距离", d, 2);
	DestroyList(L1); DestroyList(L2); DestroyList(L3);

	//三个数相同则距离为 0
	const int e[] = { 5 }, f[] = { 5 }, g[] = { 5 };
	SqList L4 = Make(e, 1), L5 = Make(f, 1), L6 = Make(g, 1);
	d = -1;
	FindMinD(L4, L5, L6, d);
	CheckInt("三数相同距离为 0", d, 0);
	DestroyList(L4); DestroyList(L5); DestroyList(L6);

	SqList E1, E2, E3;
	InitList(E1); InitList(E2); InitList(E3);
	CheckBool("空表返回 false", FindMinD(E1, L2, L3, d), false);
	DestroyList(E1); DestroyList(E2); DestroyList(E3);
}

//==================================================================
int main(void) {
	TestBasic();
	TestDeleteMin();
	TestListReverse();
	TestDeleteX();
	TestDeleteBetweenSAndT();
	TestListDeduplicate();
	TestListMerge();
	TestPositionExchange();
	TestFindX();
	TestFindSameBetweenThree();
	TestListMoveForwardP();
	TestFindMid();
	TestFindMainElem();
	TestFindMinPositiveIntNotExist();
	TestFindMinD();

	printf("\n========================================\n");
	printf("共 %d 项，通过 %d，失败 %d\n",
		g_total, g_total - g_failed, g_failed);
	printf("========================================\n");
	return g_failed == 0 ? 0 : 1;
}
