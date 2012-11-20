// HttpSocket.cpp: implementation of the HttpSocket class.
// - Vinod Vijayan (30 june 2007)
//   (vinod_vijayanvin@hotmail.com)
// A generic class for sending HTTP HEAD/GET requests and storing 
// the header information of the related response.
// 0.2 Modification History:
// 27Nov08 strHttpResponse is allocated memory from heap and 
//		   deallocated in the class destructor.
// 29Nov08 HTTP Reqest parameters are passed to the method
//		   sendHttpRequest as an object of type HttpParams.
// 29Nov08 Added constant member RESLEN for length of response
//		   and related modifications.
// 29Nov08 HTTP Port specified in the URI gets preference over
//		   that specified in the HTTP port field.
// 30Nov08 Added support for HTTP Proxy connection in method 
//		   sendHttpRequest.
// 
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "HTTP Spy.h"
#include "HttpSocket.h"


#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

HttpSocket::HttpSocket() 
: RESLEN(1024)
{
	sock					= socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	sessionErrorCode		= 0;
	portNo					= 0;
	isPortInURI				= false;
	strHttpResponse			= new char[RESLEN];
	scheme					= "http://";
}

HttpSocket::HttpSocket(CString uri)
: RESLEN(1024){
	sock					= socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	sessionErrorCode		= 0;
	portNo					= 0;
	isPortInURI				= false;
	strHttpResponse			= new char[RESLEN];
	scheme					= "http://";
	this->uri				= uri;
}

HttpSocket::HttpSocket(CString uri, int portno)
: RESLEN(1024){
	sock					= socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	sessionErrorCode		= 0;
	portNo					= 0;
	isPortInURI				= false;
	strHttpResponse			= new char[RESLEN];
	scheme					= "http://";
	this->uri				= uri;
	this->portNo			= portNo;
}

HttpSocket::HttpSocket(HttpProxy &httpProxyObj)
: RESLEN(1024)
{
	sock					= socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	sessionErrorCode		= 0;
	portNo					= 0;
	isPortInURI				= false;
	strHttpResponse			= new char[RESLEN];
	scheme					= "http://";
	m_httpProxyObj.setIPAddr(httpProxyObj.getIPAddr());
	m_httpProxyObj.setPort(httpProxyObj.getPort());
}

HttpSocket::~HttpSocket()
{
	delete []strHttpResponse;
	strHttpResponse	=	0;
	closesocket(sock);
}

CString HttpSocket::getHost()
{
	return host;
}

int HttpSocket::getPortNo()
{
	return portNo;
}

CString HttpSocket::getResource()
{
	return resource;
}

// DNS lookup to get the IP address of the destination or host
int HttpSocket::getHostAddr( const char* pszhost,sockaddr_in& dest)
{
    // Initialize the destination host info block
    memset( &dest, 0, sizeof( dest ) );

    // Turn first passed parameter into an IP address to ping
    unsigned int addr = inet_addr( pszhost );
    //if its quad Address then OK
    if( addr != INADDR_NONE )
    {
        // It was a dotted quad number, so save result
        dest.sin_addr.s_addr	= addr;
        dest.sin_family			= AF_INET;
    }
    else
	{
        // Not in dotted quad form, so try and look it up
        hostent* hp = gethostbyname( pszhost );
        if ( hp != 0 )
		{
            // Found an address for that host, so save it
            memcpy( &(dest.sin_addr), hp->h_addr, hp->h_length );
            dest.sin_family = hp->h_addrtype;
        }
        else
		{
            // Not a recognized hostname either!
            printf("Failed to resolve %s\n", pszhost);
			sessionErrorCode = HostNotFound;
            return sessionErrorCode;
        }
    }

    return 0;
}

/** 
 * If the socket is readible then there is a response which can be read.
 * If not readible then after timeout return no response status
 */
int HttpSocket::IsSocketReadible(SOCKET &s)
{
	struct timeval Timeout;
	fd_set readfds;
	readfds.fd_count	= 1;
	readfds.fd_array[0]	= s;
	Timeout.tv_sec		= 5;
    Timeout.tv_usec		= 0;

	return(select(1, &readfds, NULL, NULL, &Timeout));
}

