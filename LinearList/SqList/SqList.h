#pragma once

#include<stdlib.h>
typedef int ElemType;

constexpr int InitSize = 100;  //初始化时顺序表最大长度
//#define InitSize 100;  //初始化时顺序表最大长度
/*
问题出在 InitSize 的宏定义里：
你在 #define InitSize 100; 后面多了一个分号。
宏展开会把分号也带进代码，
导致像 malloc(InitSize * sizeof(ElemType)) 被展开成 malloc(100; * sizeof(ElemType))，
这在语法上是非法的。
*/

typedef struct SqList {  //定义顺序表
	int MaxSize = 0;  //顺序表最大长度
	ElemType* data = nullptr;  //顺序表数组头指针
	int len = 0;  //顺序表当前长度
	//C++20 改成"没有 user-declared 构造函数" —— = delete 也算 declared。于是 SqList 不再是一个聚合体。
	//C+20只要有任何一个构造函数被显式声明，编译器就不再隐式生成默认构造函数
	SqList() = default;  //一并写出构造函数
	~SqList() { free(data); }
	SqList(const SqList&) = delete;  // 不许拷贝构造
	SqList& operator=(const SqList&) = delete;  // 不许赋值
}SqList;

//接口函数的函数声明
bool InitList(SqList& L);
bool GetElem(const SqList& L, int pos, ElemType& e);
int LocateElem(const SqList& L, ElemType e);
bool IncreaseListLength(SqList& L, int extra_size);
//bool ListInsert(SqList& L, ElemType e, int pos);
//要及时更改函数声明
bool ListInsert(SqList& L, int pos, ElemType e);
bool ListDelete(SqList& L, int pos, ElemType& e);
