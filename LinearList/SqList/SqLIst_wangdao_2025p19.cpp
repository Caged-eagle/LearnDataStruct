#include <stdio.h>
#include <stdlib.h>
#include "SqList.h"

//==================================================================
// 王道《数据结构》p19 综合应用题 01~08
// 约定：每题上方保留题目原文；带"注意 / 容易出错"的是调试时的踩坑记录
//==================================================================

//01.从顺序表中删除具有最小值的元素（假设唯一）并由函数返回被删元素的值。空出的位置由最后一个元素填补，若顺序表为空，则显示出错信息并退出运行。
bool DeleteMin(SqList& L, ElemType& e) {
	if (0 == L.len) {
		printf("顺序表为空。");
		return false;
	}
	//else if (1 == L.len)
	//{
	//	e = L.data[0];
	//	L.len--;
	//	return true;
	//}
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
	for (int i = 0; i < L.len / 2; i++) {
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
	int cnt = 0;  //记录表中x的个数
	//改为双指针
	for (int i = 0; i < L.len; i++) {
		if (x == L.data[i]) {
			cnt++;
		}
		else
		{
			L.data[i - cnt] = L.data[i];
		}

	}
	L.len -= cnt;
	return true;
}



//04.从顺序表中删除其值在给定值s和t之间（包含s和t,要求s<t）的所有元素，若s或t不合理或顺序表为空，则显示出错信息并退出运行。
bool DeleteBetweenSAndT(SqList& L, ElemType s, ElemType t) {
	//初步判断
	if (s >= t) {
		printf("s或t不合理。");
		return false;
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
	//同03改为双指针
	for (int i = 0; i < L.len - 1; i++) {
		if (L.data[i] == L.data[i + 1]) {
			cnt++;
		}
		else {
			L.data[i - cnt] = L.data[i];
		}
	}
	L.data[L.len - 1 - cnt] = L.data[L.len - 1];
	L.len -= cnt;  //注意表长的更改
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
			merged_L.len += 2;
		}
		else if (L1.data[i] < L2.data[j]) {
			merged_L.data[k] = L1.data[i];
			k++;
			i++;
			merged_L.len++;
		}
		else if (L1.data[i] > L2.data[j]) {
			merged_L.data[k] = L2.data[j];
			k++;
			j++;
			merged_L.len++;
		}
	}
	while (i < L1.len) {
		merged_L.data[k] = L1.data[i];
		k++;
		i++;
		merged_L.len++;
	}
	while (j < L2.len) {
		merged_L.data[k] = L2.data[j];
		k++;
		j++;
		merged_L.len++;
	}
	return true;
}

//07.已知在一维数组A[m+n]中依次存放两个线性表(a1, a2, …, am)和(b1, b2, …, bn)。
//   编写一个函数，将数组中两个顺序表的位置互换，
//   即将(b1, b2, …, bn)放在(a1, a2, …, am)的前面。

//---- 旧实现（O(n) 空间，保留备查）：借助辅助数组完成两块互换 ----
//bool PositionExchange(SqList& L, int m, int n) {
//	if (L.len < 2 || m < 1 || n < 1 || m + n != L.len) {  //增加判断m+n是否和L.len相等
//		return false;
//	}
//	ElemType* temp_data = (ElemType*)malloc(L.maxsize * sizeof(ElemType));
//	if (nullptr == temp_data) {                           //增加malloc判空
//		return false;
//	}
//	for (int i = 0; i < n; i++) {
//		temp_data[i] = L.data[i + m];
//	}
//	for (int i = 0; i < m; i++) {
//		temp_data[i + n] = L.data[i];
//	}
//	free(L.data);
//	L.data = temp_data;
//	return true;
//}

//---- 新实现：三次逆置，空间复杂度 O(1) ----
//单独写一个区间逆置函数，pos_low / pos_high 传入的是位序（从 1 开始）。
//注意 pos_low 允许等于 pos_high：区间为空，什么都不做直接返回 true；
//只有 pos_low > pos_high 才视为非法。
static bool ListReverseFromLoToHi(SqList& L, int pos_low, int pos_high) {
	if (pos_low > pos_high || pos_low<1 || pos_high>L.len) {
		return false;
	}
	ElemType temp_e = L.data[pos_low - 1];
	for (int i = pos_low - 1; i < pos_low + (pos_high - pos_low) / 2; i++) {
		temp_e = L.data[i];
		L.data[i] = L.data[pos_high - 1 + pos_low - 1 - i];  //注意pos_high - 1 + pos_low - 1 - i才是和i对称的，而不是pos_high-1-i
		L.data[pos_high - 1 + pos_low - 1 - i] = temp_e;
	}
	return true;
}

bool PositionExchange(SqList& L, int m, int n) {
	if (L.len < 2 || m < 1 || n < 1 || m + n != L.len) {  //增加判断m+n是否和L.len相等
		return false;
	}
	if (ListReverseFromLoToHi(L, 1, L.len) && ListReverseFromLoToHi(L, 1, n) && ListReverseFromLoToHi(L, n + 1, L.len)) {
		return true;
	}
	else {
		return false;  //注意条件判断语句的添加
	}
}

//08.线性表(a1,a2,a3,…,an)中的元素递增有序且按顺序存储于计算机内。
//要求设计一个算法,完成用最少时间在表中查找数值为x的元素，若找到，则将其与后继元素位置相交换,
//若找不到，则将其插入表中并使表中元素仍递增有序。
bool FindX(SqList& L, ElemType x) {
	if (0 == L.len) {
		if (L.maxsize > 0) {
			L.data[0] = x;
			L.len++;  //注意插入元素更改表长
			return true;
		}
		else
		{
			return false;
		}
	}
	int i = 0, j = L.len - 1, mid = i + (j - i) / 2;  //这么写可以避免i+j超出int范围的问题
	while (i <= j) {
		mid = i + (j - i) / 2;
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
		// 添加判断L.data[mid]是否为末尾元素；
		if (mid == L.len - 1) {
			return true;
		}
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
		//这里容易出错。二分查找到最后最后退出时，i及i的右边必然都大于x，j及j的左边必然都小于x，
		//mid要么等于i，要么等于j，取决于最后一步是i越界（mid=j）还是j越界（mid=i），因此不能用mid来判断插入位置。
		for (int k = L.len - 1; k >= i; k--) {
			L.data[k + 1] = L.data[k];
		}
		L.data[i] = x;
		L.len++;  //注意插入时要让增加L.len
		return true;
	}
}