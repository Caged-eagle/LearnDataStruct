#define InitSize 100
typedef int ElemType;

//定义顺序表
typedef struct SqList {
	ElemType* data = nullptr;
	int len = 0;
	int maxsize = 0;
}SqList;