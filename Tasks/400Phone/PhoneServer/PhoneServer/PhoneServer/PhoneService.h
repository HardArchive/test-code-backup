#ifndef _phone_service_h_
#define _phone_service_h_

#include "NTService.h"
#include "RecvData.h"
class CPhoneService:public CNTService
{
public:
	CPhoneService();

// ��д
public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();
	// ����
	virtual void Run(DWORD argc, LPTSTR * argv);
	// ֹͣ
	virtual void Stop();
// ʵ��
private:
	//ȷ��ֻ��һ������ʵ������
	HANDLE m_hServiceMutex;	
	CRecvData recvData;
};
#endif