/**
 * Formulate the Http Request and send it.
 * Read the response for meta-information and parse the Server Configuration information.
 * Params : URL , port no and the method (HEAD or GET)
 */
int HttpSocket::sendHttpRequest(HttpParams *hq)
{
	CString strurl, useragent, httpRequest;
	HTTP_METHOD method;
	CONN_TYPE connType;
	int port=0, proxyport=0;
	char strPortno[6] = {0};

	strurl		= hq->getURI();
	method		= hq->getHttpMethod();
	useragent	= hq->getUserAgent();
	connType	= hq->getConnType();
	port		= hq->getHttpPort();

	if((sessionErrorCode = parseURL(strurl)) != 0)
		return sessionErrorCode;
	
	/** If the port is specified in the URI
	 *  that gets the priority over what is 
	 *  specified in the HTTP port edit box.
	 */
	if(isPortSpecified()==true)
		port		= portNo;
	else
		portNo		= port;
	/** If the port is not specified by the 
	 *	user in the HTTP port edit control
	 *	then use the default port 80
	 */

	if(isValidPortRange(port)==false)
	{
		sessionErrorCode = InvalidHTTPPortNo;
		return sessionErrorCode;
	}

	/** Construct the HTTP request to be sent to the
	 *	HTTP Server.
	 */
	if(method == HEAD)
		httpRequest = "HEAD ";
	else 
		httpRequest = "GET ";

	/** If the HTTP connection is through the proxy, the 
	 *  absolute URI of the resource requested needs to 
	 *  be send as a parmeter of the HTTP method in HTTP 
	 *  header.
	 */
	if(connType == THRU_PROXY)
	{
		/** The resource with http port needs to be given
		 *	as specified in the requested URI.
		 */
		if(isPortSpecified()==true)
		{
			itoa(port,strPortno,10);
			httpRequest = httpRequest	+	scheme + host + ":" + CString(strPortno) + resource	+ " HTTP/1.1\r\n";
		}
		else
			httpRequest = httpRequest	+	scheme + host + resource + " HTTP/1.1\r\n";
	}
	else
		httpRequest = httpRequest	+	resource				+ " HTTP/1.1\r\n";

	httpRequest = httpRequest	+	"Host: "				+ host + "\r\n";
	httpRequest = httpRequest	+	"User-Agent: "			+  useragent + "\r\n";
	httpRequest = httpRequest	+	"Accept: */*\r\n";
	httpRequest = httpRequest	+	"Accept-Language: en-us,en;q=0.5\r\n";
	httpRequest = httpRequest	+	"Connection: close\r\n"	+ "\r\n";

	
    struct sockaddr_in hostaddr;
	/**
	 * If connection is through HTTP Proxy then 
	 * TCP connection should be established with 
	 * it and then then HTTP requset needs to be
	 * sent on the connection.
	 */
	if(connType == THRU_PROXY)
	{
		if(getHostAddr(hq->getProxyIP(),hostaddr) != 0)
		{
			return sessionErrorCode;
		}
		hostaddr.sin_port = htons(hq->getProxyPort());
	}
	else
	{
		/** Connect to the host HTTP Server on the
		 * specified port no
		 */
		if(getHostAddr((LPCSTR)host,hostaddr) != 0)
		{
			return sessionErrorCode;
		}
		hostaddr.sin_port = htons(portNo);
	}

	if(connect(sock, (struct sockaddr *)&hostaddr, sizeof(hostaddr)) == SOCKET_ERROR)
	{
		if(connType == THRU_PROXY)
			sessionErrorCode = HTTPProxyConnErr;			
		else
			sessionErrorCode = HTTPSrvrConnErr;

        return sessionErrorCode;
	}
	
	// Send an HTTP request to the host at the specified port no
	int writtenBytes = send(sock, (LPCSTR) httpRequest, httpRequest.GetLength(),0);
	int readBytes    = 0;

	memset(strHttpResponse,0,RESLEN); // Clear the receive buffer

	if(writtenBytes <=0)
	{
		sessionErrorCode = ZeroBytesSent;
        return sessionErrorCode;
	}
	
	if(IsSocketReadible(sock)) {
		readBytes           = recvfrom(sock, strHttpResponse, RESLEN-1, 0, 0, 0);
		
		if(readBytes > 0) {
			strHttpResponse[readBytes] = '\0';
		}
	}
	
    return 0;
}

