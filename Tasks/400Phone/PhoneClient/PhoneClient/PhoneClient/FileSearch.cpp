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
		//�ļ���·��û��ָ��
		TracePrint(LOG_ERROR, "�ļ���·��û��ָ����%s;\r\n", g_szMinitorFolderPah);
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

//��pszFilePathָ���ļ���ȡ���绰������������fiRecord�ṹ��
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
		printf("���ļ�ʧ��\n");
		return FALSE;
	}
	
	if(NULL==fgets(szFileTem, 128, PFile))
	{
		fclose(PFile);
		printf("��ȡ����ʧ��\n");
		return FALSE;
	}
	fclose(PFile);

	pTem = (char*)strrchr(szFileTem, ',');
	strncpy(szPhoneNum, szFileTem, strlen(szFileTem)-strlen(pTem));
	strcpy(szPwdNum, pTem+1);

	iLen = strlen(szPwdNum);
	printf("ȥ�ո�ǰ��%s,Len:%d\r\n", szPwdNum, iLen);
	for (int i=iLen; i>=0; i--)
	{
		if ('0'<= szPwdNum[i] && szPwdNum[i]<='9' )
			break;
		szPwdNum[i] = '\0';
	}

	iLen = strlen(szPwdNum);
	printf("ȥ�ո��%s,Len:%d\r\n", szPwdNum, iLen);
	if(strlen(szPhoneNum) !=11 || iLen>8 ||  iLen<4)
	{
		//fclose(file);
		printf("���㳤��ʧ��\n");
		return FALSE;
	}
	strncpy(fiRecord.PhoneNum, szPhoneNum, 11);
	strncpy(fiRecord.Pwd, szPwdNum, iLen);
	g_objLog.Userlog(1, szFileTem);  //��¼��־


	return TRUE;
}
//char *szFilePath,����ļ���REQ(*).TXT  const char *szFolderPath�ļ���·��
void CFileSearch::DoFoundFile(const char *szFilePath)
{
	FileInfo record;
	char szFile[MAX_PATH] = {0};
	//��ȡ�ļ���
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

	////ɾ��VMSLINK(n)�ļ�
	//if(!::DeleteFile(szFile))
	//{
	//	::Sleep(200);
	//	::DeleteFile(szFile);
	//}

	//ɾ��REQ(n)�ļ�
	ZeroMemory(szFile, MAX_PATH);
	sprintf_s(szFile, MAX_PATH, "%s%s", g_szMinitorFolderPah, szFilePath);
	if(!::DeleteFile(szFile))
	{
		::Sleep(200);
		::DeleteFile(szFile);
	}
}