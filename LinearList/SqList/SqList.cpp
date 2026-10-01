#include <stdlib.h>
#include "SqList.h"

//---- 辅助函数 ----
//访问位置合法
static bool IsAccessPos(const SqList& L, int pos) {
	return pos >= 1 && pos <= L.len;
}

//修改位置合法
static bool IsInsertPos(const SqList& L, int pos) {
	return pos >= 1 && pos <= L.len + 1;  //注意插入时可以插入到末尾
}

//---- 接口函数 ----
//初始化顺序表，重复调用 = 清空重置
bool InitList(SqList& L) {
	ElemType* new_data = (ElemType*)malloc(InitSize * sizeof(ElemType));  //分配InitSize大小内存
	if (nullptr == new_data) {
		return false;
	}
	//防止重复初始化导致内存泄漏：若已分配过，先释放旧空间
	if (L.data != nullptr) {
		free(L.data);
		L.data = nullptr;
	}
	L.data = new_data;
	L.MaxSize = InitSize;
	L.len = 0;  //顺序表当前长度置为0

	////清除脏数据，但应该省略，减少不必要开销
	//for (int i = 0; i < L.MaxSize; i++) {
	//	L.data[i] = 0;
	//}
	return true;
}

//按位查找
bool GetElem(const SqList& L, int pos, ElemType& e) {
	if (!IsAccessPos(L, pos)) {
		return false;
	}
	e = L.data[pos - 1];
	return true;
}

//按值查找
int LocateElem(const SqList& L, ElemType e) {
	for (int i = 0; i < L.len; i++) {
		if (e == L.data[i]) {
			return i + 1;
		}
	}
	return -1;  //返回-1表示没找到
}

//增加顺序表最大长度
bool IncreaseListLength(SqList& L, int extra_size) {
	//首先要判断extra_size的合法性
	if (extra_size < 0) {
		return false;
	}
	//extra_size为0时无需扩容，直接返回，避免无意义的分配-复制-释放
	if (extra_size == 0) {
		return true;
	}

	//extra_size为0时至少扩容1个单位
	//if (extra_size == 0) {
	//	extra_size += 1;
	//}
	//用临时指针分配空间
	ElemType* temp_data = (ElemType*)malloc((L.MaxSize + extra_size) * sizeof(ElemType));
	//判断是否分配成功
	if (nullptr == temp_data)
	{
		return false;
	}
	//转移数据
	for (int i = 0; i < L.len; i++) {
		temp_data[i] = L.data[i];
	}
	free(L.data);
	L.data = temp_data;
	L.MaxSize += extra_size;
	return true;
}

//指定位置插入元素（前插）
bool ListInsert(SqList& L, int pos, ElemType e) {
	//判断插入位置是否合法
	if (!IsInsertPos(L, pos)) {
		return false;
	}
	//判断顺序表是否需要扩容
	if (L.len >= L.MaxSize) {
		//让扩容兜住底：ListInsert 里传的 extra_size 改成 L.MaxSize > 0 ? L.MaxSize : InitSize，
		// 这样 MaxSize == 0 时也能真正开出空间，插入照样成功。
		if (!IncreaseListLength(L, L.MaxSize > 0 ? L.MaxSize : InitSize)) {

			return false;
		}
	}
	//pos及后面所有元素右移一位
	for (int i = L.len; i > pos - 1; i--) {
		L.data[i] = L.data[i - 1];
	}
	L.data[pos - 1] = e;
	L.len++;  //注意更改表长
	return true;
}

//删除元素
bool ListDelete(SqList& L, int pos, ElemType& e) {
	if (!IsAccessPos(L, pos)) {
		return false;
	}
	e = L.data[pos - 1];
	for (int i = pos - 1; i < L.len - 1; i++) {
		L.data[i] = L.data[i + 1];
	}
	L.len--;  //注意不能忘记更改表长
	return true;
}