#ifndef _phone_client_service_h_
#define _phone_client_service_h_

#include "NTService.h"
#include "FileSearch.h"
class CClientService:public CNTService
{
public:
	CClientService();

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
	CFileSearch fileSearch;
};
#endif