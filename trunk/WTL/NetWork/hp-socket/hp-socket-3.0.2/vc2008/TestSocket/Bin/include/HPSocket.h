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

/******************************************************************************
Module:  HPSocket DLL

Usage:
		����һ��
		--------------------------------------------------------------------------------------
		0. Ӧ�ó������ SocketInterface.h �� HPSocket.h ͷ�ļ�
		1. ���� HP_Create_Xxx() �������� HPSocket ����
		2. ʹ����Ϻ���� HP_Destroy_Xxx() �������� HPSocket ����

		��������
		--------------------------------------------------------------------------------------
		0. Ӧ�ó������ SocketInterface.h �� HPSocket.h ͷ�ļ�
		1. ���� CXxxWrapper ��װ����ͨ����װ������ָ��ʹ�� HPSocket ����

Release:
		1. Bin/x86/HPSocket.dll		- (32λ/MBCS/Release)
		2. Bin/x86/HPSocket_D.dll	- (32λ/MBCS/DeBug)
		3. Bin/x86/HPSocket_U.dll	- (32λ/UNICODE/Release)
		4. Bin/x86/HPSocket_UD.dll	- (32λ/UNICODE/DeBug)
		5. Bin/x64/HPSocket.dll		- (64λ/MBCS/Release)
		6. Bin/x64/HPSocket_D.dll	- (64λ/MBCS/DeBug)
		7. Bin/x64/HPSocket_U.dll	- (64λ/UNICODE/Release)
		8. Bin/x64/HPSocket_UD.dll	- (64λ/UNICODE/DeBug)

******************************************************************************/

#pragma once

/**************************************************/
/********* imports / exports HPSocket.dll *********/

#ifdef HPSOCKET_EXPORTS
	#define HPSOCKET_API __declspec(dllexport)
#else
	#define HPSOCKET_API __declspec(dllimport)
#endif

#include "SocketInterface.h"

/**************************************************/
/************** HPSocket.dll �������� **************/

// ���� ITcpServer ����
EXTERN_C HPSOCKET_API ITcpServer* HP_Create_TcpServer(ITcpServerListener* pListener);
// ���� ITcpClient ����
EXTERN_C HPSOCKET_API ITcpClient* HP_Create_TcpClient(ITcpClientListener* pListener);
// ���� ITcpPullServer ����
EXTERN_C HPSOCKET_API ITcpPullServer* HP_Create_TcpPullServer(ITcpServerListener* pListener);
// ���� ITcpPullClient ����
EXTERN_C HPSOCKET_API ITcpPullClient* HP_Create_TcpPullClient(ITcpClientListener* pListener);
// ���� IUdpServer ����
EXTERN_C HPSOCKET_API IUdpServer* HP_Create_UdpServer(IUdpServerListener* pListener);
// ���� IUdpClient ����
EXTERN_C HPSOCKET_API IUdpClient* HP_Create_UdpClient(IUdpClientListener* pListener);

// ���� ITcpServer ����
EXTERN_C HPSOCKET_API void HP_Destroy_TcpServer(ITcpServer* pServer);
// ���� ITcpClient ����
EXTERN_C HPSOCKET_API void HP_Destroy_TcpClient(ITcpClient* pClient);
// ���� ITcpPullServer ����
EXTERN_C HPSOCKET_API void HP_Destroy_TcpPullServer(ITcpPullServer* pServer);
// ���� ITcpPullClient ����
EXTERN_C HPSOCKET_API void HP_Destroy_TcpPullClient(ITcpPullClient* pClient);
// ���� IUdpServer ����
EXTERN_C HPSOCKET_API void HP_Destroy_UdpServer(IUdpServer* pServer);
// ���� IUdpClient ����
EXTERN_C HPSOCKET_API void HP_Destroy_UdpClient(IUdpClient* pClient);

// ITcpServer ���󴴽���
struct TcpServer_Creator
{
	static ITcpServer* Create(ITcpServerListener* pListener)
	{
		return HP_Create_TcpServer(pListener);
	}

	static void Destroy(ITcpServer* pServer)
	{
		return HP_Destroy_TcpServer(pServer);
	}
};

