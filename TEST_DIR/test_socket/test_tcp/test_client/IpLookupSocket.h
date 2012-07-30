#ifndef _IPLOOKUP_SOCKET_H_
#define _IPLOOKUP_SOCKET_H_

#include "comm_protocol.h"
#include "Socket.h"

namespace QNA
{
	class CIpLookupSocket
	{
	public:
		CIpLookupSocket()
		{
			ZeroMemory(m_byDISendBuffer, 1024);
			ZeroMemory(m_byNDISendBuffer, 1024);
		}
		~CIpLookupSocket()
		{
			ZeroMemory(m_byDISendBuffer, 1024);
			ZeroMemory(m_byNDISendBuffer, 1024);
		}

	public:
		//从分发中心服务器获取IP段数据
		int DownLoadIpSection(LPCTSTR lpszIP, UINT uiPort, IPSection& stuIPSection)
		{
			Head stuHead = {0};
			//IPSection stuIPSection = {0};    //IP段数据

			BYTE byBuffer[1024] = {0};
			QNA::CSocket objSocket;

			objSocket.Connect(lpszIP, uiPort);  //连接

			//发送请求命令
			stuHead.dwFlag = 0x11223344;
			stuHead.dwCommand = 1;
			memcpy(byBuffer, &stuHead, sizeof(stuHead));
			if (1 != objSocket.Send(objSocket.GetSocket(), byBuffer, 1024))
			{
				TRACE("发送请求数据失败……\r\n");
				return -1;
			}

			ZeroMemory(&stuHead, sizeof(stuHead));
			ZeroMemory(byBuffer, 1024);
			//接收IP段数据
			if (1 != objSocket.Recv(objSocket.GetSocket(), byBuffer, 1024))
			{
				TRACE("接收数据失败……\r\n");
				return -2;
			}
			memcpy(&stuHead, byBuffer, sizeof(stuHead));
			if (0x11223344 != stuHead.dwFlag)
			{
				TRACE("接收数据内容出错……\r\n");
				return -3
			}

			if (2 != stuHead.dwCommand)
			{
				TRACE("接收数据内容出错……不是IP段数据\r\n");
				return -4;
			}
			memcpy(&stuIPSection, byBuffer+sizeof(Head), sizeof(stuIPSection));

			return 1;
		}

		//发送域名信息，bIsEnd 当前域名信息段是否结束
		bool SendDomainInfo(DomainInfo stuDomainInfo, bool bIsEnd)
		{

			return true;
		}


		//发送域名信息，bIsEnd 当前域名信息段是否结束
		bool SendNoDomainInfo(NoDomainInfo stuNoDomainInfo, bool bIsEnd)
		{

			return true;
		}

	private:
		//如果当前域名长度小于1024个字节则看能否追加一个信息，
		//返回false则发送已有buffer再行追加， ture则等待继续追加
		bool AppendData(DomainInfo stuDomainInfo, bool bIsEnd)
		{
			if (m_iDISBLen+sizeof(DomainInfo)>1024)
			{
				return false;
			}
			memcpy(m_byDISendBuffer+m_iDISBLen, &stuDomainInfo, sizeof(DomainInfo));
			m_iDISBLen += sizeof(DomainInfo);

			if (bIsEnd)
			{
				return false;
			}
			
		}

		bool AppendData(NoDomainInfo stuNoDomainInfo, bool bIsEnd)
		{

		}

		bool SendBuffer(int iConnectFlag)
		{
			if (1 == iConnectFlag)
			{

			}

		}
	
	private:
		//QNA::CSocket m_clsSocket;
		int m_iDISBLen;               //当前buffer（DISB）长度
		int m_iDISBFlag;              //发送数据状态   1为没有数据， 2为数据未满， 3为可以发送
		BYTE m_byDISendBuffer[1024];  //发送公安后台域名数据

		int m_iNDISBLen;                //当前域名段长度
		int m_iNDISBFlag;               //发送状态
		BYTE m_byNDISendBuffer[1024];   //发送公安后台无域名数据
	}
}
#endif  //_IPLOOKUP_SOCKET_H_