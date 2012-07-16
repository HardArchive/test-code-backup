// test.cpp : �������̨Ӧ�ó������ڵ㡣
//

#include "stdafx.h"
#include <Windows.h>

//�����༶Ŀ¼���粻�����򴴽� ʧ�ܷ���false����������true
bool CreatePath(char* pDirPath)
{
	int iNum = 0;
	char* pDir = NULL;
	char szParentDir[MAX_PATH] = {0};
	char szTempDir[MAX_PATH] = {0};

	//����·���Ƿ����
	if (GetFileAttributes(pDirPath) == FILE_ATTRIBUTE_DIRECTORY)
	{
		return TRUE;
	}

	pDir = (char*)strrchr(pDirPath, '\\');  //��β�������ַ�
	iNum = strlen(pDir);
	if (1 == iNum)
	{
		strncpy(szTempDir, pDirPath, strlen(pDirPath)-1);  //��Ŀ¼��
		pDir = (char*)strrchr(szTempDir, '\\');
		//pwDir += 1;
		iNum += strlen(pDir);
	}

	strncpy(szParentDir, pDirPath, strlen(pDirPath)-iNum);

	//���鸸��·���Ƿ����  ��������򴴽���Ŀ¼
	if (GetFileAttributes(szParentDir) == FILE_ATTRIBUTE_DIRECTORY)
	{		
		if (!CreateDirectory(pDirPath, NULL))
		{
			//�����ļ���ʧ��
			//FindClose(hParentFile);
			::MessageBox(NULL, pDirPath, "�����ļ���ʧ��", MB_OK);
			return false;
		}
		else
		{
			SetFileAttributes(pDirPath, FILE_ATTRIBUTE_NORMAL);	
			return true;
		}
	}
	else
	{
		//�����������ݹ�
		if (!CreatePath(szParentDir))
		{
			return false;
		}
		else
		{//��Ŀ¼��������򴴽���Ŀ¼
			if (!CreatePath(pDirPath))
			{
				return false;
			}
			return true;
		}
	}

	return true;
}

bool CopyFolder(char* pExistingFolderPath, char* pNewFolderPath)
{
	int iRet = 0;
	int iFileNum = 0;   //��ǰ�ļ������ļ����� Ϊ��ֵʱ��������
	HANDLE hFind = NULL;
	WIN32_FIND_DATA stuWFD = {0};	
	char szFullPath[MAX_PATH] = {0};
	char szNewFullPath[MAX_PATH] = {0};
	char szFindFilter[MAX_PATH] = {0};
	DWORD dwAttributes = 0;

	if (!(pExistingFolderPath && pNewFolderPath))
	{
		return -1;
	}
	//��鴴�����ļ���·��
	if (!CreatePath(pNewFolderPath))
	{
		return -2;
	}

	sprintf(szFindFilter, "%s\\*.*", pExistingFolderPath);    //�����ļ�����

	if ((hFind = FindFirstFile(szFindFilter, &stuWFD)) == INVALID_HANDLE_VALUE)
	{
		return -3;
	}

	do
	{
		//���˱���Ŀ¼�͸�Ŀ¼
		if(_tcscmp(stuWFD.cFileName, _T(".")) == 0 || _tcscmp(stuWFD.cFileName, _T("..")) == 0)
			continue;

		//����ҵ�����Ŀ¼��������Ŀ¼���еݹ�
		if(stuWFD.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) 
		{
			ZeroMemory(szFullPath, sizeof(szFullPath));
			sprintf(szFullPath, "%s\\%s", pExistingFolderPath, stuWFD.cFileName);  //�õ��ļ�������·��
			sprintf(szNewFullPath, "%s\\%s", pNewFolderPath, stuWFD.cFileName);    //���ļ�������·��
			if (1 != CopyFolder(szFullPath, szNewFullPath))
			{
				FindClose(hFind);
				return -4;
			}
		}
		else 
		{ 
			//if (!bIsRoot)
			{
				//�ҵ������ļ�
				sprintf(szFullPath, "%s\\%s", pExistingFolderPath, stuWFD.cFileName); //�õ��ļ�����·��
				sprintf(szNewFullPath, "%s\\%s", pNewFolderPath, stuWFD.cFileName);
				if (!CopyFile(szFullPath, szNewFullPath, FALSE))
				{
					FindClose(hFind);
					return -4;
				}
			}
		}	
		//iFileNum++;
	}while (FindNextFile(hFind, &stuWFD));

	FindClose(hFind);

	return 1;
}

int _tmain(int argc, _TCHAR* argv[])
{
	int iCount = 10000;
	int wMinute = 0;
	char szFolderName[MAX_PATH] = {0};
	char szNewFolderPath[MAX_PATH] = {0};
	SYSTEMTIME stuSystemTime = {0};
	//D:\WinPath\desktop\test\dest\201204280922  �Ѵ����ļ���
	//D:\WinPath\desktop\test  Ŀ��λ��
	GetLocalTime(&stuSystemTime);
	
	while(iCount)
	{
		wMinute = stuSystemTime.wMinute;
		while(true)
		{
			if (wMinute+1 == stuSystemTime.wMinute)
			{
				break;
			}
			if (wMinute-59 == stuSystemTime.wMinute)
			{
				break;
			}

			GetLocalTime(&stuSystemTime);
			Sleep(1000);
		}
		sprintf(szFolderName, "%04d%02d%02d%02d%02d", stuSystemTime.wYear, stuSystemTime.wMonth, stuSystemTime.wDay, stuSystemTime.wHour, stuSystemTime.wMinute);
		sprintf(szNewFolderPath, "D:\\WinPath\\desktop\\test\\%s", szFolderName);
		printf(szNewFolderPath);
		printf("\r\n");

		if (1!=CopyFolder("D:\\WinPath\\desktop\\test\\dest\\201204280922", szNewFolderPath))
		{
			printf("�ļ��п���ʧ�ܣ���������");
			printf("\r\n");
			return FALSE;
		}

		iCount--;
	}
	return 0;
}

