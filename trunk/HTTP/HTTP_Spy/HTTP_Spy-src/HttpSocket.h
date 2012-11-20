// HttpSocket.h: interface for the HttpSocket class.
// 30Nov08 - Added Class HttpProxy for compartmentization
//			 of proxy IP address and port no
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_HTTPSOCKET_H__331CB89A_46E3_4157_A1DE_B55DE1508E7D__INCLUDED_)
#define AFX_HTTPSOCKET_H__331CB89A_46E3_4157_A1DE_B55DE1508E7D__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

enum HTTP_METHOD{
	HEAD,
	GET
};

enum RESET_TYPE{
	SETTING,
	RESULT
};

enum CONN_TYPE{
	DIRECT,
	THRU_PROXY
};

/**
 * Class that stores information related
 * to the HTTP proxy server.
 */
class HttpProxy{
	CString m_IPAddr;
	unsigned int m_portNo;
	
	public:
	void setIPAddr(CString ipAddr) {
		m_IPAddr = ipAddr;
	}

	CString getIPAddr(void) {
		return m_IPAddr;
	}
	
	void setPort(unsigned int portNo) {
		m_portNo = portNo;
	}

	unsigned int getPort(void) {
		return m_portNo;
	}

	HttpProxy(){
		m_IPAddr = "";
		m_portNo = 8080;
	}

	HttpProxy(CString ipAddr, unsigned int portNo)
	{
		m_IPAddr = ipAddr;
		m_portNo = portNo;
	}

	HttpProxy(HttpProxy &httpProxyObj)
	{
		m_IPAddr = httpProxyObj.m_IPAddr;
		m_portNo = httpProxyObj.m_portNo;
	}
};

/** 
 * Class that represents the HTTP 
 * parameters that are passed from
 * the UI to the HTTP socket
 */
class HttpParams{
	CString m_strURI;
	unsigned int m_httpPort;
	enum HTTP_METHOD m_httpMethod; /* 0 = HEAD, 1 = GET*/
	enum CONN_TYPE m_connType;
	CString m_strUserAgent;
	HttpProxy m_httpProxyObj;


public:
	~HttpParams(){}
	//Set data member functions
	void setUserAgent(CString strUserAgent) { m_strUserAgent = strUserAgent;}
	void setURI(CString strURI) {m_strURI = strURI;}
	void setHttpPort(unsigned int httpPort) { 
		m_httpPort = httpPort; 
	}
	
	HttpProxy getHttpProxyObj() {
		return m_httpProxyObj;
	}
	
	void setProxyPort(unsigned int ProxyPort) { 
		m_httpProxyObj.setPort(ProxyPort);
	}
	
	void setHttpMethod(	enum HTTP_METHOD httpMethod) { m_httpMethod=httpMethod;}
	void setConnType( enum CONN_TYPE connType) { m_connType = connType;}
	//Get data member functions
	CString getURI(){ return m_strURI;}
	CString getUserAgent(){ return m_strUserAgent;}
	void setProxyIP(CString proxyIP) {
		m_httpProxyObj.setIPAddr(proxyIP);
	}
	CString getProxyIP() { return m_httpProxyObj.getIPAddr(); } 
	unsigned int getHttpPort(){ return m_httpPort;}
	unsigned int getProxyPort() { return m_httpProxyObj.getPort(); }
	enum HTTP_METHOD getHttpMethod(void) { return m_httpMethod;}
	enum CONN_TYPE getConnType(void) { return m_connType;}

	HttpParams(){
		m_strURI			= "";
		m_httpMethod		= HEAD;
		m_connType			= DIRECT;
		m_strUserAgent		= "";
		m_httpPort			= 80;
	}
};

class HttpSocket  
{
private:
	SOCKET	sock;
	CString	uri;
	CString scheme; // should be http 
	CString host;
	CString resource;
	int     portNo;
	bool	isPortInURI;
	char	*strHttpResponse;
	int     sessionErrorCode;
	const int RESLEN;
	HttpProxy m_httpProxyObj;


	enum errorCodes {
 		 HostNotFound=1, 
		 TcpConnectionError, 
		 HTTPSrvrConnErr,
		 HTTPProxyConnErr,
		 ResourceNotFound,
		 ZeroBytesSent,
		 NonHTTPScheme,
		 InvalidHTTPPortNo,
		 InvalidHTTPProxyPortNo,
		 MalformedURI,
	};

public:
	HttpSocket();
	HttpSocket(CString);
	HttpSocket(CString,int);
	HttpSocket(HttpProxy &);
	virtual ~HttpSocket();
	CString getResource();
	CString getHost();
	int getHeaderInfo(CString, CString &);
	int getPortNo();
	void setURI(CString);
	CString getURI();
	CString getErrorDesc();
	int parseURL(CString);
	int sendHttpRequest(HttpParams *httpParams);
	CString getHttpResponse();
	int getHostAddr( const char* pszHost,sockaddr_in& dest);
	int IsSocketReadible(SOCKET &);
	
	bool isValidPortRange(unsigned int port)
	{
		return ((port>0) && (port < 65536))?true:false;
	}

	bool isPortSpecified()
	{
		return isPortInURI;
	}
	
	void setPortSpecified(bool isPortInURI)
	{
		this->isPortInURI=isPortInURI;
	}
};

#endif // !defined(AFX_HTTPSOCKET_H__331CB89A_46E3_4157_A1DE_B55DE1508E7D__INCLUDED_)
