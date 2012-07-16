/*******************************************************************************
 * Copyright (C), 2011-2020, ��  ��
 * FileName: AppConfig.h
 * Author: ��  ��    Version: 1.0    Date: 2011-08-18
 * Description: Ӧ�ó���������
 * Version: 1.0     // �汾��Ϣ
 * Function List:   // ��Ҫ�������书��
 * 1. 
 * History:         // ��ʷ�޸ļ�¼
 * <author>   <time>   <version>   <desc>
 *  ��  ��  2011-08-18    1.0    build this moudle  
 ******************************************************************************/
#pragma once

#ifndef APP_CONFIG_H_
#define APP_CONFIG_H_
#include <vector>

using namespace std;


// �����̻߳���
class CAppConfig
{
public:
	// ��ʼ��
	static bool Init();
	// ��ȡӦ�ó�������ʵ��
	static CAppConfig** GetInstance();
	// �ͷ���Դ
	static void Free();
	// �մ�ָ��
	static char* Empty();
	// ������������
	bool LoadData();
public:
	// Ӧ�ó���Ŀ¼
	char AppDir[MAX_PATH];
	////// Դ����·��������'\\'�ַ�
	////char SourcePath[MAX_PATH];
	////// Ŀ������·��������'\\'�ַ�
	////char DestPath[MAX_PATH];
	////// ���ļ����������·��[Ŀǰ��Ҫ�ǵ����ʼ��ĸ���]������'\\'�ַ�
	////char FilePath[MAX_PATH];
	////// �����ļ�·��������'\\'�ַ�
	////char ErrorPath[MAX_PATH];
	////// �������ļ�·��������'\\'�ַ�
	////char OtherPath[MAX_PATH];
	//���Ŀ¼·��
	char m_szWatchDirPath[MAX_PATH];
	//Ŀ¼�ƶ�Ŀ��·��
	char m_szDestDirPath[MAX_PATH];   
	//�ļ��ƶ�Ŀ¼·��
	char m_szDestFilePath[MAX_PATH]; 


	// ��������ģʽ��С��2ʱ��������ģʽ������2ʱ����ʾ�������ӽ�������Ĭ��Ϊ������ģʽ
	DWORD m_RunMode;
private:
	// ���캯��
	CAppConfig();
	// ��������
	~CAppConfig();

	// �ַ���д��ע���
	void WriteStringRegistry(LPCTSTR strName, const string & strValue);
	// ��ע�������������Ϣ
	bool LoadDataFromRegister();

	// ��ȡע���洢���ļ�·������Ĭ�����·������'\\'�ַ�
	inline int GetRegPath(HKEY hKey, const char* pKeyName, char* pValue, const char* pDefault);

	// 
	DWORD IpStr2Dword(const char* pSrc);


	// ʵ������
	static CAppConfig *m_instance;
	// �մ�ָ��
	static char m_empty;
};

#define EMPTY_POINTER CAppConfig::Empty()
#endif