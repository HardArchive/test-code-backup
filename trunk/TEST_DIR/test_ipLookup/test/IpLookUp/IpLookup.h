#pragma once

#include <vector>
using namespace std;
#include "QNA_File.h"
#include "QNA_DownLoad.h"
using namespace QNA;


//IP�����������

class CIpLookup
{
public:
	CIpLookup(void);
	~CIpLookup(void);

public:
	//����������ļ���ȡ�ĵ���������������ַ��������ص�������ʽ
	//void Init();
	void SetUrlRegex(LPCTSTR pstrUrl, LPCTSTR pstrRegex); //���õ�����������������ַ��������ص�������
	//����IP ���з������
	bool StartIpLookup(LPCTSTR pstrIP);      //��ʼ����

	//��ȡ������� ���������л�ȡ  ÿ��ȡһ��
	//��ȡ��ǰ·�� 
	bool GetCurrentPath(LPTSTR pstrPath);
private:
	vector<wstring> m_strVecDomain;   //���ڴ���������ַ�������
	//vector<string>::iterator m_stringIter; //����ָ���ַ����ĵ���������

	TCHAR m_tszUrl[MAX_PATH];       //������������������ַ
	TCHAR m_tszRegex[MAX_PATH];     //������������������ַ��ص�������ʽ
	//TCHAR m_tszIP[64];              //�跴���IP
	//TCHAR m_tszLoadBuf[512*1024];  //��ҳ�ļ���ȡBuffer 1MB

private:
	//bool Download(void);
	void LoadFile(std::wstring& wstrOut, std::wistream& wistreamIn);
	//bool GetHtmlBuffer();  //��ȡ�Ѿ�����ҳ�浽�ڴ�
	bool ParseBuffer(LPCTSTR pstrPath);    //ͨ���������Buffer ��������ص�����
};
