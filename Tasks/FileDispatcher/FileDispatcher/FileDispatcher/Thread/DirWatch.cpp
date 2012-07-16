#include "StdAfx.h"
#include "DirWatch.h"
//
//#include <Shlwapi.h>
//#pragma comment(lib, "Shlwapi.lib")  
#include "Queue.h"
using namespace QNA;
//����ʹ�ó�ʼ��Ϊȫ�ֳ�Ա
CQueue CQueue::m_instance;

CDirWatch::CDirWatch(void)
{
	ZeroMemory(m_szWatchDirPath, sizeof(m_szWatchDirPath));
	ZeroMemory(m_szDestDirPath, sizeof(m_szDestDirPath));
	ZeroMemory(m_szDestFilePath, sizeof(m_szDestFilePath));
	m_bIsStoped = false;
}
CDirWatch::CDirWatch(char* pWatchDirPath, char* pDestDirPath, char* pDestFilePath)
{
	ZeroMemory(m_szWatchDirPath, sizeof(m_szWatchDirPath));
	ZeroMemory(m_szDestDirPath, sizeof(m_szDestDirPath));
	ZeroMemory(m_szDestFilePath, sizeof(m_szDestFilePath));
	strcpy(m_szWatchDirPath, pWatchDirPath);
	strcpy(m_szDestDirPath, pDestDirPath);
	strcpy(m_szDestFilePath, pDestFilePath);
	m_bIsStoped = true;
}
CDirWatch::~CDirWatch(void)
{
	m_bIsStoped = false;
	ZeroMemory(m_szWatchDirPath, sizeof(m_szWatchDirPath));
	ZeroMemory(m_szDestDirPath, sizeof(m_szDestDirPath));
	ZeroMemory(m_szDestFilePath, sizeof(m_szDestFilePath));
}

//�ж��ļ������Ƿ�Ϊ���� �Ƿ���true
bool CDirWatch::CheckFileIsHide(char* pFileName)
{
	DWORD dwAttribute = 0;
	if (!pFileName)
	{
		return false;
	}
	dwAttribute = GetFileAttributes(pFileName);
	if (!(FILE_ATTRIBUTE_HIDDEN & dwAttribute))
	{
		return false;
	}
	return true;
}

//��ָ���ļ���������Ŀ��Ŀ¼��Ȼ��ɾ��ԭ�ļ�
int CDirWatch::DealFile(char* pFileName)
{
	char* pFName = NULL;
	char szParentName[16] = {0};
	char szCopyFileName[MAX_PATH] = {0};
	if (!pFileName)
	{
		return -1;
	}

	//����Ƿ�Ϊdat�ļ�
	pFName = (char*)strrchr(pFileName, '.');  //��β�������ַ�  �����ļ���ʽ
	if (strcmp(pFName, ".dat"))
	{
		return -2;
	}
	pFName = NULL;
	
	//��ȡ��Ŀ¼��
	if (!GetParentDirName(pFileName, szParentName))
	{
		return -3;
	}

	pFName = (char*)strrchr(pFileName, '\\')+1;  //��β�������ַ�  �����ļ���

	if (!strcmp(pFName, "IMMsgLog.dat"))
	{
		sprintf(szCopyFileName, "%s%s\\", m_szDestFilePath, szParentName);
		if (!CreatePath(szCopyFileName))
		{
			return -4;
		}
		strcat(szCopyFileName, "IMMsgLog.dat");
		if (!CopyFile(pFileName, szCopyFileName, false))
		{
			return -5;
		}
	}

	sprintf(szCopyFileName, "%s%s\\", m_szDestDirPath, szParentName);
	if (!CreatePath(szCopyFileName))
	{
		return -6;
	}

	strcat(szCopyFileName, pFName);

	if (!CopyFile(pFileName, szCopyFileName, false))
	{
		return -7;
	}
	
	//Sleep(15);
	//1�����ļ�����m_szDestFilePathĿ¼�������� �ļ��������ļ���
	//2��ȡ���ļ������ļ�����
	// ��m_szDestDirPath����Ƿ����ļ��д��ڣ��������򴴽�
	// ���ļ�����m_szDestDirPath���ļ�����
	// 
	//DeleteFile(pFileName);
	return 1;
}

