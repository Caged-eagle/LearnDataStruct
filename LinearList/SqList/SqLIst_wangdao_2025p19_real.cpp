#include <stdio.h>
#include <stdlib.h>
#include "SqList.h"

//==================================================================
// 统考仿真题及真题 09~14（2010 / 2011 / 2013 / 2018 / 2020）
//==================================================================

//09.给定三个序列A、B、C，长度均为n，且均为无重复元素的递增序列，请设计一个时间上尽可能高效的算法，逐行输出同时存在于这三个序列中的所有元素。
//例如，数组A为{1,2,3}，数组B为{2,3,4}，数组C为{-1,0,2}，则输出2。要求:
//1) 给出算法的基本设计思想。
//2）根据设计思想，采用C或 C++语言描述算法，关键之处给出注释。
//3）说明你的算法的时间复杂度和空间复杂度。

//1) 三个指针依次遍历数组并比较大小，发现三个相同元素直接输出。
//2) 见下
//3) 时间复杂度O(n)，空间复杂度O(1)
bool FindSameBetweenThree(SqList& L1, SqList& L2, SqList& L3) {
	if (L1.len != L2.len || L2.len != L3.len) {
		return false;
	}
	int i = 0, j = 0, k = 0;  //i,j,k分别为三个顺序表的指针
	while (i < L1.len && j < L2.len && k < L3.len) {
		if (L1.data[i] < L2.data[j]) {
			i++;
		}
		else if (L1.data[i] > L2.data[j])
		{
			j++;
		}
		else
		{
			if (k < L3.len && L2.data[j] < L3.data[k]) {
				j++;
				continue;
			}
			while (k < L3.len && L2.data[j] > L3.data[k]) {  //注意这里也要有上界，并且是否越界的判断需要放在前面
				k++;
			}
			if (L2.data[j] == L3.data[k]) {
				printf("%d\n", L3.data[k]);
				j++;
				k++;
			}
		}
	}
	return true;
}

//10.【2010统考真题】设将n（n>1）个整数存放到一维数组R中,设计一个在时间和空间两方面都尽可能高效的算法。
//将R中保存的序列循环左移p(0<p<n)个位置，即将R中的数据由（X0, X1, …, Xn-1)变换为（Xp, Xp+1, …, Xn-1, X0, X1, …, Xp-1)。要求:
//1）给出算法的基本设计思想。
//2）根据设计思想，采用C或C++或Java语言描述算法，关键之处给出注释。
//3）说明你所设计算法的时间复杂度和空间复杂度。

