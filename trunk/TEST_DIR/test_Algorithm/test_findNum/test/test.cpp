// test.cpp : �������̨Ӧ�ó������ڵ㡣
//

#include "stdafx.h"

bool FindNum1()
{
	//��һ�ַ�����,�ȶ������������
	//Ȼ������
	
	int oldArr[7] = {1, 5, 7, 6, 4, 8, 10};
	int n = 10, k=0;  
	int newAry[10] = {0};  

	//��oldArr��������  ����㷨û��

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
	int oldArr[7] = {5,1,6,3,7,8,10};//ȱʧ��Դ����997������  
	int newArr[10] = {0};  
	int lostArr[3] = {0} ;//Ҫ�ҵ���������  
	//���������е�����ԭ����Ϊ�±긳ֵΪһ
	for(int i = 0; i < n-3; i++) 
	{  
		newArr[oldArr[i]-1] = 1;  
	}  
	int i=0;
	//������������ֵ��Ϊ1�ļ�Ϊȱʧ����
	for(int j = 0; j < n; j++) 
	{  
		if(!newArr[j]) 
		{  
			lostArr[i++] = j+1;  
		}  
	}  

	//��ӡ����
	printf("�ҵ������飺");
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

