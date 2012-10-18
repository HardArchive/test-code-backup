// Result.h: interface for the CResult class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_RESULT_H__207B95C6_9A2F_4E6D_A4DA_6F3145D3EE88__INCLUDED_)
#define AFX_RESULT_H__207B95C6_9A2F_4E6D_A4DA_6F3145D3EE88__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CResult  
{
public:
	void SignTime();
	void WritePingHeader(int hostNum);
	void WritePort(CString strHost, long existTcpPort[],long TcpLength, long existUdpPort[],long UdpLength);
	void WriteDetailLine(CString strHost, int TcpNum, long TcpPort[1024], int UdpNum, long UdpPort[1024]);
	void WriteString(CString str );
	CResult();
	virtual ~CResult();
	void WriteHeader(int HostNum,int TcpNum, int UdpNum);
protected:
//	CFile file;
	CString m_strFileName;
	CString m_strWriteToFile;

	

};

#endif // !defined(AFX_RESULT_H__207B95C6_9A2F_4E6D_A4DA_6F3145D3EE88__INCLUDED_)
