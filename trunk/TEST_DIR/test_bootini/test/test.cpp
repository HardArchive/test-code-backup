// test.cpp : �������̨Ӧ�ó������ڵ㡣
//

#include "stdafx.h"

#define BOOTLOGO L"/bootlogo"
#define NOGUIBOOT L"/noguiboot"

void XpModifyIni(bool bFlag);
int _tmain(int argc, _TCHAR* argv[])
{
	XpModifyIni(false);

	return 0;
}

//ϵͳ���������������ж�boot.ini���Ƿ�����˿���LOGO��
//δ���"/bootlogo"����1�� δ���"/noguiboot"����2��������δ��ӷ���3
//���߶��Ѿ���ӷ���0
int AnalysisParameter(WCHAR* pszParameter)
{
	int iRet = 0;
	WCHAR *pTemp = NULL;

	pTemp = wcswcs(pszParameter, BOOTLOGO);
	if (!pTemp)
	{
		//δ���"/bootlogo"����1
		iRet = 1;
	}

	pTemp = wcswcs(pszParameter, NOGUIBOOT);
	if (!pTemp)
	{
		if (1==iRet)
			iRet = 3;   //���߶�δ��ӷ���3
		else
			iRet = 2;   //δ���"/noguiboot"����2
	}

	return iRet;
}

/******************************************************************************* 
1�� �������ƣ� ChangeParam(WCHAR* pszParam, int iInPut)
2�� ���������� 
3�� ��������� WCHAR* pszParamϵͳ��������
			 int iInPut ��������
				1�����BOOTLOGO
				2�����NOGUIBOOT
				3�����BOOTLOGO��NOGUIBOOT
				4:ȥ��BOOTLOGO��NOGUIBOOT
4�� �� �� ֵ�� ��
5�� ��̬�ڴ棺 ��
6�� ������ƣ�  ��ë
7�� �������ڣ� 2011-9-30 15:45:56
8�� ��    ע�� �������pszParam�������㹻��Ŀռ������BOOTLOGO��NOGUIBOOT����ܿ��ܳ���
********************************************************************************/ 
void ChangeParam(WCHAR* pszParam, int iInPut)
{
	int iNum = 0;
	WCHAR* pTemp = NULL;
	WCHAR wszTemp[MAX_PATH] = {0};
	BOOL bLogo = FALSE;
	BOOL bNogui = FALSE;
	bLogo = (1 == iInPut)?TRUE:((3 == iInPut)?TRUE:FALSE);
	bNogui = (2 == iInPut)?TRUE:((3 == iInPut)?TRUE:FALSE);
	//���BOOTLOGO��NOGUIBOOT
	if (bLogo)
	{
		wcscat(pszParam, BOOTLOGO);
	}

	if (bNogui)
	{
		wcscat(pszParam, NOGUIBOOT);
	}

	if (4 == iInPut)
	{
		//�Ƴ�BOOTLOGO
		pTemp = wcswcs(pszParam, BOOTLOGO);	//���ҵ�һ�γ��ֵ�λ��	
		if (pTemp)
		{
			iNum = wcslen(pTemp);
			wcsncpy(wszTemp, pszParam, wcslen(pszParam)-iNum);//copy���ֵ�ǰһ����
			iNum = wcslen(wszTemp) + wcslen(BOOTLOGO);
			wcscat(wszTemp, pszParam+iNum);  //���Ƴ��ֵĺ��沿��
			wcscpy(pszParam, wszTemp);

			pTemp = NULL;
			iNum = 0;
			ZeroMemory(wszTemp, sizeof(WCHAR)*wcslen(wszTemp));
		}
		//�Ƴ�NOGUIBOOT
	   	pTemp = wcswcs(pszParam, NOGUIBOOT);	//���ҵ�һ�γ��ֵ�λ��	
		if (pTemp)
		{
			iNum = wcslen(pTemp);
			wcsncpy(wszTemp, pszParam, wcslen(pszParam)-iNum);//copy���ֵ�ǰһ����
			iNum = wcslen(wszTemp) + wcslen(NOGUIBOOT);
			wcscat(wszTemp, pszParam+iNum);  //���Ƴ��ֵĺ��沿��
			wcscpy(pszParam, wszTemp);

			pTemp = NULL;
			iNum = 0;
			ZeroMemory(wszTemp, sizeof(WCHAR)*wcslen(wszTemp));
		}
	}
}

/******************************************************************************* 
1�� �������ƣ� XpModifyIni(bool bFlag)
2�� ���������� ��BOOT.INI������Ƴ�����BOOTLOGO��NOGUIBOOT��������
3�� ��������� bool bAddFlag, ��ӱ�־������Ϊtrueʱ,����Ϊfalseʱ,�Ƴ�BOOTLOGO��NOGUIBOOT
4�� �� �� ֵ�� �ɹ�����true��ʧ�ܷ���false
5�� ��̬�ڴ棺 ��
6�� ������ƣ�  ��ë
7�� �������ڣ� 2011-9-30 16:25:22
8�� ��    ע�� 
********************************************************************************/ 
void XpModifyIni(bool bAddFlag)
{
	int iNum = 0;	
	WCHAR wszIniStrOS[MAX_PATH] = {0};     //wszIniStrLoader =�Ĳ���
	WCHAR wszIniStrTEMP[MAX_PATH] = {0};
	WCHAR wszBootIniPath[MAX_PATH] = {0};

	//��ȡϵͳ�����̷�
	GetSystemDirectory(wszBootIniPath, MAX_PATH);
	memcpy(wszIniStrTEMP, wszBootIniPath, 3*sizeof(WCHAR));
	ZeroMemory(wszBootIniPath, sizeof(WCHAR));
	swprintf(wszBootIniPath, L"%sboot.ini", wszIniStrTEMP);
	ZeroMemory(wszIniStrTEMP, sizeof(WCHAR));

	//�õ�default=���ַ��������������õ�ϵͳ
	GetPrivateProfileString(
		L"boot loader",
		L"default",
		L"",
		wszIniStrTEMP,
		MAX_PATH,
		wszBootIniPath);

	//�õ�ϵͳ��������Ĳ���
	GetPrivateProfileString(
		L"operating systems",
		wszIniStrTEMP,
		L"",
		wszIniStrOS,
		MAX_PATH,
		wszBootIniPath);

	iNum = AnalysisParameter(wszIniStrOS);

	if(bAddFlag)
	{
		if (!iNum)
		{
			ZeroMemory(wszIniStrOS, sizeof(wszIniStrOS));
			ZeroMemory(wszIniStrTEMP, sizeof(wszIniStrTEMP));
			ZeroMemory(wszBootIniPath, sizeof(wszBootIniPath));
			return;
		}
		
		ChangeParam(wszIniStrOS, iNum);
	}
	else
	{
		ChangeParam(wszIniStrOS, 4);
	}

	::WritePrivateProfileString(
		L"operating systems",
		wszIniStrTEMP,
		wszIniStrOS,
		wszBootIniPath);	

	ZeroMemory(wszIniStrOS, sizeof(wszIniStrOS));
	ZeroMemory(wszIniStrTEMP, sizeof(wszIniStrTEMP));
}

