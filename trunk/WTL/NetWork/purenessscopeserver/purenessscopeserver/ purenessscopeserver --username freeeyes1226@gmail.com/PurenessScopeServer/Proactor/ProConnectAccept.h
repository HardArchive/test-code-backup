#ifndef _PROACOOECTACCEPT_H
#define _PROACOOECTACCEPT_H

#include <vector>

using namespace std;

#include "ForbiddenIP.h"
#include "ProConsoleHandle.h"
#include "ProConnectHandle.h"

//���ں�̨�����Acceptor
class CProConsoleConnectAcceptor: public ACE_Asynch_Acceptor<CProConsoleHandle>
{
private:
	virtual CProConsoleHandle *make_handler (void);
	virtual int validate_connection (const ACE_Asynch_Accept::Result& result,
		const ACE_INET_Addr &remote,
		const ACE_INET_Addr& local);
};

//ƽ���ͻ��˵�Acceptor
class ProConnectAcceptor : public ACE_Asynch_Acceptor<CProConnectHandle>
{
private:
	virtual CProConnectHandle *make_handler (void);
	virtual int validate_connection (const ACE_Asynch_Accept::Result& result,
		                             const ACE_INET_Addr &remote,
		                             const ACE_INET_Addr& local);
};

class CProConnectAcceptManager
{
public:
	CProConnectAcceptManager(void);
	~CProConnectAcceptManager(void);

	bool InitConnectAcceptor(int nCount);
	void Close();
	int GetCount();
	ProConnectAcceptor* GetConnectAcceptor(int nIndex);
	const char* GetError();

private:
	typedef vector<ProConnectAcceptor*> vecProConnectAcceptor;
	vecProConnectAcceptor m_vecConnectAcceptor;
	int                   m_nAcceptorCount;
	char                  m_szError[MAX_BUFF_500];
};
#endif
