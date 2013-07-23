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
#ifndef ASSERT
#define ASSERT(f)	ATLASSERT(f)
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
            Close();
        }

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
		4、 Return Value  ： 无
		5、 Dynamic memory： 
		6、 Author        ：  RG 
		7、 Created       ： 2013-7-23 13:47:40
		8、 Remark        ： 
		****************************************************************************************************/
        void Create(LPCTSTR lpszName, DWORD dwOpenMode, DWORD dwPipeMode, DWORD dwMaxInstances, DWORD dwOutBufferSize,
                    DWORD dwInBufferSize, DWORD dwDefaultTimeOut, LPSECURITY_ATTRIBUTES lpSecurityAttributes = NULL)
        {
            ASSERT(!IsOpen());
            ASSERT(_tcslen(lpszName));

            TCHAR tszPipeName[MAX_PATH] = {0};
            _stprintf_s(tszPipeName, MAX_PATH * sizeof(TCHAR), _T("\\\\.\\PIPE\\%s"), lpszName);

			//创建一个命名管道。返回的句柄由管道的服务器端使用 INVALID_HANDLE_VALUE表示失败。会设置GetLastError 
            m_hPipe = ::CreateNamedPipe(tszPipeName, dwOpenMode, dwPipeMode, dwMaxInstances, dwOutBufferSize, dwInBufferSize, dwDefaultTimeOut, lpSecurityAttributes);
            if (m_hPipe == INVALID_HANDLE_VALUE)
                ThrowNamedPipeException();
        }

		//由客户端调用
        void Open(LPCTSTR lpszServerName, LPCTSTR lpszPipeName, DWORD dwDesiredAccess,
                  DWORD dwShareMode, LPSECURITY_ATTRIBUTES lpSecurityAttributes = NULL,
                  DWORD dwFlagsAndAttributes = 0)
        {
            
            ASSERT(!IsOpen());
            ASSERT(_tcslen(lpszServerName));
            ASSERT(_tcslen(lpszPipeName));

            //Construct the canonical pipe name
            TCHAR tszPipeName[MAX_PATH] = {0};
            _stprintf_s(tszPipeName, MAX_PATH * sizeof(TCHAR), _T("\\\\%s\\PIPE\\%s"), lpszServerName, lpszPipeName);


            m_hPipe = CreateFile(tszPipeName, dwDesiredAccess, dwShareMode, lpSecurityAttributes, OPEN_EXISTING, dwFlagsAndAttributes, NULL);
            if (m_hPipe == INVALID_HANDLE_VALUE)
                ThrowNamedPipeException();
        }

        operator HANDLE() const
        {
            return m_hPipe;
        };

        void Close()
        {
            if (IsOpen())
            {
                ::CloseHandle(m_hPipe);
                m_hPipe = INVALID_HANDLE_VALUE;
            }
        }

        void Attach(HANDLE hPipe)
        {
            Close();
            m_hPipe = hPipe;
        }

        HANDLE Detach()
        {
            HANDLE hReturn = m_hPipe;
            m_hPipe = INVALID_HANDLE_VALUE;
            return hReturn;
        }

        //一般功能
        void ConnectClient(LPOVERLAPPED lpOverlapped = NULL)
        {            
            ASSERT(IsOpen()); 
            ASSERT(IsServerPipe()); //Must be called from the server side

            if (!::ConnectNamedPipe(m_hPipe, lpOverlapped))
                ThrowNamedPipeException();
        }
        void DisconnectClient()
        {            
            ASSERT(IsOpen()); 
            ASSERT(IsServerPipe()); //Must be called from the server side

            if (!::DisconnectNamedPipe(m_hPipe))
                ThrowNamedPipeException();
        }
        void Flush()
        {            
            ASSERT(IsOpen()); 
			//针对指定的文件句柄，刷新内部文件缓冲区 
            if (!::FlushFileBuffers(m_hPipe))
                ThrowNamedPipeException();
        }

        DWORD Write(LPCVOID lpBuffer, DWORD dwNumberOfBytesToWrite,
                    LPOVERLAPPED lpOverlapped = NULL)
        {            
            ASSERT(IsOpen()); 

            DWORD dwNumberOfBytesWritten = 0;
            if (!::WriteFile(m_hPipe, lpBuffer, dwNumberOfBytesToWrite, &dwNumberOfBytesWritten, lpOverlapped))
                ThrowNamedPipeException();

            return dwNumberOfBytesWritten;
        }

        void Write(LPCVOID lpBuffer, DWORD dwNumberOfBytesToWrite, DWORD &dwNumberOfBytesWritten,
                    LPOVERLAPPED lpOverlapped = NULL)
        {            
            ASSERT(IsOpen()); 

            if (!::WriteFile(m_hPipe, lpBuffer, dwNumberOfBytesToWrite, &dwNumberOfBytesWritten, lpOverlapped))
                ThrowNamedPipeException();
        }

        void Write(LPCVOID lpBuffer, DWORD dwNumberOfBytesToWrite,
                    LPOVERLAPPED lpOverlapped, LPOVERLAPPED_COMPLETION_ROUTINE lpCompletionRoutine)
        {            
            ASSERT(IsOpen()); 

            if (!::WriteFileEx(m_hPipe, lpBuffer, dwNumberOfBytesToWrite, lpOverlapped, lpCompletionRoutine))
                ThrowNamedPipeException();
        }

        DWORD Read(LPVOID lpBuffer, DWORD dwNumberOfBytesToRead,
                   LPOVERLAPPED lpOverlapped = NULL)
        {            
            ASSERT(IsOpen()); 

            DWORD dwNumberOfBytesRead = 0;
            if (!::ReadFile(m_hPipe, lpBuffer, dwNumberOfBytesToRead, &dwNumberOfBytesRead, lpOverlapped))
                ThrowNamedPipeException();

            return dwNumberOfBytesRead;
        }

        void Read(LPVOID lpBuffer, DWORD dwNumberOfBytesToRead, DWORD &dwNumberOfBytesRead,
                   LPOVERLAPPED lpOverlapped = NULL)
        {            
            ASSERT(IsOpen()); 

            if (!::ReadFile(m_hPipe, lpBuffer, dwNumberOfBytesToRead, &dwNumberOfBytesRead, lpOverlapped))
                ThrowNamedPipeException();
        }

        void Read(LPVOID lpBuffer, DWORD dwNumberOfBytesToRead,
                   LPOVERLAPPED lpOverlapped,  LPOVERLAPPED_COMPLETION_ROUTINE lpCompletionRoutine)
        {            
            ASSERT(IsOpen()); 

            if (!::ReadFileEx(m_hPipe, lpBuffer, dwNumberOfBytesToRead, lpOverlapped, lpCompletionRoutine))
                ThrowNamedPipeException();
        }

        DWORD Peek(LPVOID lpBuffer, DWORD dwBufferSize, DWORD *lpdwTotalBytesAvail, DWORD *lpdwBytesLeftThisMessage)
        {            
            ASSERT(IsOpen()); 

            DWORD dwBytesRead = 0;
			//预览一个管道中的数据，或取得与管道中的数据有关的信息.
            if (!::PeekNamedPipe(m_hPipe, lpBuffer, dwBufferSize, &dwBytesRead, lpdwTotalBytesAvail, lpdwBytesLeftThisMessage))
                ThrowNamedPipeException();

            return dwBytesRead;
        }

        void Peek(LPVOID lpBuffer, DWORD dwBufferSize, DWORD *lpdwBytesRead, DWORD *lpdwTotalBytesAvail, DWORD *lpdwBytesLeftThisMessage)
        {            
            ASSERT(IsOpen()); 

            if (!::PeekNamedPipe(m_hPipe, lpBuffer, dwBufferSize, lpdwBytesRead, lpdwTotalBytesAvail, lpdwBytesLeftThisMessage))
                ThrowNamedPipeException();
        }

        void  Transact(LPVOID lpInBuffer, DWORD dwInBufferSize, LPVOID lpOutBuffer,
                       DWORD dwOutBufferSize, DWORD &dwBytesRead, LPOVERLAPPED lpOverlapped = NULL)
        {            
            ASSERT(IsOpen()); 
			//该函数在单独一个函数中同时合并了对管道的读、写操作。客户和服务器进程都可用它
            if (!::TransactNamedPipe(m_hPipe, lpInBuffer, dwInBufferSize, lpOutBuffer, dwOutBufferSize, &dwBytesRead, lpOverlapped))
                ThrowNamedPipeException();
        }

        BOOL IsOpen() const
        {
            return (m_hPipe != INVALID_HANDLE_VALUE);
        };

		//管道是否在阻塞模式
        BOOL IsBlockingPipe() const
        {            
            ASSERT(IsOpen()); 

            DWORD dwState = 0;
			//获取一个命名管道当前的状态信息
            if (!::GetNamedPipeHandleState(m_hPipe, &dwState, NULL, NULL, NULL, NULL, 0))
                ThrowNamedPipeException();

            return ((dwState & PIPE_NOWAIT) == 0);
        }

        BOOL IsClientPipe() const
        {            
            ASSERT(IsOpen()); 

            DWORD dwFlags = 0;
			//获得指定命名管道的信息
            if (!::GetNamedPipeInfo(m_hPipe, &dwFlags, NULL, NULL, NULL))
                ThrowNamedPipeException();

            return ((dwFlags & PIPE_CLIENT_END) != 0);
        }

        BOOL IsServerPipe() const
        {            
            ASSERT(IsOpen()); 

            DWORD dwFlags = 0;
			//PIPE_CLIENT_END 这个句柄是关于一个命名管道的客户端，此值被默认
			//PIPE_TYPE_BYTE 命名管道是一个字节管道型，此值被默认
            if (!::GetNamedPipeInfo(m_hPipe, &dwFlags, NULL, NULL, NULL))
                ThrowNamedPipeException();

            return ((dwFlags & PIPE_SERVER_END) != 0);
        }

        BOOL IsMessagePipe() const
        {            
            ASSERT(IsOpen()); 

            DWORD dwState = 0;
            if (!::GetNamedPipeHandleState(m_hPipe, &dwState, NULL, NULL, NULL, NULL, 0))
                ThrowNamedPipeException();

            return ((dwState & PIPE_READMODE_MESSAGE) != 0);
        }

        DWORD   GetCurrentInstances() const
        {            
            ASSERT(IsOpen()); 

            DWORD dwCurInstances = 0;
            if (!::GetNamedPipeHandleState(m_hPipe, NULL, &dwCurInstances, NULL, NULL, NULL, 0))
                ThrowNamedPipeException();

            return dwCurInstances;
        }

        DWORD   GetMaxCollectionCount() const
        {            
            ASSERT(IsOpen()); 
            ASSERT(IsClientPipe()); //Must be called from the client side

            DWORD dwMaxCollectionCount = 0;
            if (!::GetNamedPipeHandleState(m_hPipe, NULL, NULL, &dwMaxCollectionCount, NULL, NULL, 0))
                ThrowNamedPipeException();

            return dwMaxCollectionCount;
        }

        DWORD   GetCollectionTimeout() const
        {            
            ASSERT(IsOpen()); 
            ASSERT(IsClientPipe()); //Must be called from the client side

            DWORD dwCollectDataTimeout = 0;
            if (!::GetNamedPipeHandleState(m_hPipe, NULL, NULL, NULL, &dwCollectDataTimeout, NULL, 0))
                ThrowNamedPipeException();

            return dwCollectDataTimeout;
        }

        DWORD   GetOutboundBufferSize() const
        {            
            ASSERT(IsOpen()); 

            DWORD dwOutBufferSize = 0;
            if (!::GetNamedPipeInfo(m_hPipe, NULL, &dwOutBufferSize, NULL, NULL))
                ThrowNamedPipeException();

            return dwOutBufferSize;
        }


        DWORD GetInboundBufferSize() const
        {
            ASSERT(IsOpen()); 

            DWORD dwInBufferSize = 0;
            if (!::GetNamedPipeInfo(m_hPipe, NULL, NULL, &dwInBufferSize, NULL))
                ThrowNamedPipeException();
            return dwInBufferSize;
        }
		
		tstring GetClientUserName() const
        {            
            ASSERT(IsOpen()); 
            ASSERT(IsServerPipe()); //Must be called from the server side

            TCHAR pszUserName[_MAX_PATH];
            if (!::GetNamedPipeHandleState(m_hPipe, NULL, NULL, NULL, NULL, pszUserName, _MAX_PATH))
                ThrowNamedPipeException();

            return tstring(pszUserName);
        }

        DWORD GetMaxInstances() const
        {            
            ASSERT(IsOpen()); 

            DWORD dwMaxInstances = 0;
            if (!::GetNamedPipeInfo(m_hPipe, NULL, NULL, NULL, &dwMaxInstances))
                ThrowNamedPipeException();

            return dwMaxInstances;
        }


        //设置状态
        void SetMode(BOOL bByteMode, BOOL bBlockingMode)
        {            
            ASSERT(IsOpen()); 

            DWORD dwMode;
            if (bByteMode)
            {
                if (bBlockingMode)
                    dwMode = PIPE_READMODE_BYTE | PIPE_WAIT;
                else
                    dwMode = PIPE_READMODE_BYTE | PIPE_NOWAIT;
            }
            else
            {
                if (bBlockingMode)
                    dwMode = PIPE_READMODE_MESSAGE | PIPE_WAIT;
                else
                    dwMode = PIPE_READMODE_MESSAGE | PIPE_NOWAIT;
            }

			//设置与一个命名管道的运作有关的信息
            if (!::SetNamedPipeHandleState(m_hPipe, &dwMode, NULL, NULL))
                ThrowNamedPipeException();
        }

        void SetMaxCollectionCount(DWORD dwCollectionCount)
        {            
            ASSERT(IsOpen()); 
            ASSERT(IsClientPipe()); //Must be called from the client side

            if (!::SetNamedPipeHandleState(m_hPipe, NULL, &dwCollectionCount, NULL))
                ThrowNamedPipeException();
        }


        void SetCollectionTimeout(DWORD dwDataTimeout)
        {            
            ASSERT(IsOpen()); 
            ASSERT(IsClientPipe()); //Must be called from the client side

            if (!::SetNamedPipeHandleState(m_hPipe, NULL, NULL, &dwDataTimeout))
                ThrowNamedPipeException();
        }

		//静态方法
        static DWORD Call(LPCTSTR lpszServerName, LPCTSTR lpszPipeName, LPVOID lpInBuffer, DWORD dwInBufferSize, LPVOID lpOutBuffer,
                          DWORD dwOutBufferSize, DWORD dwTimeOut)
        {
            //What will be the return value
            DWORD dwBytesRead = 0;

            //Construct the canonical pipe name
			TCHAR tszPipeName[MAX_PATH] = {0};
			_stprintf_s(tszPipeName, MAX_PATH * sizeof(TCHAR), _T("\\\\%s\\PIPE\\%s"), lpszServerName, lpszPipeName);

			//呼叫对服务端的连接
            if (!::CallNamedPipe(tszPipeName, lpInBuffer, dwInBufferSize, lpOutBuffer, dwOutBufferSize, &dwBytesRead, dwTimeOut))
                ThrowNamedPipeException();

            return dwBytesRead;
        }

        static void ServerAvailable(LPCTSTR lpszServerName, LPCTSTR lpszPipeName, DWORD dwTimeOut)
        {
            //Construct the canonical pipe name
			TCHAR tszPipeName[MAX_PATH] = {0};
			_stprintf_s(tszPipeName, MAX_PATH * sizeof(TCHAR), _T("\\\\%s\\PIPE\\%s"), lpszServerName, lpszPipeName);

			//客户进程用来连接到一个命名管道
            if (!::WaitNamedPipe(tszPipeName, dwTimeOut))
                ThrowNamedPipeException();
        }

		//命名管道异常处理
       static void ThrowNamedPipeException(DWORD dwError = 0)
        {
            if (dwError == 0)
                dwError = ::GetLastError();

            //CNamedPipeException *pException = new CNamedPipeException(dwError);

            //TRACE(_T("Warning: throwing CNamedPipeException for error %d\n"), dwError);
            //THROW(pException);
        }


    protected:
        HANDLE m_hPipe;
    };
}
#endif /*__NPIPE_H__*/