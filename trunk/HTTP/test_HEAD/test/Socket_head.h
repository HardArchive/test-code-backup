#ifndef _SOCKET_HEAD_H_
#define _SOCKET_HEAD_H_

/******************************************************************************* 
* 1、 Class      ： Socket_head
* 2、 Version    ： 1.00
* 3、 Description:  用于向网站发送Head请求验证域名可用性
* 4、 Author     ： QNA (http://www.9cpp.com/)
* 5、 Created    ： 2012-8-2 17:14:30
* 6、 History    ： 
* 7、 Remark     ： 套接字在构函数初始化，折构函数释放  适用于需要长期连接
********************************************************************************/ 
#include "Socket.h"
namespace QNA
{

	class CSocketHEAD
	{
	public:
		bool CheckEffective()
		{
			return false;
		}

	private:
		SOCKET m_sock;
		bool m_bIsClient;        //客户端标志，true为客户端，false为服务端
	};


}

#endif  //_SOCKET_HEAD_H_