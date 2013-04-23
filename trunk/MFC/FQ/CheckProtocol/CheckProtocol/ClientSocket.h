#pragma once
#include <winsock2.h>					      //Xt: 添加socket库
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
	static		  unsigned long WINAPI RecvThread(void *pParam);	                     //Xt: 接收线程

	void          ResolveRecvData(char *pPack);										     //Xt: 接收解析
	virtual	void  RecvData(char *pData) = 0;                                             //Xt: 接收包信息
	bool          SendPacket(char *pPack,char cType,char *pSendData,int &nSendDataLen);  //Xt: 发送封包
	void		  CloseResources();                                                      //Xt: 关闭资源
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
