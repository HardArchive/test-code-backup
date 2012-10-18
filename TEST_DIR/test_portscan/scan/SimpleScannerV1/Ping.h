// Ping.h: interface for the CPing class.
// Author AsianChai
// date   2005-5-20
// function Send icmp package to the destnation to determine wheather
// the victim is alive     
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_PING_H__63019DC0_41CC_4171_9391_4DDEEF729658__INCLUDED_)
#define AFX_PING_H__63019DC0_41CC_4171_9391_4DDEEF729658__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CPing  
{
public:
	BOOL HostReachable(CString strHost);
	CPing();
	virtual ~CPing();
	int SendEchoRequest(SOCKET s, LPSOCKADDR_IN lpstToAddr);
	

protected:
	int WaitForEchoReply(SOCKET s);
};

#endif // !defined(AFX_PING_H__63019DC0_41CC_4171_9391_4DDEEF729658__INCLUDED_)