/**
 * Get the value corresponding to the key in the header
 */
int HttpSocket::getHeaderInfo(CString key, CString &value)
{
	int start = 0, startIndex = -1, endIndex = -1; 
    CString strRespFind  = key + ": ";
	CString cstrHttpResponse	= CString(strHttpResponse);
    
	if(cstrHttpResponse.GetLength() > 0)
	{
		int startIndex		= cstrHttpResponse.Find(strRespFind,start);
		if(startIndex!=-1)
		{
			int endIndex	= cstrHttpResponse.Find("\r\n", startIndex+2);
			value			= cstrHttpResponse.Mid(startIndex+strlen(strRespFind), endIndex-(startIndex+strlen(strRespFind)));
			return 0;
		}

	}
	return -1;	
}

CString HttpSocket::getErrorDesc()
{
	CString strPortNo;
	strPortNo.Format("%d", portNo);

	switch(sessionErrorCode)
	{
		case HostNotFound:
			return "Host " + host + " could not be found";

		case TcpConnectionError:
			return "Connection refused to " + host 
				    + " on port " + strPortNo;

		case HTTPSrvrConnErr:
			return "Connection refused to HTTP Server " 
				    + host + " on port " + strPortNo;

		case HTTPProxyConnErr:
			strPortNo.Format("%d", m_httpProxyObj.getPort());
			return "Connection refused to HTTP Proxy "
					+ m_httpProxyObj.getIPAddr() + 
					" on port " + strPortNo;

		case ZeroBytesSent:
			return "Not able to send any bytes to " + host;

		case ResourceNotFound:
			return resource + " was not found at " + host;

		case MalformedURI:
			return "The URI is malformed";

		case NonHTTPScheme:
			return "Non HTTP URI's are not supported";

		case InvalidHTTPPortNo:
			return "HTTP Port specified should be in the range 1 - 65535";

		case InvalidHTTPProxyPortNo:
			return "HTTP Proxy Port specified should be in the range 1 - 65535";
			
		default:
			return "Unknown Error";
			break;

	}
}

CString HttpSocket::getHttpResponse(void)
{
	return CString(strHttpResponse);
}

int HttpSocket::parseURL(CString strurl)
{
	int index = strurl.Find("/",7);
	/**
	 * Check if resource to be retrieved from 
	 * the host is specified. Seperate the host
	 * or authority from the resource and http 
	 * port specified if any.
	 * e.g.	http://www.abc.com:8080/
	 *		{
	 *			Scheme				:	http
	 *			Authority or host	:	www.abc.com
	 *			HTTP port on host	:	8080
	 *			Resource			:	/
	 *		 }
	 */
	if(index != -1){
		/** 
		 * If the scheme http is already specified
		 * in the HTTP URI,then discard it and 
		 * seperate the host and the specified 
		 * resource.
		 */
		if(strurl.Find("http://",0)==-1)
			host		= strurl.Mid(0, index);
		else
			host		= strurl.Mid(7, index-7);

		if(host.GetLength() != 0)
			resource	= strurl.Mid(index);
		else
		{
			host		= strurl.Mid(7);
			resource	= "/";
		}
	}
	else
	{
        if(strurl.Find("http://",0)==-1)
		{
			/** Only HTTP protocol scheme in the 
			 * specified URI will be accepted
			 */
			if(strurl.Find("://",0) != -1)
			{
				sessionErrorCode = NonHTTPScheme;
				return NonHTTPScheme;
			}
			host		= strurl.Mid(0);
		}
		else
			host		= strurl.Mid(7);
		
		//The default resource is /		  
		resource		= "/";
	}

	// If Port is present in the URI, seperate
	// it from the host
	if(host.Find(":") != -1) 
	{
		setPortSpecified(true);
		index				= host.Find(":");
		CString strPortno	= host.Mid(index+1);
		portNo              = atoi((LPCSTR)strPortno);
		if(isValidPortRange(portNo)==false)
		{
			sessionErrorCode = InvalidHTTPPortNo;
			return sessionErrorCode;
		}
		host				= host.Mid(0, index);
	}
		
	return 0;
}
