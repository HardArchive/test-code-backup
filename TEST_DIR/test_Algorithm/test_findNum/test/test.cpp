// test.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"

bool FindNum1()
{
	//第一种方法是,先对数组进行排序
	//然后再与
	
	int oldArr[7] = {1, 5, 7, 6, 4, 8, 10};
	int n = 10, k=0;  
	int newAry[10] = {0};  

	//对oldArr进行排序  这个算法没加

	for(int i = 1, j=0; i <= n; i ++,j++)
	{  
		int diff = oldArr[j] - i;  
		if(!oldArr[j])
		{  
			newAry[k++] = i;
		} 
		else if(diff > 0)
		{  
			for(int k = 0; k < diff; k++)
			{  
				newAry[k++] = i++;     
			}  
		}  
	}  

	return true;
}

bool FindNum2()
{
	int n = 10;
	int oldArr[7] = {5,1,6,3,7,8,10};//缺失的源数组997个数；  
	int newArr[10] = {0};  
	int lostArr[3] = {0} ;//要找的数的数组  
	//将新数组中的数按原数组为下标赋值为一
	for(int i = 0; i < n-3; i++) 
	{  
		newArr[oldArr[i]-1] = 1;  
	}  
	int i=0;
	//所以新数组中值不为1的即为缺失的数
	for(int j = 0; j < n; j++) 
	{  
		if(!newArr[j]) 
		{  
			lostArr[i++] = j+1;  
		}  
	}  

	//打印出来
	printf("找到的数组：");
	for (int k=0; k<i; k++)
	{
		printf("%d\r\n", lostArr[k]);
	}

	return true;
}

int _tmain(int argc, _TCHAR* argv[])
{
	FindNum2();
	getchar();
	return 0;
}

