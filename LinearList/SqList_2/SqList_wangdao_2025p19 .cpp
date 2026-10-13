#include <stdio.h>
#include <stdlib.h>
#include "SqList2.h"

//01.从顺序表中删除具有最小值的元素（假设唯一）并由函数返回被删元素的值。空出的位置由最后一个元素填补，若顺序表为空，则显示出错信息并退出运行。
bool DeleteMin(SqList& L, ElemType& e) {
	if (0 == L.len) {
		printf("顺序表为空。");
		return false;
	}
	else if (1 == L.len)
	{
		e = L.data[0];
		L.len--;
		return true;
	}
	int temp_idx = 0;
	e = L.data[0];
	for (int i = 0; i < L.len; i++) {
		if (L.data[i] < L.data[temp_idx]) {
			temp_idx = i;
		}
	}
	e = L.data[temp_idx];
	L.data[temp_idx] = L.data[L.len - 1];
	L.len--;
	return true;
}

//02.设计一个高效算法，将顺序表L的所有元素逆置，要求算法的空间复杂度为O(1)。
bool ListReverse(SqList& L) {
	if (0 == L.len) {
		return false;
	}
	ElemType temp_e = L.data[0];
	for (int i = 0; i < L.len / 2 - 1; i++) {
		temp_e = L.data[i];
		L.data[i] = L.data[L.len - 1 - i];
		L.data[L.len - 1 - i] = temp_e;
	}
	return true;
}

//03.对长度为n的顺序表L，编写一个时间复杂度为O(n)、空间复杂度为O(1)的算法，该算法删除顺序表中所有值为x的数据元素。
bool DeleteX(SqList& L, ElemType x) {
	if (0 == L.len) {
		return false;
	}
	//如果表中只有一个元素且为x
	else if (1 == L.len; x == L.data[0]) {
		L.len--;
		return true;
	}
	int cnt = 0;  //记录表中x的个数
	for (int i = 0; i < L.len; i++) {
		if (x == L.data[i]) {
			cnt++;
			L.len--;
		}
		L.data[i] = L.data[i + cnt];
	}
	return true;
}

//04.从顺序表中删除其值在给定值s和t之间（包含s和t,要求s<t）的所有元素，若s或t不合理或顺序表为空，则显示出错信息并退出运行。
bool DeleteBetweenSAndT(SqList& L, ElemType s, ElemType t) {
	//初步判断
	if (s >= t) {
		printf("s或t不合理。");
	}
	else if (0 == L.len) {
		printf("顺序表为空。");
		return false;
	}
	//如果表中只有一个元素且为在区间内
	else if (1 == L.len && L.data[0] >= s && L.data[0] <= t) {
		L.len--;
		return true;
	}
	int cnt = 0;  //记录表中区间内元素的个数
	for (int i = 0; i < L.len; i++) {
		L.data[i] = L.data[i + cnt];
		if (L.data[i] >= s && L.data[i] <= t) {
			cnt++;
			L.len--;
			i--;
		}

	}
	return true;
}

//05.从有序顺序表中删除所有其值重复的元素，使表中所有元素的值均不同。
bool ListDeduplicate(SqList& L) {
	if (0 == L.len) {
		return false;
	}
	int cnt = 0;//记录重复元素的个数
	for (int i = 0; i < L.len; i++) {
		L.data[i] = L.data[i + cnt];
		if (L.data[i] == L.data[i + cnt + 1]) {
			cnt++;
			i--;
			L.len--;
		}
	}
	return true;
}

//06.将两个有序顺序表合并为一个新的有序顺序表，并由函数返回结果顺序表。
bool ListMerge(SqList& L1, SqList& L2, SqList& merged_L) {
	//判断L1L2是否为空以及merged_L是否满足容量要求
	if (0 == L1.len || 0 == L2.len || merged_L.maxsize < L1.len + L2.len) {
		return false;
	}
	int i = 0, j = 0, k = 0;
	while (i < L1.len && j < L2.len) {
		if (L1.data[i] == L2.data[j])
		{
			merged_L.data[k] = L1.data[i];
			k++;
			i++;
			merged_L.data[k] = L2.data[j];
			k++;
			j++;
		}
		else if (L1.data[i] < L2.data[j]) {
			merged_L.data[k] = L1.data[i];
			k++;
			i++;
		}
		else if (L1.data[i] > L2.data[j]) {
			merged_L.data[k] = L1.data[j];
			k++;
			j++;
		}
	}
	while (i < L1.len) {
		merged_L.data[k] = L1.data[i];
		k++;
		i++;
	}
	while (j < L1.len) {
		merged_L.data[k] = L1.data[j];
		k++;
		j++;
	}
	return true;
}

//07.已知在一维数组A[m+n]中依次存放两个线性表(a1,a2,a3,…`,am)和(b1,b2,b3,…,b,)。
//编写一个函数，将数组中两个顺序表的位置互换，即将(b1, b2, b3, …, b, )放在(a1, ai, a3, …, am)的前面。
bool PositionExchange(SqList& L, int m, int n) {
	if (L.len < 2 || m < 1 || n < 1) {
		return false;
	}
	ElemType* temp_data = (ElemType*)malloc(L.maxsize * sizeof(ElemType));
	for (int i = 0; i < n; i++) {
		temp_data[i] = L.data[i + m];
	}
	for (int i = 0; i < m; i++) {
		temp_data[i + n] = L.data[i];
	}
	return true;
}

//08.线性表(a1,a2,a3,…,an)中的元素递增有序且按顺序存储于计算机内。
//要求设计一个算法,完成用最少时间在表中查找数值为x的元素，若找到，则将其与后继元素位置相交换,
//若找不到，则将其插入表中并使表中元素仍递增有序。
bool FindX(SqList& L, ElemType x) {
	int i = 0, j = L.len - 1, mid = (i + j) / 2;
	while (i <= j) {
		mid = (i + j) / 2;
		if (x < L.data[mid]) {
			j = mid - 1;
		}
		else if (x > L.data[mid]) {
			i = mid + 1;
		}
		else {
			break;
		}
	}
	if (x == L.data[mid]) {
		ElemType temp_data = L.data[mid];
		L.data[mid] = L.data[mid + 1];
		L.data[mid + 1] = temp_data;
		return true;
	}
	else
	{
		if (L.maxsize == L.len) {
			return false;
		}
		for (int i = L.len - 1; i >= mid; i--) {
			L.data[i + 1] = L.data[i];
		}
		L.data[mid] = x;
		return true;
	}
}