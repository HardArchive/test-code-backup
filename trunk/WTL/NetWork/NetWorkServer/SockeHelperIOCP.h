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

	//�̺߳���
private:
	static DWORD WINAPI _ListenThreadProc(LPVOID lpParam);  //�����߳� - ���������߳�
	static DWORD WINAPI _WorkerThreadProc(LPVOID lpParam);  //�����߳� - ���մ��������߳�


public:
	bool Init(int nPort = 4567);     //��ʼ������
	bool UnInit();   //�˳�������


protected:

public:

};


