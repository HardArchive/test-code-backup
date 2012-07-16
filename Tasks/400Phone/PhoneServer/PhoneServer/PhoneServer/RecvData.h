#ifndef _RS_RECVDATA_H
#define _RS_RECVDATA_H
#include <winsock2.h>

typedef struct tagFileInfo
{
	int iNum;             //�ڼ�·�绰
	char PhoneNum[16];    //�ֻ�����
	char Pwd[16];         //����
	unsigned short ErrTimes; //�������
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