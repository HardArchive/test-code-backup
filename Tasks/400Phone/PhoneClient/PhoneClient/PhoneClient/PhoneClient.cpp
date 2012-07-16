// PhoneClient.cpp : �������̨Ӧ�ó������ڵ㡣
//

#include "stdafx.h"
#include "ClientService.h"
 
#pragma comment(lib,"ws2_32.lib")


int _tmain(int argc, _TCHAR* argv[])
{
	//ȡ����־�ļ�·��
	ZeroMemory(g_szMinitorFolderPah, sizeof(g_szMinitorFolderPah));
	CTool::Get400RegConfig(TEXT("logpath"), g_szMinitorFolderPah);
	if (strlen(g_szMinitorFolderPah)<3)
	{
		char szTemp[MAX_PATH] = {0};
		//��ȡ��ǰĿ¼  //�����ǻ�ȡ��ǰ�����ļ�������·�� 
		if (!GetModuleFileName(NULL, szTemp, MAX_PATH))
		{
			return 0;
		} 
		//GetCurrentDirectory(MAX_PATH, g_szMinitorFolderPah);
		memcpy(g_szMinitorFolderPah, szTemp, (strlen(szTemp)-strlen("/PhoneClient.exe")));
		strcat(g_szMinitorFolderPah, "\\log");
		CTool::SetRegConfig(TEXT("logpath"), g_szMinitorFolderPah);
	}
	g_objLog.SetLogPath(g_szMinitorFolderPah);

	//��ȡ���·��
	ZeroMemory(g_szMinitorFolderPah, sizeof(g_szMinitorFolderPah));
	CTool::Get400RegConfig(TEXT("path"), g_szMinitorFolderPah);
	if (!strlen(g_szMinitorFolderPah))
	{
		TracePrint(LOG_ERROR, "��ע����ȡ���·��ʧ�ܡ�%s;\r\n", g_szMinitorFolderPah);
		return 0;
	}

	CClientService service;
	if(!service.InitInstance())
	{
		return 1;
	}
	service.ExitInstance();
	return 0;
}
