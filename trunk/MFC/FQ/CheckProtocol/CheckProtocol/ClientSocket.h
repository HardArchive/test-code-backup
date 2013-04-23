#pragma once
#include <winsock2.h>					      //Xt: ���socket��
#pragma comment(lib,"ws2_32.lib")

#define RECV_DATA                             40960

class CClientSocket
{
public:
	CClientSocket(void);
	~CClientSocket(void);
public:
	bool		  GetWinsockLibrary();
	bool		  LinkService(char *pStrIp = "127.0.0.1",int nPort = 8886);
	bool		  SendData(char *pData,int nSendDataLen);                                       
	static		  unsigned long WINAPI RecvThread(void *pParam);	                     //Xt: �����߳�

	void          ResolveRecvData(char *pPack);										     //Xt: ���ս���
	virtual	void  RecvData(char *pData) = 0;                                             //Xt: ���հ���Ϣ
	bool          SendPacket(char *pPack,char cType,char *pSendData,int &nSendDataLen);  //Xt: ���ͷ��
	void		  CloseResources();                                                      //Xt: �ر���Դ
	volatile bool m_bQuieRecvThread;
public:
	bool		  m_bWinsockLibrary;
	SOCKET		  m_hClientSocket;
	HANDLE		  m_hRecvThread;
	HANDLE		  m_hOverRecvThreadEvent;

	int           m_nPackEndLen;
	char          m_strPackEnd[10];
	char		  m_pStrData[40960];
};
