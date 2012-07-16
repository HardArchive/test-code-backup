#include "StdAfx.h"
#include "DirWatch.h"
//
//#include <Shlwapi.h>
//#pragma comment(lib, "Shlwapi.lib")  
#include "Queue.h"
using namespace QNA;
//由于使用初始化为全局成员
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

//判断文件属性是否为隐藏 是返回true
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

//将指定文件拷贝到两目标目录，然后删除原文件
int CDirWatch::DealFile(char* pFileName)
{
	char* pFName = NULL;
	char szParentName[16] = {0};
	char szCopyFileName[MAX_PATH] = {0};
	if (!pFileName)
	{
		return -1;
	}

	//检查是否为dat文件
	pFName = (char*)strrchr(pFileName, '.');  //从尾部查找字符  返回文件格式
	if (strcmp(pFName, ".dat"))
	{
		return -2;
	}
	pFName = NULL;
	
	//获取父目录名
	if (!GetParentDirName(pFileName, szParentName))
	{
		return -3;
	}

	pFName = (char*)strrchr(pFileName, '\\')+1;  //从尾部查找字符  返回文件名

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
	//1、将文件拷以m_szDestFilePath目录并改名不 文件夹名加文件名
	//2、取得文件所在文件夹名
	// 在m_szDestDirPath检查是否有文件夹存在，不存在则创建
	// 当文件拷以m_szDestDirPath的文件夹下
	// 
	//DeleteFile(pFileName);
	return 1;
}

void CDirWatch::DeleteDir(char* pDirPath)
{
	//SHFILEOPSTRUCT FileOp;
	//ZeroMemory((void*)&FileOp,sizeof(SHFILEOPSTRUCT));

	////如果加了FOF_SILENT标志，那么在函数调用的时候，即使出错，也不会弹框提示，，如果这个目录是共享的，依然还是会弹框提示
	//FileOp.fFlags = FOF_SILENT|FOF_NOCONFIRMATION;
	//FileOp.hNameMappings = NULL;
	//FileOp.hwnd = NULL;
	//FileOp.lpszProgressTitle = NULL;
	//FileOp.pFrom = pDirPath;
	//FileOp.pTo = NULL;
	//FileOp.wFunc = FO_DELETE;
	//SHFileOperation(&FileOp);
	// 

	char str[MAX_PATH] ="RMDIR /S/Q "; //加上 /Q  删除时不要求确认
	strcat(str, pDirPath);
	system(str);
}

//检查文件夹类型，判断是否是日期类(eg:201205070945)的文件夹
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

	strncpy(szTem, pDirPath, 4);  //复制年份
	//dwNname =  strtoul(szTem, NULL, 10);
	wYear = atol(szTem);  //eg 2012 03 04 12 58
	if (wYear<1000||wYear>5000)
	{
		return false;    //在2000年到3000年之外的排除
	}
	ZeroMemory(szTem, 5);
	strncpy(szTem, pDirPath+4, 2);//月
	wMonth = atol(szTem);  
	if (wMonth>12||wMonth<1)
	{
		return false;
	}

	strncpy(szTem, pDirPath+4+2, 2); //日
	wDay = atol(szTem); 
	if (wDay>31||wDay<1)
	{
		return false;
	}
	strncpy(szTem, pDirPath+4+2+2, 2); //时
	wHour = atol(szTem); 
	if (wHour>24||wHour<0)
	{
		return false;
	}

	strncpy(szTem, pDirPath+4+2+2+2, 2); //时
	wMinute = atol(szTem); 
	if (wMinute>60||wMinute<0)
	{
		return false;
	}
	//GetLocalTime(&stuSystemTime);
	////GetSystemTime(&stuSystemTime);
	//wYear = stuSystemTime.wYear - wYear;       //年与年之间的差距最多为1
	//iMonth = stuSystemTime.wMonth - wMonth;    //范围-11、1
	//iDay = stuSystemTime.wDay - wDay;          //-30， 1
	//wHour = stuSystemTime.wHour - wHour;       //-23， 1
	//wMinute = stuSystemTime.wMinute - wMinute; //-59，1，2，3，4，5
	////2012 06 01 00 05  —— 2012 05 31 23 58
	////计算时间2013 01 01 00 02  ——  2012 12 31 23 58 
	//dwNname = ((wYear*12 + iMonth)*wDay + iDay)*24 + wHour;
	////判断是否相差1个小时  如果超过1个小时 则非我们所需要的文件 直接返回
	//if ((1 != dwNname) && (0 != dwNname))
	//{
	//	return 0;
	//}

	//dwNname = dwNname*60 + wMinute;      //得出两者的时间之差

	////大于于60分钟则  不处理
	//if (dwNname>60 )
	//{
	//	return 0;
	//}

	//if (dwNname<5)
	//{//小于5分钟不处理
	//	return 0;
	//}

	return true;
}

