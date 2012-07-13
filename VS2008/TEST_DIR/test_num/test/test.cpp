// test.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"


int _tmain(int argc, _TCHAR* argv[])
{
	int szNum[12] = {0};                                //存放1-12数字的数组
	int iTem1 = 0, iTem2 = 0, iTem3 = 0, iTem4 = 0;     //临时变量
	//初始化数组 
	for (int i=0; i<12; i++)
	{
		szNum[i] = i+1;
	}

	for (int i=0; i<12; i++)
	{
		iTem1 = szNum[i];                           //获取第一个数
		for (int j=0; j<12; j++)
		{
			if (iTem1 == szNum[j])
				continue;
			iTem2 = szNum[j];                     //获取第二个数
			for (int k=0; k<12; k++)
			{
				if ((iTem1 == szNum[k])||(iTem2 == szNum[k]))
					continue;
				iTem3 = szNum[k];                //获取第三个数

				printf("1到12中选择3个数字第%d个:%d%d%d\r\n", ++iTem4, iTem1, iTem2, iTem3);
			}
		}
	}

	getchar();
	
	return 0;
}

