#pragma once
#include <Windows.h>
/******************************************************************************* 
* 1�� �ļ����ƣ� UrlShortcut
* 2�� �桡������ Version *.*
* 3�� ��    ���� URL��ݷ�ʽ����
* 4�� ������ƣ� ��ë
* 5�� �������ڣ� 2012-2-6 13:40:36
* 6�� �� �� �ˣ� 
* 7�� �޸����ڣ� 
********************************************************************************/ 

class CUrlShortcuts
{
public:
	CUrlShortcuts(void);
	~CUrlShortcuts(void);

public:
	TCHAR m_tszTitle[MAX_PATH];       //����
	TCHAR m_tszUrl[MAX_PATH];         //��ַ
	TCHAR m_tszParentDir[MAX_PATH];   //�����ڵĸ�Ŀ¼
	int m_iParentLevel[MAX_PATH];     //��Ŀ¼���� ��Ŀ¼Ϊ�㼶��������ϵ���
	//
	int m_iUrlPosition;                  //����ʹ��  ���б�ؼ��е�λ�ã�����ʱ�����λ��


};