void CDirWatch::DeleteDir(char* pDirPath)
{
	//SHFILEOPSTRUCT FileOp;
	//ZeroMemory((void*)&FileOp,sizeof(SHFILEOPSTRUCT));

	////�������FOF_SILENT��־����ô�ں������õ�ʱ�򣬼�ʹ����Ҳ���ᵯ����ʾ����������Ŀ¼�ǹ���ģ���Ȼ���ǻᵯ����ʾ
	//FileOp.fFlags = FOF_SILENT|FOF_NOCONFIRMATION;
	//FileOp.hNameMappings = NULL;
	//FileOp.hwnd = NULL;
	//FileOp.lpszProgressTitle = NULL;
	//FileOp.pFrom = pDirPath;
	//FileOp.pTo = NULL;
	//FileOp.wFunc = FO_DELETE;
	//SHFileOperation(&FileOp);
	// 

	char str[MAX_PATH] ="RMDIR /S/Q "; //���� /Q  ɾ��ʱ��Ҫ��ȷ��
	strcat(str, pDirPath);
	system(str);
}

//����ļ������ͣ��ж��Ƿ���������(eg:201205070945)���ļ���
bool CDirWatch::CheckDirType(char* pDirPath)
{
	long dwNname = 0;
	char szTem[16] ={0};
	int wYear = 0;
	int wMonth = 0;
	int wDay = 0;
	int wHour = 0;
	int wMinute = 0;
	int iMonth = 0, iDay = 0;
	SYSTEMTIME stuSystemTime = {0};
	if (!pDirPath)
	{
		return false;
	}

	if (12 != strlen(pDirPath))
	{
		return false;
	}

	strncpy(szTem, pDirPath, 4);  //�������
	//dwNname =  strtoul(szTem, NULL, 10);
	wYear = atol(szTem);  //eg 2012 03 04 12 58
	if (wYear<1000||wYear>5000)
	{
		return false;    //��2000�굽3000��֮����ų�
	}
	ZeroMemory(szTem, 5);
	strncpy(szTem, pDirPath+4, 2);//��
	wMonth = atol(szTem);  
	if (wMonth>12||wMonth<1)
	{
		return false;
	}

	strncpy(szTem, pDirPath+4+2, 2); //��
	wDay = atol(szTem); 
	if (wDay>31||wDay<1)
	{
		return false;
	}
	strncpy(szTem, pDirPath+4+2+2, 2); //ʱ
	wHour = atol(szTem); 
	if (wHour>24||wHour<0)
	{
		return false;
	}

	strncpy(szTem, pDirPath+4+2+2+2, 2); //ʱ
	wMinute = atol(szTem); 
	if (wMinute>60||wMinute<0)
	{
		return false;
	}
	//GetLocalTime(&stuSystemTime);
	////GetSystemTime(&stuSystemTime);
	//wYear = stuSystemTime.wYear - wYear;       //������֮��Ĳ�����Ϊ1
	//iMonth = stuSystemTime.wMonth - wMonth;    //��Χ-11��1
	//iDay = stuSystemTime.wDay - wDay;          //-30�� 1
	//wHour = stuSystemTime.wHour - wHour;       //-23�� 1
	//wMinute = stuSystemTime.wMinute - wMinute; //-59��1��2��3��4��5
	////2012 06 01 00 05  ���� 2012 05 31 23 58
	////����ʱ��2013 01 01 00 02  ����  2012 12 31 23 58 
	//dwNname = ((wYear*12 + iMonth)*wDay + iDay)*24 + wHour;
	////�ж��Ƿ����1��Сʱ  �������1��Сʱ �����������Ҫ���ļ� ֱ�ӷ���
	//if ((1 != dwNname) && (0 != dwNname))
	//{
	//	return 0;
	//}

	//dwNname = dwNname*60 + wMinute;      //�ó����ߵ�ʱ��֮��

	////������60������  ������
	//if (dwNname>60 )
	//{
	//	return 0;
	//}

	//if (dwNname<5)
	//{//С��5���Ӳ�����
	//	return 0;
	//}

	return true;
}

