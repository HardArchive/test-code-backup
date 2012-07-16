#ifndef _phone_service_h_
#define _phone_service_h_

#include "NTService.h"
#include "RecvData.h"
class CPhoneService:public CNTService
{
public:
	CPhoneService();

// 重写
public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();
	// 运行
	virtual void Run(DWORD argc, LPTSTR * argv);
	// 停止
	virtual void Stop();
// 实现
private:
	//确保只有一个进程实例运行
	HANDLE m_hServiceMutex;	
	CRecvData recvData;
};
#endif