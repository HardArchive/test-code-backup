// Define.h
// ��ȡACE�����ļ��ķ�����
// �κ�һ���£����Ǵ��㿪ʼ�ģ����ԣ���Ҫ�����Ժ�����ʧ�ܣ����ڵĻ���������δ���������ѵĻ�ʯ��
// add by freeeyes
// 2008-12-22

#ifndef _APPCONFIG_H
#define _APPCONFIG_H

#include "define.h"

#include "ace/ACE.h"
#include "ace/OS_main.h"
#include "ace/Configuration.h"
#include "ace/Configuration_Import_Export.h"

class CAppConfig
{
public:
	CAppConfig(void);
	~CAppConfig(void);

	void Close();

	bool ReadConfig(const char* szConfigname);
	bool WriteConfig(const char* szConfigname);
	bool WriteConfig();

	bool GetValue(const char* szName, ACE_TString& strValue, const char* szRoot);
	bool SetValue(const char* szName, ACE_TString& strValue, const char* szRoot);

	const char* GetError();

private:
	ACE_Configuration_Heap* m_pConfig;
	ACE_Ini_ImpExp*         m_pIniImp;
	ACE_TString             m_strConfigName;
	char                    m_szError[MAX_BUFF_500];
};

#endif