//�����ļ�����·����ȡ�丸Ŀ¼��
bool CDirWatch::GetParentDirName(char* pInFileName, char* pOutParentDirName)
{
	int iNum = 0;
	char* pDir = NULL;
	char szTempDir[MAX_PATH] = {0};
	if (!(pInFileName&&pOutParentDirName))
	{
		return false;
	}

	pDir = (char*)strrchr(pInFileName, '\\');  //��β�������ַ�  �����ļ���
	iNum = strlen(pDir);
	//if (1 == iNum)
	//{
	//	strncpy(szTempDir, pInFileName, strlen(pInFileName)-1);  //��Ŀ¼����·��
	//	//pDir = (char*)strrchr(szTempDir, '\\');  //��Ŀ¼��
	//}
	strncpy(szTempDir, pInFileName, strlen(pInFileName)-iNum);
	pDir = (char*)strrchr(szTempDir, '\\');  //��β�������ַ�  �����ļ���
	strcpy(pOutParentDirName, pDir+1);
	//pDir = (char*)strrchr(pInFileName, '\\');  //��β�������ַ�  �����ļ���

	return true;
}

////���������ļ��� ����������·�����ļ���
//bool CDirWatch::GetFileName(char* pInFileName, char* pOutFileName)
//{
//	int iNum = 0;
//	char* pDir = NULL;
//	char szTempDir[MAX_PATH] = {0};
//
//	if (!(pInFileName&&pOutFileName))
//	{
//		return false;
//	}
//	return true;
//}

//�����༶Ŀ¼���粻�����򴴽� ʧ�ܷ���false����������true
bool CDirWatch::CreatePath(char* pDirPath)
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


//����Ŀ¼  UINT uiDirType,�������� LPCTSTR pwszDirPath Ŀ¼·�� bool bIsRoot �Ƿ�Ϊ��Ŀ¼
int CDirWatch::TraverseDirectory(char* pszDirPath, bool bIsRoot)
{
	int iRet = 0;
	int iFileNum = 0;   //��ǰ�ļ������ļ����� Ϊ��ֵʱ��������
	HANDLE hFind = NULL;
	WIN32_FIND_DATA stuWFD = {0};	
	char szFullPath[MAX_PATH] = {0};
	char szFindFilter[MAX_PATH] = {0};
	DWORD dwAttributes = 0;
	
	if (!pszDirPath)
	{
		return -1;
	}

	sprintf(szFindFilter, "%s\\*.*", pszDirPath);    //�����ļ�����

	if ((hFind = FindFirstFile(szFindFilter, &stuWFD)) == INVALID_HANDLE_VALUE)
	{
		return -2;
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
			sprintf(szFullPath, "%s%s\\", pszDirPath, stuWFD.cFileName);  //�õ��ļ�������·��

			if (!CheckDirType(stuWFD.cFileName))
				continue;

			iRet = TraverseDirectory(szFullPath, false);
			if (0 > iRet)
			{
				FindClose(hFind);
				return iRet;
			}
			if (0 == iRet)
			{
				RemoveDirectory(szFullPath);  //����ļ���Ϊ��ʱɾ���ļ���
			}
		}
		else 
		{   //�ҵ������ļ� ������Ǹ�Ŀ¼����ʱ������Ŀ¼���ļ�
			if (bIsRoot)
				continue;
			sprintf(szFullPath, "%s%s", pszDirPath, stuWFD.cFileName); //�õ��ļ�����·��
			//�ж��Ƿ���������
			if (!CheckFileIsHide(szFullPath))  
				continue;

			if (CQueue::GetInstance()->Exists(szFullPath))
			{
				continue;
			}

			TracePrint(LOG_INFO, "�����ļ�:%s��\r\n", szFullPath);
			if (1 != DealFile(szFullPath))
				continue;

			//����ɹ�   ɾ���ļ�,���ļ�����ɾ������
			if (!DeleteFile(szFullPath))
			{
				CQueue::GetInstance()->Push(szFullPath);
			}
			else
			{
				TracePrint(LOG_DEBUG, "RSCloudSave delete file: %s\r\n", szFullPath);
			}
		}	
		iFileNum++;
	}while (FindNextFile(hFind, &stuWFD) && m_bIsStoped);

	FindClose(hFind);
	return iFileNum;
}

void CDirWatch::DeleteAll(bool bQuit)
{
	//CQueue* pobjDelQueue = CQueue::GetInstance();
	CQueue::GetInstance()->DeleteAll(bQuit);
}

void CDirWatch::StopTraverse()
{
	m_bIsStoped = false;
}