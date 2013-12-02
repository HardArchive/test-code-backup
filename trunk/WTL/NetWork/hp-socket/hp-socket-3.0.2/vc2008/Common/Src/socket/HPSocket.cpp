/*
 * Copyright Bruce Liang (ldcsaa@gmail.com)
 *
 * Version	: 3.0.2
 * Author	: Bruce Liang
 * Website	: http://www.jessma.org
 * Porject	: https://code.google.com/p/ldcsaa
 * Bolg		: http://www.cnblogs.com/ldcsaa
 * WeiBo	: http://weibo.com/u/1402935851
 * QQ Group	: 75375912
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */
 
#include "stdafx.h"
#include "HPSocket.h"
#include "TcpServer.h"
#include "TcpClient.h"
#include "TcpPullServer.h"
#include "TcpPullClient.h"
#include "UdpServer.h"
#include "UdpClient.h"

EXTERN_C HPSOCKET_API ITcpServer* HP_Create_TcpServer(ITcpServerListener* pListener)
{
	return new CTcpServer(pListener);
}

EXTERN_C HPSOCKET_API ITcpClient* HP_Create_TcpClient(ITcpClientListener* pListener)
{
	return new CTcpClient(pListener);
}

EXTERN_C HPSOCKET_API ITcpPullServer* HP_Create_TcpPullServer(ITcpServerListener* pListener)
{
	return (ITcpPullServer*)(new CTcpPullServer(pListener));
}

EXTERN_C HPSOCKET_API ITcpPullClient* HP_Create_TcpPullClient(ITcpClientListener* pListener)
{
	return (ITcpPullClient*)(new CTcpPullClient(pListener));
}

EXTERN_C HPSOCKET_API IUdpServer* HP_Create_UdpServer(IUdpServerListener* pListener)
{
	return new CUdpServer(pListener);
}

EXTERN_C HPSOCKET_API IUdpClient* HP_Create_UdpClient(IUdpClientListener* pListener)
{
	return new CUdpClient(pListener);
}

EXTERN_C HPSOCKET_API void HP_Destroy_TcpServer(ITcpServer* pServer)
{
	delete pServer;
}

EXTERN_C HPSOCKET_API void HP_Destroy_TcpClient(ITcpClient* pClient)
{
	delete pClient;
}

EXTERN_C HPSOCKET_API void HP_Destroy_TcpPullServer(ITcpPullServer* pServer)
{
	delete pServer;
}

EXTERN_C HPSOCKET_API void HP_Destroy_TcpPullClient(ITcpPullClient* pClient)
{
	delete pClient;
}

EXTERN_C HPSOCKET_API void HP_Destroy_UdpServer(IUdpServer* pServer)
{
	delete pServer;
}

EXTERN_C HPSOCKET_API void HP_Destroy_UdpClient(IUdpClient* pClient)
{
	delete pClient;
}
