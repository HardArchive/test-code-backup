#include "stdafx.h"

#include <iostream> 
#include <ctime> 
const int SIZE = 100; 
const int MAX = 1000; 
using namespace std; 
//�������� 
void Swap(int &a, int &b) 
{ 
	int temp = a; 
	a = b; 
	b = temp; 
} 

// ð������ 
void BubbleSort(int *arr, int size) 
{ 
	int i, j; 
	for(i=0;i<size-1;i++) 
		for(j=size-1;j>i;j--) 
			if(arr[j] < arr[j-1]) 
				Swap(arr[j], arr[j-1]); 
} 

//ѡ������ 
void SelectionSort(int *arr, int size) 
{ 
	int i, j, min; 
	//�ҳ���a[i]��a[size-1]����СԪ�ص�λ�� 
	for(i=0;i<size-1;i++) 
	{ 
		min = i; 
		for(j=i+1;j<size;j++) 
			if(arr[min] > arr[j]) 
				min = j; 
		//��a[i]��a[min]�����ݽ��� 
		Swap(arr[i], arr[min]); 
	} 
} 

//�������� 
void InsertSort(int *arr, int size) 
{ 
	int fOut, loc, temp; 
	for(fOut=1;fOut<size;fOut++) 
		if(arr[fOut] < arr[fOut-1]) 
		{ 
			temp = arr[fOut]; 
			loc = fOut; 
			do 
			{ 
				arr[loc] = arr[loc-1]; 
				loc--; 
			}while(loc>0 && arr[loc-1]>temp); 
			arr[loc] = temp; 
		} 
} 

//�������� 
int Partition(int *arr, int first, int last) 
{ 
	int i, small, x; 
	//Ϊ�˼����������ĳ���Ƶ�ʶ�����һ���Ż� 
	swap(arr[first], arr[ (first+last)/2 ]); 
	x = arr[first]; 
	small = first; 
	for(i=first+1;i<=last;i++) 
		if(arr[i] < x) 
		{ 
			small++; 
			swap(arr[small], arr[i]); 
		} 
		swap(arr[first], arr[small]); 
		return small; 
} 
void RecQuick(int *arr, int first, int last) 
{ 
	int pivotLoc; 
	if(first < last) 
	{ 
		pivotLoc = Partition(arr, first, last); 
		RecQuick(arr, first, pivotLoc-1); 
		RecQuick(arr, pivotLoc+1, last); 
	} 
} 
void QuickSort(int *arr, int size) 
{ 
	RecQuick(arr, 0, size-1); 
} 
//�������� 
void CountSort(int *arr, int size) 
{ 
	int temp[MAX] = {0}; 
	int i, j; 
	for(i=0;i<size;i++) 
		temp[arr[i]]++; 
	j = 0; 
	for(i=0;i<MAX;i++) 
	{ 
		while(0 != temp[i]) 
		{ 
			arr[j] = i; 
			temp[i]--; 
			j++; 
		} 
	} 
} 
//�鲢���� 
void Merge(int *arr, int start, int mid, int end) 
{ 
	int temp1[SIZE], temp2[SIZE]; 
	int n1, n2; 
	int i, j, k; 
	n1 = mid - start + 1; 
	n2 = end - mid; 
	//����ǰ�벿������ 
	for(i=0;i<n1;i++) 
		temp1[i] = arr[start + i]; 
	//������벿������ 
	for(i=0;i<n2;i++) 
		temp2[i] = arr[mid + i + 1]; 
	//�Ѻ����Ԫ�����õĺܴ� 
	temp1[n1] = temp2[n2] = INT_MAX; 
	i = j = 0; 
	//  ���ɨ������������Ȼ��ŵ���Ӧ��λ��ȥ 
	for(k=start;k<=end;k++) 
	{ 
		if(temp1[i] <= temp2[j]) 
		{ 
			arr[k] = temp1[i]; 
			i++; 
		} 
		else 
		{ 
			arr[k] = temp2[j]; 
			j++; 
		} 
	} 
} 
void RecMerge(int *arr, int start, int end) 
{ 
	int i; 
	if(start < end) 
	{ 
		i = (start + end) / 2; 
		RecMerge(arr, start, i); 
		RecMerge(arr, i+1, end); 
		Merge(arr, start, i, end); 
	} 
} 
void MergeSort(int *arr, int size) 
{ 
	RecMerge(arr, 0, size-1); 
} 
//������ 
void Heapify(int *arr, int low, int high) 
{ 
	int large; 
	int temp = arr[low]; 
	large = 2 * low + 1; 
	while(large <= high) 
	{ 
		if(large<high && arr[large]<arr[large+1]) 
			large = large + 1; 
		if(temp > arr[large]) 
			break; 
		else 
		{ 
			arr[low] = arr[large]; 
			low = large; 
			large = 2 * low + 1; 
		} 
	} 
	arr[low] = temp; 
} 
void BuildHeap(int *arr, int size) 
{ 
	int i; 
	for(i=size/2-1;i>=0;i--) 
		Heapify(arr, i, size-1); 
} 
void HeapSort(int *arr, int size) 
{ 
	int i;                  //lastOfOrder 
	BuildHeap(arr, size); 
	for(i=size-1;i>=0;i--) 
	{ 
		swap(arr[0], arr[i]); 
		Heapify(arr, 0, i-1); 
	} 
} 
//ϣ������ 
void ShellSort(int *arr, int size) 
{ 
	int i, j, k, temp; 
	//i Ϊ���� 
	for(i=size/2;i>0;i/=2) 
	{ 
		for(j=i;j<size;j+=i) 
		{ 
			temp = arr[j]; 
			k = j; 
			while(k-i>=0 && temp<arr[k-i]) 
			{ 
				arr[k] = arr[k-i]; 
				k -= i; 
			} 
			arr[k] = temp; 
		} 
	} 
} 
//�������������� 
void Print(int *arr, int size) 
{ 
	int i; 
	for(i=0;i<size;i++) 
	{ 
		printf("%6d ", arr[i]); 
		if(0 == (i+1) % 10) 
			cout << endl; 
	} 
	cout << endl; 
} 
//������ 
//�����ѡ��100 ���������ݷ��������У������ 
//Ȼ�����С�������򣬲���� 
int main() 
{ 
	int arr[SIZE], i; 
	cout << "The array is: " << endl; 
	srand((unsigned)time(0)); 
	for(i=0;i<SIZE;i++) 
		arr[i] = rand() % MAX; 
	Print(arr, SIZE); 
	//��ѡ������һ�������㷨�������иó��� 
	//BubbleSort(arr, SIZE); 
	//SelectionSort(arr, SIZE); 
	//CountSort(arr, SIZE); 
	//InsertSort(arr, SIZE); 
	//SelectionSort(arr, SIZE); 
	//QuickSort(arr, SIZE); 
	//MergeSort(arr, SIZE); 
	//HeapSort(arr, SIZE); 
	ShellSort(arr, SIZE); 
	cout << "After sorting, the array is:" << endl; 
	Print(arr, SIZE); 
	return 0; 
} 

