#pragma once

#include "Ini.h"
#include "DebugTrace.h"

//������������Ϣ
typedef struct _SERVER
{
	TCHAR tszIP[64];       //IP��ַ
	UINT uiPort;           //�˿ں�
}SERVER, *PSERVER;

//��ѯ��Ϣ
typedef struct _QUERYINFO
{
	int   iSiteNum;              //��վ���
	TCHAR tszUrl[MAX_PATH];      //��ѯ�õĵ�������վ����ַ
	TCHAR tszRegex[MAX_PATH];    //����ַ��Ӧ��������ʽ
}QUERYINFO, *PQUERYINFO;
class CConfig
{
public:
	CConfig(void)
	{
		m_bInit = false;
	}
	virtual ~CConfig(void)
	{
		m_bInit = false;
	}
	//��ʼ��������δ��ʼ��֮ǰ������������ִ��
	bool Init()
	{
		TCHAR tszIniPath[MAX_PATH] = {0};
		if (!GetExePath(tszIniPath))
		{
			return false;
		}
		_stprintf_s(tszIniPath, MAX_PATH, _T("%s\\config.ini"), tszIniPath);

		if (!m_clsIni.SetIniFilename(tszIniPath))
		{
			return false;
		}
		m_bInit = true;
		return true;
	}
	//��ȡ��ǰ��������Ŀ¼
	bool GetExePath(LPTSTR pstrPath)
	{
		LPTSTR ptTem = NULL;
		TCHAR tszTemp[MAX_PATH] = {0};
		//��ȡ��ǰĿ¼  //�����ǻ�ȡ��ǰ�����ļ�������·�� 
		if (!GetModuleFileName(NULL, tszTemp, MAX_PATH) && pstrPath)
			return false; 

		ptTem = _tcsrchr(tszTemp, '\\');
		memcpy(pstrPath, tszTemp, (_tcslen(tszTemp)-_tcslen(ptTem))*sizeof(TCHAR));
		return true;
	}

	//��ȡ������IP�Ͷ˿� stuServer ������IP�˿ڣ� bIsDCServer�Ƿ�Ϊ�ַ�����
	bool GetServer(SERVER& stuServer, bool bIsDCServer)
	{
		std::tstring strTem;
		if (!m_bInit)
		{
			QNA::TRACE( _T("%s��%d ���д���!!!!"), __FILE__, __LINE__);
			QNA::TRACE(_T("QNA-GetServer û�г�ʼ������"));
			return false;			
		}
		//��ȡIP
		if (bIsDCServer)
			strTem = m_clsIni.GetString(_T("DC"), _T("IP"));   //�ַ�����
		else
			strTem = m_clsIni.GetString(_T("PB"), _T("IP"));   //������̨

		if (!strTem.length())
		{
			QNA::TRACE(_T("QNA-GetServer ��ȡIP���󡭡�"));
			return false;
		}
		_stprintf_s(stuServer.tszIP, 64, _T("%s"), strTem.c_str());
		//��ȡ�˿�
		if (bIsDCServer)
			stuServer.uiPort = m_clsIni.GetInt(_T("DC"), _T("Port"));
		else
			stuServer.uiPort = m_clsIni.GetInt(_T("PB"), _T("Port"));

		if (!stuServer.uiPort)
		{
			QNA::TRACE(_T("QNA-GetServer ��ȡ�˿ڴ��󡭡�"));
			return false;
		}
		return true;
	}

	/******************************************************************************* 
	1�� Function Name �� GetWebQueryInfo
	2�� Description   �� ȡ����һ������վ����ַ������
	3�� Parameters    �� stuQueryInfo���ڷ��� ���ϴ�ȡ���Ľṹ
	4�� Return Value  �� �ɹ�1�� δ��ʼ��-1�� ����վ���Ѿ�����-0��ȡ��ַ����-2��ȡ�������-3
	5�� Dynamic memory�� ��
	6�� Author        ��  QNA 
	7�� Created       �� 2012-7-18 15:58:12
	8�� Remark        �� ע�⴫��������ϴε���GetWebQueryInfo��ȡ�õĲ�����  ��һ�δ��ռ���
	********************************************************************************/ 
	int GetWebQueryInfo(QUERYINFO& stuQueryInfo)
	{
		int iNum = 0;
		TCHAR tszTem[32] = {0};
		std::tstring strTem;
		if (!m_bInit)
		{
			QNA::TRACE( _T("%s��%d ���д���!!!!"), __FILE__, __LINE__);
			QNA::TRACE(_T("QNA-GetWebQueryInfo û�г�ʼ������"));
			return -1;			
		}
		iNum = ClacNext(stuQueryInfo.iSiteNum);
		if (-1 == iNum)
		{
			return 0;
		}
		_stprintf_s(tszTem, 32, _T("SITE%d"), iNum);
		strTem = m_clsIni.GetString(tszTem, _T("Url"));    //����վ����ַ
		if (!strTem.length())
		{
			QNA::TRACE(_T("QNA-GetWebQueryInfo ��ȡ��ַ���󡭡�"));
			return -2;
		}
		_stprintf_s(stuQueryInfo.tszUrl, MAX_PATH, _T("%s"), strTem.c_str());

		strTem = m_clsIni.GetString(tszTem, _T("Regex"));   //����
		if (!strTem.length())
		{
			QNA::TRACE(_T("QNA-GetWebQueryInfo ��ȡ������󡭡�"));
			return -3;
		}
		_stprintf_s(stuQueryInfo.tszRegex, MAX_PATH, _T("%s"), strTem.c_str());
		
		stuQueryInfo.iSiteNum = iNum;
		return 1;
	}

	/******************************************************************************* 
	1�� Function Name �� ResetQuery
	2�� Description   �� ���ò�ѯ���ص�һ������վ����Ϣ��
	3�� Parameters    �� stuQueryInfo ������һ������վ����Ϣ
	4�� Return Value  �� �ɹ�true��ʧ��false
	5�� Dynamic memory�� ��
	6�� Author        �� QNA 
	7�� Created       �� 2012-7-18 16:04:35
	8�� Remark        �� 
	********************************************************************************/ 
	bool ResetQuery(QUERYINFO& stuQueryInfo)
	{
		stuQueryInfo.iSiteNum = 0;
		if (1!= GetWebQueryInfo(stuQueryInfo))
		{
			return false;
		}
		return true;
	}

private:
	//��ȡ����վ������ 
	int GetSiteNum()
	{
		return m_clsIni.GetInt(_T("SITENUM"), _T("Num"));
	}

	//������һ��ȡֵ����
	int ClacNext(int iNum)
	{
		//��Ӧ���ǵ�һ��ȡֵ
		if ((iNum<1)||iNum>GetSiteNum())
			return 1;
		//��Ӧ���ǵ�Num(Iniֵ)+1��ȡֵ,���Բ���ȡ
		if (GetSiteNum() == iNum)
			return -1;
		return ++iNum;
	}

private:
	QNA::CIni m_clsIni;
	bool m_bInit;         //��ʼ������

};
