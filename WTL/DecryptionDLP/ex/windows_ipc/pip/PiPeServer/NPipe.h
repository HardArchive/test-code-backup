//file npipe.h
/***************************************************************************************************
* 1、 File       ： npipe.h  --namepipi
* 2、 Version    ： 1.0
* 3、 Description： 命名管道通信类
* 4、 Author     ： RG (http://www.9cpp.com/)
* 5、 Created    ： 2013-7-22 17:34:09
* 6、 History    ：
* 7、 Remark     ：
****************************************************************************************************/

#ifndef __NPIPE_H__
#define __NPIPE_H__
#include <string>
using namespace std;

namespace RG
{
#ifdef _UNICODE
#define tstring wstring
#else
#define tstring string
#endif

	class CNamedPipe
	{
	public:
		CNamedPipe()
		{
			m_hPipe = INVALID_HANDLE_VALUE;
		}
		~CNamedPipe()
		{

		}

	public:
		bool IsOpen() const
		{
			return (m_hPipe != INVALID_HANDLE_VALUE);
		}

		void Close()
		{
			if (!IsOpen()) return;
			::CloseHandle(m_hPipe);
			m_hPipe = INVALID_HANDLE_VALUE;
		}

		//判断是否为客户端
		bool IsClientPipe() const
		{           
			if (!IsOpen()) return false; 
			DWORD dwFlags = 0;
			//获得指定命名管道的信息
			if (!::GetNamedPipeInfo(m_hPipe, &dwFlags, NULL, NULL, NULL)) return false;

			return ((dwFlags & PIPE_CLIENT_END) != 0);
		}

		//判断是否为服务器
		bool IsServerPipe() const
		{            
			if (!IsOpen()) return false;
			DWORD dwFlags = 0;
			if (!::GetNamedPipeInfo(m_hPipe, &dwFlags, NULL, NULL, NULL))	return false;

			return ((dwFlags & PIPE_SERVER_END) != 0);
		}

		bool Flush()
		{            
			if (!IsOpen()) return false;
			//针对指定的文件句柄，刷新内部文件缓冲区 
			if (!::FlushFileBuffers(m_hPipe)) return false;
			return true;
		}

		DWORD Write(LPCVOID lpBuffer, DWORD dwNumberOfBytesToWrite, DWORD dwNumberOfBytesWritten = 0,
			LPOVERLAPPED lpOverlapped = NULL)
		{            
			if (!IsOpen()) return 0; 

			if (!::WriteFile(m_hPipe, lpBuffer, dwNumberOfBytesToWrite, &dwNumberOfBytesWritten, lpOverlapped))
				return 0;
			return dwNumberOfBytesWritten;
		}

		DWORD Write(LPCVOID lpBuffer, DWORD dwNumberOfBytesToWrite,
			LPOVERLAPPED lpOverlapped, LPOVERLAPPED_COMPLETION_ROUTINE lpCompletionRoutine)
		{            
			if (!IsOpen()) return 0;

			if (!::WriteFileEx(m_hPipe, lpBuffer, dwNumberOfBytesToWrite, lpOverlapped, lpCompletionRoutine))
				return 0;
			return dwNumberOfBytesToWrite;
		}

		DWORD Read(LPVOID lpBuffer, DWORD dwNumberOfBytesToRead, DWORD dwNumberOfBytesRead = 0,
			LPOVERLAPPED lpOverlapped = NULL)
		{            
			if (!IsOpen()) return 0;

			if (!::ReadFile(m_hPipe, lpBuffer, dwNumberOfBytesToRead, &dwNumberOfBytesRead, lpOverlapped))
				return 0;
			return dwNumberOfBytesRead;
		}

		DWORD Read(LPVOID lpBuffer, DWORD dwNumberOfBytesToRead,
			LPOVERLAPPED lpOverlapped,  LPOVERLAPPED_COMPLETION_ROUTINE lpCompletionRoutine)
		{            
			if (!IsOpen()) return 0;

			if (!::ReadFileEx(m_hPipe, lpBuffer, dwNumberOfBytesToRead, lpOverlapped, lpCompletionRoutine))
				return 0;
			return dwNumberOfBytesToRead;
		}