//1) 和07题类似，可以进行三次逆置操作。
//2) 见下
//3) 时间复杂度O(n)
static bool ListReverseFromLoToHi(SqList& L, int pos_low, int pos_high) {
	//先判断
	if (pos_low >= pos_high || pos_low<1 || pos_high>L.len) {
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

bool ListMoveForwardP(SqList& L, int p) {  //注意这里的p是左移的位置，按题目要求不能为0，也可以认为是下标
	if (0 == L.len || p<1 || p>L.len - 1) {
		return false;
	}
	if (ListReverseFromLoToHi(L, 1, L.len) && ListReverseFromLoToHi(L, 1, L.len - p) && ListReverseFromLoToHi(L, L.len - p + 1, L.len)) {
		return true;
	}
	else
	{
		return false;
	}
}
//11.【2011统考真题】一个长度为L（L≥1）的升序序列S，处在第 ⌈L/2⌉个位置的数称为S的中位数。
//例如，若序列S1 =(11, 13, 15, 17, 19)，则S1的中位数是15，两个序列的中位数是含它们所有元素的升序序列的中位数。
//例如，若S2 = (2, 4, 6, 8, 20)，则S1和S2的中位数是11。
//现在有两个等长升序序列A和B，试设计一个在时间和空间两方面都尽可能高效的算法，找出两个序列A和B的中位数。要求：
//1) 给出算法的基本设计思想。
//2) 根据设计思想，采用C或C++或Java语言描述算法，关键之处给出注释。
//3) 说明你所设计算法的时间复杂度和空间复杂度。

//1) 对两表分别折半查找并对结果进行比较后确定后续折半查找的方向，最后两表仅剩1个元素时，取较小的一个元素
// 算法有误——每轮把两表的中点一并丢弃，中位数候选可能被扔掉；收尾取较大值属巧合通过。
// 实测等长 3 的 {1,3,5} 与 {2,4,6} 求得 4，期望 3。
// 应按王道标准解法重写：奇数段保留中点、偶数段单侧舍弃中点、两表各剩一个元素时取较小者
//2) 见下
//3) 时间复杂度O(n)，空间复杂度O(1)	
//bool FindMid(SqList L1, SqList L2, ElemType& result) {
//	if (0 == L1.len || 0 == L2.len || L1.len != L2.len) {
//		return false;
//	}
//	int low1 = 0, low2 = 0, high1 = L1.len - 1, high2 = L2.len - 1;
//	int mid1 = low1 + (high1 - low1) / 2, mid2 = low2 + (high2 - low2) / 2;
//	while (low1 < high1 && low2 < high2) {
//		mid1 = low1 + (high1 - low1) / 2;
//		mid2 = low2 + (high2 - low2) / 2;
//		if (L1.data[mid1] < L2.data[mid2]) {
//			low1 = mid1 + 1;
//			high2 = mid2 - 1;
//		}
//		else if (L1.data[mid1] > L2.data[mid2]) {
//			high1 = mid1 - 1;
//			low2 = mid2 + 1;
//		}
//		else
//		{
//			break;
//		}
//	}
//	result = L1.data[mid1] > L2.data[mid2] ? L1.data[mid1] : L2.data[mid2];  //应该取最后两者较大的一个
//	return true;  //bool函数千万要记得返回
//}

//王道写法
//值得注意的是，分奇偶的目的是保证每次分割后两个数组的剩余比较部分长度都相等，这样才能保证L2.data中至多有high2-low2个元素比L1.data[mid1]大或小。
//只有 high1−low1 == high2−low2，才有 mid1−low1 == mid2−low2——两个 mid 才是各自区间里同一个位置上的元素。
bool FindMid(SqList L1, SqList L2, ElemType& result) {
	if (0 == L1.len || 0 == L2.len || L1.len != L2.len) {
		return false;
	}
	int low1 = 0, low2 = 0, high1 = L1.len - 1, high2 = L2.len - 1;
	int mid1 = low1 + (high1 - low1) / 2, mid2 = low2 + (high2 - low2) / 2;
	while (low1 < high1 && low2 < high2) {
		mid1 = low1 + (high1 - low1) / 2;
		mid2 = low2 + (high2 - low2) / 2;
		if (L1.data[mid1] == L2.data[mid2]) {
			result = L1.data[mid1];
			return true;
		}
		if (L1.data[mid1] < L2.data[mid2]) {
			if ((high1 + 1 - low1) % 2 == 0) {
				low1 = mid1 + 1;
				high2 = mid2;
			}
			else
			{
				low1 = mid1;
				high2 = mid2;
			}
		}
		else
		{
			if ((high2 + 1 - low2) % 2 == 0) {
				high1 = mid1;
				low2 = mid2 + 1;
			}
			else
			{
				high1 = mid1;
				low2 = mid2;
			}
		}
	}
	result = L1.data[low1] < L2.data[low2] ? L1.data[low1] : L2.data[low2];
	return true;
}

//12.【2013统考真题】已知一个整数序列 A=(ao,a1,…,am-1)，其中O≤ai<n (0≤i<n)。
//若存在ap1 = ap2 = … = apm = x且m > n / 2(0≤pk < n, 1≤k≤m)，则称x为A的主元素。
//例如A = (0, 5, 5, 3, 5, 7, 5, 5)，则5为主元素；又如A = (0, 5, 5, 3, 5, 1, 5, 7)，则A中没有主元素。
//假设A中的n个元素保存在一个一维数组中，请设计一个尽可能高效的算法，找出A的主元素。若存在主元素，则输出该元素；否则输出 - 1。要求:
//1）给出算法的基本设计思想。
//2）根据设计思想，采用C或C++或Java语言描述算法，关键之处给出注释。
//3）说明你所设计算法的时间复杂度和空间复杂度。

//1）一开始的想法是，相同两个合并成一个,但这是错误的。要么就只能记录每个元素的个数。答案是将不同的元素对消，剩下的一个再重新判断。
//2）见下
//3）时间复杂度O(n)，空间复杂度O(1)
bool FindMainElem(SqList& L, ElemType& result_e) {
	if (L.len < 1) {
		return false;
	}
	int cnt = 0;
	ElemType temp_e = L.data[0];
	for (int i = 1; i < L.len; i++) {
		if (temp_e == L.data[i]) {
			cnt++;
		}
		else
		{
			if (0 == cnt) {
				temp_e = L.data[i];
			}
			else
			{
				cnt--;
			}
		}
	}
	cnt = 0;
	for (int i = 0; i < L.len; i++) {
		if (temp_e == L.data[i]) {
			cnt++;
		}
	}
	if (cnt >= L.len / 2 + 1) {
		result_e = temp_e;
	}
	else
	{
		result_e = -1;
	}
	return true;
}

//13.【2018统考真题】给定一个含n（n≥1）个整数的数组，请设计一个在时间上尽可能高效的算法，找出数组中未出现的最小正整数。
//例如，数组{ -5,3,2,3 }中未出现的最小正整数是1；数组{ 1,2,3 }中未出现的最小正整数是4。要求：
//1) 给出算法的基本设计思想。
//2）根据设计思想，采用C或C++语言描述算法，关键之处给出注释。
//3）说明你所设计算法的时间复杂度和空间复杂度。

//1) 遍历数组并用一个变量存储其最小值，遍历结束后先判断变量是否大于0，若大于零直接输出该变量+1，否则输出1。但这只对题目例子正确，如果为1245，则不正确了。
//因此只能用空间换时间，用一个数组进行标记
//2) 见下
//3) 时间复杂度O(n)，空间复杂度O(n)
bool FindMinPositiveIntNotExist(SqList& L, int& result) {
	if (0 == L.len) {
		return false;
	}
	SqList tag_L;
	tag_L.len = L.len;
	tag_L.maxsize = L.len;
	tag_L.data = (ElemType*)malloc(L.len * sizeof(ElemType));
	if (nullptr == tag_L.data) {
		return false;
	}
	for (int i = 0; i < tag_L.len; i++) {
		tag_L.data[i] = 0;
	}
	for (int i = 0; i < L.len; i++) {
		if (L.data[i] >= 1 && L.data[i] <= L.len) {
			tag_L.data[L.data[i] - 1]++;
		}
	}
	for (int i = 0; i < tag_L.len; i++)
	{
		if (0 == tag_L.data[i]) {
			result = i + 1;
			free(tag_L.data);
			return true;
		}
	}
	result = tag_L.len + 1;
	free(tag_L.data);
	return true;
}

//14.【2020统考真题】定义三元组(a, b, c)(a, b, c均为整数)的距离D = | a - b | + | b - c | + | c - a | 。
//给定3个非空整数集合S1、S2和S3，按升序分别存储在3个数组中。请设计一个尽可能高效的算法，计算并输出所有可能的三元组(a, b, c)(a∈S1, b∈S2, c∈S3)中的最小距离。
//例如S1 = { -1,0,9 }, S2 = { -25,-10,10,11 }, S3 = { 2,9,17,30,41 }，则最小距离为2，相应的三元组为(9, 10, 9)。要求：
//1) 给出算法的基本设计思想。
//2）根据设计思想，采用C语言或C++语言描述算法，关键之处给出注释。
//3）说明你所设计算法的时间复杂度和空间复杂度。

//1) i,j,k三个指针分别遍历三个数组，min_D存储最小距离，遍历时，值最小的元素所在的指针右移。
//2) 见下
//3) 时间复杂度为O(n)，空间复杂度为O(1)

////辅助函数，返回差的绝对值,可省略，直接用abs函数
//static int ReturnAbsoluteValue(int e1, int e2) {
//	return e1 < e2 ? e2 - e1 : e1 - e2;
//}

//辅助函数，计算三元组距离
static int CalcD(int e1, int e2, int e3) {
	return abs(e1 - e2) + abs(e2 - e3) + abs(e1 - e3);
}

//找最小三元组的接口函数
bool FindMinD(SqList& L1, SqList& L2, SqList& L3, int& result_D) {
	if (0 == L1.len || 0 == L2.len || 0 == L3.len) {
		return false;
	}
	int i = 0, j = 0, k = 0, Min_D = CalcD(L1.data[i], L2.data[j], L3.data[k]);
	while (i < L1.len && j < L2.len && k < L3.len) {
		int temp_D = CalcD(L1.data[i], L2.data[j], L3.data[k]);  //可以用局部变量存储减少计算量
		if (temp_D < Min_D) {
			Min_D = temp_D;
		}
		if (L1.data[i] < L2.data[j]) {
			if (L1.data[i] < L3.data[k]) {
				i++;
			}
			else
			{
				k++;
			}
		}
		else
		{
			if (L2.data[j] < L3.data[k]) {
				j++;
			}
			else
			{
				k++;
			}
		}
	}
	result_D = Min_D;
	return true;
}