#include "stdafx.h"
#include<stdio.h>
#include<Windows.h>
#include<tchar.h>

	//��ȡϵͳ���д���������
void main(int argc,char* argv[])
{
	char strRootPath[] = "c:\\";    //����Ŀ¼��ǵĴ��̷���
	DWORD dwSectorsPerCluster = 0;   //ÿ����������
	DWORD dwBytesPerSector = 0;     //ÿ�������ֽ���
	DWORD dwFreeClusters = 0;       //ʣ�����
	DWORD dwTotalClusters = 0;      //�ܴ���

	DWORD dwDrivers = GetLogicalDrives(); //��ȡ���д���������
	int n = 0;
	int k = 0;

	//char chDriverTest = 'A';//��ʼֵΪA
	char chDriverTest = 'C';   //��ʼֵΪC
	char szBuffer[MAX_PATH] = {0};
	bool flag = TRUE;

	for (k = 0;k<26;k++)
	{
		n = dwDrivers&(1<<(chDriverTest-'A'));

		if (n==0)
		{  
			chDriverTest++;
			continue;
		}
		else if(n>0)
		{
			if(flag)
			{ 
				flag = FALSE;
				printf("�̷�\t����\t\t���ÿռ�\t���ÿռ�\r\n"); 
			}
			//sprintf(szBuffer, "%d",number);
			sprintf(szBuffer, "%c ��\t", chDriverTest);
			printf(szBuffer);

			//��ȡ����ʣ��������С 

			strRootPath[0] = chDriverTest;

			ZeroMemory(szBuffer, sizeof(szBuffer));
			//��ȡ��һ�����̵���֯�йص���Ϣ���Լ��˽�ʣ��ռ������
			if (GetDiskFreeSpace((LPCSTR)strRootPath, &dwSectorsPerCluster, &dwBytesPerSector,
				&dwFreeClusters, &dwTotalClusters))
			{
				//double m_dwVolSize = dwTotalClusters*dwSectorsPerCluster*dwBytesPerSector;//��������������Խ��

				double dd = dwSectorsPerCluster*dwBytesPerSector;  //ÿ�ص��ֽ���
				dd = dd/(1024.0*1024.0*1024.0);  //��λGB

				double m_dFree = dwFreeClusters*dd;       //�ô���ʣ�������ܴ�С GB
				double m_dwVolSize = dwTotalClusters*dd;   //�ܴ�С GB
				double m_dUsed = m_dwVolSize-m_dFree;      //���ô�С GB

				sprintf(szBuffer, "%f GB\t", m_dwVolSize);					
				printf(szBuffer);
				ZeroMemory(szBuffer, sizeof(szBuffer));

				sprintf(szBuffer, "%f GB\t", m_dUsed);					
				printf(szBuffer);
				ZeroMemory(szBuffer, sizeof(szBuffer));
				sprintf(szBuffer,"%f GB\t\n", m_dFree);
				printf(szBuffer);
			}        
			else
			{
				sprintf(szBuffer,"%s","���ܻ�ȡ��������\n");
				printf(szBuffer);
			} 

			chDriverTest++;
		}
		else
		{
			return;
		}
	}
	printf("\n");
	system("pause");
}