// ITcpClient ���󴴽���
struct TcpClient_Creator
{
	static ITcpClient* Create(ITcpClientListener* pListener)
	{
		return HP_Create_TcpClient(pListener);
	}

	static void Destroy(ITcpClient* pClient)
	{
		return HP_Destroy_TcpClient(pClient);
	}
};

// ITcpPullServer ���󴴽���
struct TcpPullServer_Creator
{
	static ITcpPullServer* Create(ITcpServerListener* pListener)
	{
		return HP_Create_TcpPullServer(pListener);
	}

	static void Destroy(ITcpPullServer* pServer)
	{
		return HP_Destroy_TcpPullServer(pServer);
	}
};

// ITcpPullClient ���󴴽���
struct TcpPullClient_Creator
{
	static ITcpPullClient* Create(ITcpClientListener* pListener)
	{
		return HP_Create_TcpPullClient(pListener);
	}

	static void Destroy(ITcpPullClient* pClient)
	{
		return HP_Destroy_TcpPullClient(pClient);
	}
};

// IUdpServer ���󴴽���
struct UdpServer_Creator
{
	static IUdpServer* Create(IUdpServerListener* pListener)
	{
		return HP_Create_UdpServer(pListener);
	}

	static void Destroy(IUdpServer* pServer)
	{
		return HP_Destroy_UdpServer(pServer);
	}
};

// IUdpClient ���󴴽���
struct UdpClient_Creator
{
	static IUdpClient* Create(IUdpClientListener* pListener)
	{
		return HP_Create_UdpClient(pListener);
	}

	static void Destroy(IUdpClient* pClient)
	{
		return HP_Destroy_UdpClient(pClient);
	}
};

/**************************************************/
/*************** HPSocket �����װ�� ***************/

template<class T, class _Listener, class _Creator> class CHPWrapper
{
public:
	CHPWrapper(_Listener* pListener = nullptr)
	{
		if(pListener)
			m_pObj = _Creator::Create(pListener);
		else
			m_pObj = nullptr;
	}

	~CHPWrapper()
	{
		Reset();
	}

public:
	CHPWrapper&  Reset(T* pObj = nullptr)
	{
		if(pObj != m_pObj)
		{
			if(m_pObj)
				_Creator::Destroy(m_pObj);

			m_pObj = pObj;
		}

		return *this;
	}

	CHPWrapper& Attach(T* pObj)
	{
		return Reset(pObj);
	}

	T* Detach()
	{
		T* pObj	= m_pObj;
		m_pObj	= nullptr;

		return pObj;
	}

	BOOL IsValid	()	const	{return m_pObj != nullptr	;}
	T* Get			()	const	{return m_pObj				;}
	T* operator ->	()	const	{return m_pObj				;}
	operator T*		()	const	{return m_pObj				;}

	CHPWrapper& operator =	(T* pObj)	{return Reset(pObj)	;}

private:
	CHPWrapper(const CHPWrapper&);
	CHPWrapper& operator = (const CHPWrapper&);

private:
	T* m_pObj;
};

// ITcpServer �����װ��
typedef CHPWrapper<ITcpServer, ITcpServerListener, TcpServer_Creator>			CTcpServerWrapper;
// ITcpClient �����װ��
typedef CHPWrapper<ITcpClient, ITcpClientListener, TcpClient_Creator>			CTcpClientWrapper;
// ITcpPullServer �����װ��
typedef CHPWrapper<ITcpPullServer, ITcpServerListener, TcpPullServer_Creator>	CTcpPullServerWrapper;
// ITcpPullClient �����װ��
typedef CHPWrapper<ITcpPullClient, ITcpClientListener, TcpPullClient_Creator>	CTcpPullClientWrapper;
// IUdpServer �����װ��
typedef CHPWrapper<IUdpServer, IUdpServerListener, UdpServer_Creator>			CUdpServerWrapper;
// IUdpClient �����װ��
typedef CHPWrapper<IUdpClient, IUdpClientListener, UdpClient_Creator>			CUdpClientWrapper;
