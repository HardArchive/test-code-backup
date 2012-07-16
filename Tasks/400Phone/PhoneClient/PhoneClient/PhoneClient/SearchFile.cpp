#include "stdafx.h"
#include "SearchFile.h"

//���ûص�����
void CSearchFile::SetCallBackFun(DoFileCallback pFun)
{
	this->m_funDoFile =pFun;
}

//��ʼ�����ļ�,�ҵ�����ûص�����
void CSearchFile::StartSearch()
{
	char szFind[MAX_PATH];
	strcpy(szFind, g_szMinitorFolderPah);
	strcat(szFind,"REQ*.TXT");

	WIN32_FIND_DATA FindData;
	HANDLE hFind=::FindFirstFile(szFind,&FindData);
	if(INVALID_HANDLE_VALUE==hFind)
	{
		return;
	}
	this->m_funDoFile(FindData.cFileName);
	::FindClose(hFind);
}