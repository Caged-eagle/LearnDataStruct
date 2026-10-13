#include <stdio.h>
#include <stdlib.h>
#include "SqList2.h"
//---- 辅助函数 ----
static bool IsEqual(ElemType e_1, ElemType e_2) {
	return e_1 == e_2;
}

//---- 接口函数 ----
//初始化顺序表
bool InitList(SqList& L) {
	L.data = (ElemType*)malloc(InitSize * sizeof(ElemType));
	if (nullptr == L.data) {
		return false;
	}
	L.len = 0;
	L.maxsize = InitSize;
	return true;
}

//销毁顺序表
void DestroyList(SqList& L) {
	free(L.data);
	L.data = nullptr;
	L.len = 0;
	L.maxsize = 0;
}

//判断顺序表是否为空
bool ListEmpty(SqList L) {
	return 0 == L.len;
}

//返回顺序表的当前长度
int ListLength(SqList L) {
	return L.len;
}

//返回第i个元素的值
ElemType GetElem(SqList L, int pos, ElemType& e) {
	if (pos<1 || pos>L.len) {
		return false;
	}
	e = L.data[pos - 1];
	return true;
}

//返回第1个与e满足compare关系的位序
int LocateElem(SqList L, ElemType e, bool compare(ElemType, ElemType) = IsEqual) {
	for (int i = 0; i < L.len; i++) {
		if (compare(L.data[i], e)) {
			return i + 1;
		}
	}
	return 0;
}

//在L第pos个位置前插入新的元素
bool ListInsert(SqList& L, int pos, ElemType e) {
	if (pos<1 || pos>L.len + 1) {
		return false;
	}
	if (L.len == L.maxsize) {
		return false;
	}
	for (int i = L.len - 1; i > pos - 1; i--) {
		L.data[i + 1] = L.data[i];
	}
	L.len++;
	return true;
}