//传入文件完整路径获取其父目录名
bool CDirWatch::GetParentDirName(char* pInFileName, char* pOutParentDirName)
{
	int iNum = 0;
	char* pDir = NULL;
	char szTempDir[MAX_PATH] = {0};
	if (!(pInFileName&&pOutParentDirName))
	{
		return false;
	}

	pDir = (char*)strrchr(pInFileName, '\\');  //从尾部查找字符  返回文件名
	iNum = strlen(pDir);
	//if (1 == iNum)
	//{
	//	strncpy(szTempDir, pInFileName, strlen(pInFileName)-1);  //父目录完整路径
	//	//pDir = (char*)strrchr(szTempDir, '\\');  //父目录名
	//}
	strncpy(szTempDir, pInFileName, strlen(pInFileName)-iNum);
	pDir = (char*)strrchr(szTempDir, '\\');  //从尾部查找字符  返回文件名
	strcpy(pOutParentDirName, pDir+1);
	//pDir = (char*)strrchr(pInFileName, '\\');  //从尾部查找字符  返回文件名

	return true;
}

////传入完整文件名 传出不包括路径的文件名
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

//创建多级目录，如不存在则创建 失败返回false，其他返回true
bool CDirWatch::CreatePath(char* pDirPath)
{
	int iNum = 0;
	char* pDir = NULL;
	char szParentDir[MAX_PATH] = {0};
	char szTempDir[MAX_PATH] = {0};

	//检验路径是否存在
	if (GetFileAttributes(pDirPath) == FILE_ATTRIBUTE_DIRECTORY)
	{
		return TRUE;
	}

	pDir = (char*)strrchr(pDirPath, '\\');  //从尾部查找字符
	iNum = strlen(pDir);
	if (1 == iNum)
	{
		strncpy(szTempDir, pDirPath, strlen(pDirPath)-1);  //父目录名
		pDir = (char*)strrchr(szTempDir, '\\');
		//pwDir += 1;
		iNum += strlen(pDir);
	}

	strncpy(szParentDir, pDirPath, strlen(pDirPath)-iNum);

	//检验父级路径是否存在  如果存在则创建子目录
	if (GetFileAttributes(szParentDir) == FILE_ATTRIBUTE_DIRECTORY)
	{		
		if (!CreateDirectory(pDirPath, NULL))
		{
			//创建文件夹失败
			//FindClose(hParentFile);
			::MessageBox(NULL, pDirPath, "创建文件夹失败", MB_OK);
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
		//如果不存在则递归
		if (!CreatePath(szParentDir))
		{
			return false;
		}
		else
		{//父目录创建完毕则创建子目录
			if (!CreatePath(pDirPath))
			{
				return false;
			}
			return true;
		}
	}

	return true;
}


//遍历目录  UINT uiDirType,操作参数 LPCTSTR pwszDirPath 目录路径 bool bIsRoot 是否为根目录
int CDirWatch::TraverseDirectory(char* pszDirPath, bool bIsRoot)
{
	int iRet = 0;
	int iFileNum = 0;   //当前文件夹下文件数量 为负值时遍历出错
	HANDLE hFind = NULL;
	WIN32_FIND_DATA stuWFD = {0};	
	char szFullPath[MAX_PATH] = {0};
	char szFindFilter[MAX_PATH] = {0};
	DWORD dwAttributes = 0;
	
	if (!pszDirPath)
	{
		return -1;
	}

	sprintf(szFindFilter, "%s\\*.*", pszDirPath);    //查找文件类型

	if ((hFind = FindFirstFile(szFindFilter, &stuWFD)) == INVALID_HANDLE_VALUE)
	{
		return -2;
	}

	do
	{ 
		//过滤本级目录和父目录
		if(_tcscmp(stuWFD.cFileName, _T(".")) == 0 || _tcscmp(stuWFD.cFileName, _T("..")) == 0)
			continue;

		//如果找到的是目录，则进入此目录进行递归
		if(stuWFD.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) 
		{
			ZeroMemory(szFullPath, sizeof(szFullPath));
			sprintf(szFullPath, "%s%s\\", pszDirPath, stuWFD.cFileName);  //得到文件夹完整路径

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
				RemoveDirectory(szFullPath);  //如果文件夹为空时删除文件夹
			}
		}
		else 
		{   //找到的是文件 如果不是根目录，即时间日期目录下文件
			if (bIsRoot)
				continue;
			sprintf(szFullPath, "%s%s", pszDirPath, stuWFD.cFileName); //得到文件完整路径
			//判断是否不隐藏属性
			if (!CheckFileIsHide(szFullPath))  
				continue;

			if (CQueue::GetInstance()->Exists(szFullPath))
			{
				continue;
			}

			TracePrint(LOG_INFO, "处理文件:%s…\r\n", szFullPath);
			if (1 != DealFile(szFullPath))
				continue;

			//处理成功   删除文件,把文件放入删除队列
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