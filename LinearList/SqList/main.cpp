#include <stdio.h>
#include "SqList.h"

//内存说明：每个测试里的 SqList L{} 都是栈上对象，
//函数一结束（正常走到末尾，或中途 return）编译器就会自动调用 ~SqList()，
//由析构函数 free(data)。所以测试里不需要、也不应该再手写 free(L.data)：
//手写不仅重复，而且中途 return 的路径很容易漏掉，反而造成泄漏。
//这就是"RAII"最直观的好处——释放的时机交给作用域，不靠人记。

//---- 简易断言：失败只记录不中断，方便一次看清所有问题 ----
static int g_failed = 0;
#define CHECK(desc, cond)                                              \
        do {                                                           \
                if (cond) {                                            \
                        printf("[PASS] %s\n", desc);                   \
                }                                                      \
                else {                                                 \
                        printf("[FAIL] %s\n", desc);                   \
                        g_failed++;                                    \
                }                                                      \
        } while (0)

//---- 致命断言：失败即结束当前测试 ----
//初始化是整个测试的前置条件。若 InitList 失败（比如内存不够），
//后续断言仍会拿着一个没准备好的表继续跑，于是"分配失败"会被误报成一堆
//逻辑错误，甚至直接崩溃、把真正的根因埋掉。
//注意：REQUIRE 内部有 return，只能用在返回 void 的测试函数里。
#define REQUIRE(desc, cond)                                            \
        do {                                                            \
                if (cond) {                                            \
                        printf("[PASS] %s\n", desc);                   \
                }                                                      \
                else {                                                 \
                        printf("[FAIL] %s (fatal, test aborted)\n",    \
                                desc);                                 \
                        g_failed++;                                    \
                        return;                                        \
                }                                                      \
        } while (0)

//---- 测试 1：空表的边界 ----
//空表 len=0，位序合法范围是 1~0（不存在），插入合法范围是 1~1
static void TestEmptyListBoundary(void) {
	printf("---- Test 1: empty list boundary ----\n");
	SqList L{};
	REQUIRE("InitList", InitList(L));
	ElemType e = 0;
	CHECK("GetElem(-1) on empty list fails", !GetElem(L, -1, e));
	CHECK("GetElem(0) on empty list fails", !GetElem(L, 0, e));
	CHECK("GetElem(1) on empty list fails", !GetElem(L, 1, e));
	CHECK("ListDelete(1) on empty list fails", !ListDelete(L, 1, e));
	CHECK("LocateElem on empty list returns -1", LocateElem(L, 10) == -1);

	//空表插入：位序 0 和 2 都非法，只有位序 1 合法
	CHECK("ListInsert(0) on empty list fails", !ListInsert(L, 0, 99));
	CHECK("ListInsert(2) on empty list fails", !ListInsert(L, 2, 99));
	CHECK("ListInsert(1) on empty list succeeds", ListInsert(L, 1, 99));
	CHECK("len == 1 after first insert", L.len == 1);
	CHECK("GetElem(1) == 99 after insert",
		GetElem(L, 1, e) && e == 99);
}

//---- 测试 2：非空表的上下边界 + 位序一致性 ----
static void TestNonEmptyBoundary(void) {
	printf("---- Test 2: non-empty list boundary ----\n");
	SqList L{};
	REQUIRE("InitList", InitList(L));
	//表内：10 20 30
	CHECK("insert pos=1", ListInsert(L, 1, 10));
	CHECK("insert pos=2", ListInsert(L, 2, 20));
	CHECK("insert pos=3", ListInsert(L, 3, 30));

	ElemType e = 0;
	CHECK("GetElem(0) fails", !GetElem(L, 0, e));
	CHECK("GetElem(len+1=4) fails", !GetElem(L, L.len + 1, e));
	CHECK("GetElem(len=3) == 30 (last elem)",
		GetElem(L, L.len, e) && e == 30);

	//尾插：位序 len+1 是插入的上边界
	CHECK("ListInsert(len+1=4) tail insert succeeds",
		ListInsert(L, L.len + 1, 40));
	CHECK("GetElem(4) == 40 after tail insert",
		GetElem(L, 4, e) && e == 40);
	//位序 len+2 已越界（注意 len 已因尾插变为 4，必须用动态表达式）
	CHECK("ListInsert(len+2) fails", !ListInsert(L, L.len + 2, 50));
	//位序 0 已越界
	CHECK("ListInsert(0) fails", !ListInsert(L, 0, 50));

	//头插：位序 1 是插入的下边界
	CHECK("ListInsert(1) head insert succeeds", ListInsert(L, 1, 5));
	//此时表内：5 10 20 30 40

	//LocateElem 返回位序，应能与 GetElem 正确往返
	CHECK("LocateElem(5) == 1 (head)", LocateElem(L, 5) == 1);
	CHECK("LocateElem(40) == len (tail)",
		LocateElem(L, 40) == L.len);
	CHECK("LocateElem(99) == -1 (not found)",
		LocateElem(L, 99) == -1);
	CHECK("round trip: GetElem(LocateElem(e)) == e",
		GetElem(L, LocateElem(L, 30), e) && e == 30);

	//删除边界：位序 1 和 len 合法，0 和 len+1 非法
	CHECK("ListDelete(0) fails", !ListDelete(L, 0, e));
	CHECK("ListDelete(len+1=6) fails", !ListDelete(L, L.len + 1, e));
	CHECK("ListDelete(1) head delete succeeds",
		ListDelete(L, 1, e) && e == 5);
	CHECK("ListDelete(len=4) tail delete succeeds",
		ListDelete(L, L.len, e) && e == 40);
	CHECK("list back to 10 20 30",
		L.len == 3 && L.data[0] == 10 && L.data[1] == 20 &&
		L.data[2] == 30);
}

