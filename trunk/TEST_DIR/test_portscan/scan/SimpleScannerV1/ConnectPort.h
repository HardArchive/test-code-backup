// ConnectPort.h: interface for the CConnectPort class.
// Author Asian Chai
// date   2005-05-21
// function: connect to the special port of the remote computer
// to see if it open 
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_CONNECTPORT_H__925016E1_5B52_4BCC_8E6C_1348A7B81A68__INCLUDED_)
#define AFX_CONNECTPORT_H__925016E1_5B52_4BCC_8E6C_1348A7B81A68__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CConnectPort  
{
public:
	BOOL PortConnect(CString strHost,int port,UINT flag);
	CConnectPort();
	virtual ~CConnectPort();

protected:
 UINT InitWsaDll(); //≥ı ºªØWSADll 
};

#endif // !defined(AFX_CONNECTPORT_H__925016E1_5B52_4BCC_8E6C_1348A7B81A68__INCLUDED_)
