#ifndef _RS_RECVDATA_H
#define _RS_RECVDATA_H
#include <winsock2.h>

typedef struct tagFileInfo
{
	int iNum;             //第几路电话
	char PhoneNum[16];    //手机号码
	char Pwd[16];         //密码
	unsigned short ErrTimes; //错误次数
	tagFileInfo()
	{
		::memset(this,0,sizeof(tagFileInfo));
	}
}FileInfo;

class CRecvData
{
private:
	BOOL b_IsContinue;
	SOCKET m_sListening;
	unsigned short m_iPort;
public:
	CRecvData(unsigned short iPort)
	{
		m_sListening=INVALID_SOCKET;
		b_IsContinue=TRUE;
		m_iPort=iPort;
	}
	CRecvData()
	{
		m_sListening=INVALID_SOCKET;
		b_IsContinue=TRUE;
		m_iPort=0;
	}
	void SetPort(unsigned short iPort)
	{
		m_iPort=iPort;
	}
	BOOL Init();
	void UnInit();
	void Start();
	void Stop()
	{
		this->b_IsContinue=FALSE;
	}
};
#endif