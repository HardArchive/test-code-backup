#pragma once

/***************************************************************************************************
* 1�� class      �� SockeHelperIOCP
* 2�� Version    �� *.*
* 3�� Description�� ��ɶ˿ڴ�����
* 4�� Author     �� QNA (http://www.9cpp.com/)
* 5�� Created    �� 2012-11-30 17:06:01
* 6�� History    �� 
* 7�� Remark     �� 
****************************************************************************************************/
extern bool g_bExit;   //ȫ���˳�����

class CSockeHelperIOCP
{
public:
	CSockeHelperIOCP(void);
	~CSockeHelperIOCP(void);
	static DWORD WINAPI AcceptConnetProc(LPVOID lpParam);  //���������߳�
	//����̽������
	static DWORD WINAPI ReceiveDataProc(LPVOID lpParam);   //���մ��������߳�

public:
	bool Init();     //��ʼ������
	bool UnInit();   //�˳�������


protected:

public:

};


