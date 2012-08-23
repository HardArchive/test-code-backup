#include "stdafx.h"
#include<stdio.h>
#include<Windows.h>
#include<tchar.h>

	//获取系统所有磁盘驱动器
void main(int argc,char* argv[])
{
	char strRootPath[] = "c:\\";    //带根目录标记的磁盘符号
	DWORD dwSectorsPerCluster = 0;   //每簇中扇区数
	DWORD dwBytesPerSector = 0;     //每扇区中字节数
	DWORD dwFreeClusters = 0;       //剩余簇数
	DWORD dwTotalClusters = 0;      //总簇数

	DWORD dwDrivers = GetLogicalDrives(); //获取所有磁盘驱动器
	int n = 0;
	int k = 0;

	//char chDriverTest = 'A';//起始值为A
	char chDriverTest = 'C';   //起始值为C
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
				printf("盘符\t容量\t\t已用空间\t可用空间\r\n"); 
			}
			//sprintf(szBuffer, "%d",number);
			sprintf(szBuffer, "%c 盘\t", chDriverTest);
			printf(szBuffer);

			//获取磁盘剩余容量大小 

			strRootPath[0] = chDriverTest;

			ZeroMemory(szBuffer, sizeof(szBuffer));
			//获取与一个磁盘的组织有关的信息，以及了解剩余空间的容量
			if (GetDiskFreeSpace((LPCSTR)strRootPath, &dwSectorsPerCluster, &dwBytesPerSector,
				&dwFreeClusters, &dwTotalClusters))
			{
				//double m_dwVolSize = dwTotalClusters*dwSectorsPerCluster*dwBytesPerSector;//不能这样，否则越界

				double dd = dwSectorsPerCluster*dwBytesPerSector;  //每簇的字节数
				dd = dd/(1024.0*1024.0*1024.0);  //单位GB

				double m_dFree = dwFreeClusters*dd;       //该磁盘剩余容量总大小 GB
				double m_dwVolSize = dwTotalClusters*dd;   //总大小 GB
				double m_dUsed = m_dwVolSize-m_dFree;      //已用大小 GB

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
				sprintf(szBuffer,"%s","不能获取磁盘容量\n");
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