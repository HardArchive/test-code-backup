// test.cpp : �������̨Ӧ�ó������ڵ㡣
//

#include "stdafx.h"


int _tmain(int argc, _TCHAR* argv[])
{
	int szNum[12] = {0};                                //���1-12���ֵ�����
	int iTem1 = 0, iTem2 = 0, iTem3 = 0, iTem4 = 0;     //��ʱ����
	//��ʼ������ 
	for (int i=0; i<12; i++)
	{
		szNum[i] = i+1;
	}

	for (int i=0; i<12; i++)
	{
		iTem1 = szNum[i];                           //��ȡ��һ����
		for (int j=0; j<12; j++)
		{
			if (iTem1 == szNum[j])
				continue;
			iTem2 = szNum[j];                     //��ȡ�ڶ�����
			for (int k=0; k<12; k++)
			{
				if ((iTem1 == szNum[k])||(iTem2 == szNum[k]))
					continue;
				iTem3 = szNum[k];                //��ȡ��������

				printf("1��12��ѡ��3�����ֵ�%d��:%d%d%d\r\n", ++iTem4, iTem1, iTem2, iTem3);
			}
		}
	}

	getchar();
	
	return 0;
}