		/////////////////////////////////////////服务器端专用函数//////////////////////////////////////////////////
		/***************************************************************************************************
		1、 Function Name ： Create
		2、 Description   ： 创建一个命名管道。返回的句柄由管道的服务器端使用 
		3、 Parameters    ： LPCTSTR lpszName, 指定管道名，采用(\\.\管道\管道名)的形式。最多256个字符，不区分大小写。如果存在同名管道,则创建那个管道的一个新实例 
		DWORD dwOpenMode, 
		下述常数之一（对于管道的所有实例都只能且必选一样）：
		PIPE_ACCESS_DUPLEX 管道是双向的 PIPE_ACCESS_INBOUND 数据从客户端流到服务器端  PIPE_ACCESS_OUTBOUND 数据从服务器端流到客户端 
		下述常数的任意组合
		FILE_FLAG_WRITE_THROUGH 在网络中建立的字节型管道内，强迫数据在每次读写操作的时候通过网络传输。否则传输就可能延迟
		FILE_FLAG_OVERLAPPED 允许（但不要求）用这个管道进行异步（重叠式）操作 
		常数WRITE_DAC， WRITE_OWNER 和 ACCESS_ SYSTEM_SECURITY提供了附加的安全选项 
		DWORD dwPipeMode, 
		下述常数之一（管道的所有实例都必须指定相同的常数）
		PIPE_TYPE_BYTE 数据作为一个连续的字节数据流写入管道
		PIPE_TYPE_MESSAGE 数据用数据块（名为“消息”或“报文”）的形式写入管道
		下述常数之一：
		PIPE_READMODE_BYTE 数据以单独字节的形式从管道中读出
		PIPE_READMODE_MESSAGE 数据以名为“消息”的数据块形式从管道中读出（要求指定PIPE_TYPE_MESSAGE）
		下述常数之一：
		PIPE_WAIT 同步操作在等待的时候挂起线程
		PIPE_NOWAIT（不推荐！） 同步操作立即返回。这样可为异步传输提供一种落后的实现方法，已由Win32的重叠式传输机制取代了 
		DWORD dwMaxInstances, 这个管道能够创建的最大实例数量。必须是1到常数PIPE_UNLIMITED_INSTANCES间的一个值。它对于管道的所有实例来说都应是相同的
		DWORD dwOutBufferSize,建议的输出缓冲区长度；零表示用默认设置
		DWORD dwInBufferSize, 建议的输入缓冲区长度；零表示用默认设置 
		DWORD dwDefaultTimeOut, 管道的默认等待超时。对一个管道的所有实例来说都应相同 
		LPSECURITY_ATTRIBUTES lpSecurityAttributes 指定一个SECURITY_ATTRIBUTES结构，或者传递零值（将参数声明为ByVal As Long，并传递零值），以便使用不允许继承的一个默认描述符 
		4、 Return Value  ： 成功返回true失败返回false
		5、 Dynamic memory： 无
		6、 Author        ：  RG 
		7、 Created       ： 2013-7-23 13:47:40
		8、 Remark        ： 此函数由服务器调用
		****************************************************************************************************/
		bool Create(LPCTSTR lpszName, DWORD dwOpenMode = PIPE_ACCESS_DUPLEX, DWORD dwPipeMode = PIPE_TYPE_BYTE | PIPE_READMODE_BYTE , 
			DWORD dwMaxInstances = 1, DWORD dwOutBufferSize = 0, DWORD dwInBufferSize = 0, DWORD dwDefaultTimeOut = 1000, 
			LPSECURITY_ATTRIBUTES lpSecurityAttributes = NULL)
		{
			if (IsOpen()) return false;
			if (!_tcslen(lpszName)) return false;

			TCHAR tszPipeName[MAX_PATH] = {0};
			_stprintf_s(tszPipeName, MAX_PATH * sizeof(TCHAR), _T("\\\\.\\PIPE\\%s"), lpszName);

			//创建一个命名管道。返回的句柄由管道的服务器端使用 INVALID_HANDLE_VALUE表示失败。会设置GetLastError 
			m_hPipe = ::CreateNamedPipe(tszPipeName, dwOpenMode, dwPipeMode, dwMaxInstances, dwOutBufferSize, dwInBufferSize, dwDefaultTimeOut, lpSecurityAttributes);
			if (m_hPipe == INVALID_HANDLE_VALUE) return false;
			return true;
		}
		//指示一台服务器等待下去，直至客户机同一个命名管道连接
		bool ConnectClient(LPOVERLAPPED lpOverlapped = NULL)
		{       
			if (IsOpen()) return false;
			//必须是服务器端调用
			if (!IsServerPipe()) return false;

			if (!::ConnectNamedPipe(m_hPipe, lpOverlapped))	return false;
			return true;
		}

		//断开一个客户与一个命名管道的连接
		bool DisconnectClient()
		{
			if (IsOpen()) return false;
			//必须是服务器端调用
			if (!IsServerPipe()) return false;

			if (!::DisconnectNamedPipe(m_hPipe)) return false;
			return true;
		}

		tstring GetClientUserName() const
		{            
			if (IsOpen()) return "";
			//必须是服务器端调用
			if (!IsServerPipe()) return "";

			TCHAR pszUserName[_MAX_PATH];
			if (!::GetNamedPipeHandleState(m_hPipe, NULL, NULL, NULL, NULL, pszUserName, _MAX_PATH))
				return "";

			return tstring(pszUserName);
		}


		/////////////////////////////////////////客户端专用函数//////////////////////////////////////////////////
		//打开管道
		bool Open(LPCTSTR lpszPipeName, LPCTSTR lpszServerName = ".", DWORD dwDesiredAccess = GENERIC_READ | GENERIC_WRITE,
			DWORD dwShareMode = 0, LPSECURITY_ATTRIBUTES lpSecurityAttributes = NULL,
			DWORD dwFlagsAndAttributes = FILE_ATTRIBUTE_NORMAL)
		{
			if (IsOpen()) return false;
			if (!_tcslen(lpszServerName)) return false;
			if (!_tcslen(lpszPipeName)) return false;

			TCHAR tszPipeName[MAX_PATH] = {0};
			_stprintf_s(tszPipeName, MAX_PATH * sizeof(TCHAR), _T("\\\\%s\\PIPE\\%s"), lpszServerName, lpszPipeName);

			m_hPipe = CreateFile(tszPipeName, dwDesiredAccess, dwShareMode, lpSecurityAttributes, OPEN_EXISTING, dwFlagsAndAttributes, NULL);
			if (m_hPipe == INVALID_HANDLE_VALUE) return false;
			return true;
		}

		//等待服务器开启  检查服务器活动状态  在打开客户端之前调用
		bool ServerAvailable(LPCTSTR lpszPipeName, LPCTSTR lpszServerName = ".", DWORD dwTimeOut = 5000)
		{
			TCHAR tszPipeName[MAX_PATH] = {0};
			_stprintf_s(tszPipeName, MAX_PATH * sizeof(TCHAR), _T("\\\\%s\\PIPE\\%s"), lpszServerName, lpszPipeName);

			//客户进程用来连接到一个命名管道
			if (!::WaitNamedPipe(tszPipeName, dwTimeOut)) return false;
			return true;				
		}

	protected:
		HANDLE m_hPipe;
	};
}
#endif /*__NPIPE_H__*/