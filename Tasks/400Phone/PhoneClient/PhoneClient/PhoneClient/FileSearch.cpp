#include "stdafx.h"
#include "FileSearch.h"
#include "SearchFile.h"
#include "Tool.h"

//int g_iFileNum = 0;
//char g_szFolder[MAX_PATH] = {0};
CSendQueue CFileSearch::m_Queue;

void CFileSearch::Start()
{
	if(!strlen(g_szMinitorFolderPah))
	{
		//文件夹路径没有指定
		TracePrint(LOG_ERROR, "文件夹路径没有指定…%s;\r\n", g_szMinitorFolderPah);
		return;
	}
	//g_iFileNum = 0;
	//ZeroMemory(g_szFolder, sizeof(g_szFolder));

	CSearchFile search;
	search.SetCallBackFun(DoFoundFile);

	while(m_bIsContinue)
	{
		search.StartSearch();
		::Sleep(600);
	}
}

void CFileSearch::Stop()
{
	//g_iFileNum = 0;
	//ZeroMemory(g_szFolder, sizeof(g_szFolder));
	this->m_bIsContinue=FALSE;
}

//BOOL CFileSearch::CreateMyFile(const char *pszFolderPath,const char *pszFileName,int iFileNum,const char *pFileContent)
//{
//	char szFileFullPath[MAX_PATH]={0};
//	sprintf_s(szFileFullPath,MAX_PATH,"%s%s%d.TXT",pszFolderPath,pszFileName,iFileNum);
//	FILE *file=fopen(szFileFullPath,"w");
//	if(NULL ==file)
//	{
//		return FALSE;
//	}
//	fputs(pFileContent,file);
//	fclose(file);
//	return TRUE;
//}

//从pszFilePath指的文件中取出电话号码和密码放入fiRecord结构中
BOOL CFileSearch::ParseFileInfo(const char *pszFilePath,FileInfo &fiRecord)
{
	int iLen = 0;
	char* pTem = NULL;
	char szPhoneNum[16] = {0};
	char szPwdNum[16] = {0};
	char szFileTem[128] = {0};

	FILE *PFile= fopen(pszFilePath, "r");
	if(NULL == PFile)
	{
		printf("打开文件失败\n");
		return FALSE;
	}
	
	if(NULL==fgets(szFileTem, 128, PFile))
	{
		fclose(PFile);
		printf("读取数据失败\n");
		return FALSE;
	}
	fclose(PFile);

	pTem = (char*)strrchr(szFileTem, ',');
	strncpy(szPhoneNum, szFileTem, strlen(szFileTem)-strlen(pTem));
	strcpy(szPwdNum, pTem+1);

	iLen = strlen(szPwdNum);
	printf("去空格前：%s,Len:%d\r\n", szPwdNum, iLen);
	for (int i=iLen; i>=0; i--)
	{
		if ('0'<= szPwdNum[i] && szPwdNum[i]<='9' )
			break;
		szPwdNum[i] = '\0';
	}

	iLen = strlen(szPwdNum);
	printf("去空格后：%s,Len:%d\r\n", szPwdNum, iLen);
	if(strlen(szPhoneNum) !=11 || iLen>8 ||  iLen<4)
	{
		//fclose(file);
		printf("计算长度失败\n");
		return FALSE;
	}
	strncpy(fiRecord.PhoneNum, szPhoneNum, 11);
	strncpy(fiRecord.Pwd, szPwdNum, iLen);
	g_objLog.Userlog(1, szFileTem);  //记录日志


	return TRUE;
}
//char *szFilePath,监控文件名REQ(*).TXT  const char *szFolderPath文件夹路径
void CFileSearch::DoFoundFile(const char *szFilePath)
{
	FileInfo record;
	char szFile[MAX_PATH] = {0};
	//获取文件号
	record.iNum = CTool::GetNum(szFilePath);
	if(-1 == record.iNum)
	{
		printf("%s num is %d\n",szFilePath, record.iNum);
		return;
	}
	
	sprintf_s(szFile, MAX_PATH, "%sVMSLINK%d.TXT", g_szMinitorFolderPah, record.iNum);

	if(!ParseFileInfo(szFile, record))
	{
		printf("find error data from %s\n",szFile);
		if(!CTool::SaveResult(record.iNum, "4000"))
			printf("create GOTOBOX file error!\n");
	}
	else
	{
		m_Queue.AddFileInfo(record);
	}

	////删除VMSLINK(n)文件
	//if(!::DeleteFile(szFile))
	//{
	//	::Sleep(200);
	//	::DeleteFile(szFile);
	//}

	//删除REQ(n)文件
	ZeroMemory(szFile, MAX_PATH);
	sprintf_s(szFile, MAX_PATH, "%s%s", g_szMinitorFolderPah, szFilePath);
	if(!::DeleteFile(szFile))
	{
		::Sleep(200);
		::DeleteFile(szFile);
	}
}