//---- 测试 3：表满时的删除（原 Bug 2 的场景）----
//让 len 精确等于 MaxSize 后再删除，
//原实现会在左移时读 data[len] = data[MaxSize] 造成越界
static void TestDeleteWhenExactlyFull(void) {
	printf("---- Test 3: delete when list is exactly full ----\n");
	SqList L{};
	REQUIRE("InitList", InitList(L));

	//填到 len == MaxSize（全程尾插，插入前 len < MaxSize，不会触发扩容）
	while (L.len < L.MaxSize) {
		if (!ListInsert(L, L.len + 1, L.len)) {
			CHECK("fill list to full", false);
			return;
		}
	}
	CHECK("list exactly full (len == MaxSize)", L.len == L.MaxSize);
	CHECK("filling never triggers expansion (MaxSize unchanged)",
		L.MaxSize == InitSize);
	printf("       len = %d, MaxSize = %d\n", L.len, L.MaxSize);

	ElemType e = 0;
	CHECK("delete head when exactly full succeeds",
		ListDelete(L, 1, e) && e == 0);
	CHECK("len == MaxSize - 1 after delete",
		L.len == L.MaxSize - 1);
	CHECK("data shifted correctly (1,2)",
		L.data[0] == 1 && L.data[1] == 2);

	//扩容时机：未满插入不应扩容；重新插满后再插必须扩容
	int oldMax = L.MaxSize;
	CHECK("insert with room does not expand",
		ListInsert(L, L.len + 1, 9999) && L.MaxSize == oldMax);
	CHECK("insert when full again expands",
		ListInsert(L, L.len + 1, 8888) && L.MaxSize > oldMax);
	CHECK("MaxSize doubled", L.MaxSize == 2 * oldMax);
	CHECK("data intact after expansion",
		L.data[0] == 1 && L.data[L.len - 1] == 8888);
}

//---- 测试 4：扩容函数自身的参数边界 ----
static void TestIncreaseListLengthBoundary(void) {
	printf("---- Test 4: IncreaseListLength boundary ----\n");
	SqList L{};
	REQUIRE("InitList", InitList(L));
	CHECK("IncreaseListLength(-1) fails",
		!IncreaseListLength(L, -1));
	CHECK("IncreaseListLength(0) succeeds as no-op",
		IncreaseListLength(L, 0) && L.MaxSize == InitSize);
	CHECK("IncreaseListLength(10) works",
		IncreaseListLength(L, 10) && L.MaxSize == InitSize + 10);
}

//---- 测试 5：连续头插/头删压力（反复踩位序 1 的移位路径）----
static void TestHeadInsertDeleteStress(void) {
	printf("---- Test 5: head insert/delete stress ----\n");
	SqList L{};
	REQUIRE("InitList", InitList(L));

	//200 次头插，在 len == 100 时触发一次扩容（100 -> 200），每次都走完整移位路径
	for (int i = 0; i < 200; i++) {
		if (!ListInsert(L, 1, i)) {
			CHECK("head insert x200", false);
			return;
		}
	}
	CHECK("200 head inserts keep order (data[0]==199)",
		L.data[0] == 199 && L.data[199] == 0);
	CHECK("len == 200", L.len == 200);

	//200 次头删，校验删除顺序
	for (int i = 0; i < 200; i++) {
		ElemType e = 0;
		if (!ListDelete(L, 1, e) || e != 199 - i) {
			CHECK("head delete x200", false);
			return;
		}
	}
	CHECK("200 head deletes in correct order, len == 0",
		L.len == 0);

	//删空后回到空表边界，再插一次确认复用正常
	CHECK("insert after emptying works",
		ListInsert(L, 1, 42) && L.data[0] == 42);
}

int main() {
	TestEmptyListBoundary();
	TestNonEmptyBoundary();
	TestDeleteWhenExactlyFull();
	TestIncreaseListLengthBoundary();
	TestHeadInsertDeleteStress();

	printf("\n==== summary: %s (%d failed) ====\n",
		g_failed == 0 ? "ALL PASSED" : "SOME FAILED", g_failed);
	return g_failed == 0 ? 0 : 1;
}
