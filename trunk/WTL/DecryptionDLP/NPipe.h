//file npipe.h
/***************************************************************************************************
* 1�� File       �� npipe.h  --namepipi
* 2�� Version    �� 1.0
* 3�� Description�� �����ܵ�ͨ����
* 4�� Author     �� RG (http://www.9cpp.com/)
* 5�� Created    �� 2013-7-22 17:34:09
* 6�� History    ��
* 7�� Remark     ��
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
		1�� Function Name �� Create
		2�� Description   �� ����һ�������ܵ������صľ���ɹܵ��ķ�������ʹ�� 
		3�� Parameters    �� LPCTSTR lpszName, ָ���ܵ���������(\\.\�ܵ�\�ܵ���)����ʽ�����256���ַ��������ִ�Сд���������ͬ���ܵ�,�򴴽��Ǹ��ܵ���һ����ʵ�� 
		                     DWORD dwOpenMode, 
							  ��������֮һ�����ڹܵ�������ʵ����ֻ���ұ�ѡһ������
								PIPE_ACCESS_DUPLEX �ܵ���˫��� PIPE_ACCESS_INBOUND ���ݴӿͻ���������������  PIPE_ACCESS_OUTBOUND ���ݴӷ������������ͻ��� 
							  �����������������
								FILE_FLAG_WRITE_THROUGH �������н������ֽ��͹ܵ��ڣ�ǿ��������ÿ�ζ�д������ʱ��ͨ�����紫�䡣������Ϳ����ӳ�
								FILE_FLAG_OVERLAPPED ��������Ҫ��������ܵ������첽���ص�ʽ������ 
								����WRITE_DAC�� WRITE_OWNER �� ACCESS_ SYSTEM_SECURITY�ṩ�˸��ӵİ�ȫѡ�� 
							 DWORD dwPipeMode, 
							  ��������֮һ���ܵ�������ʵ��������ָ����ͬ�ĳ�����
								PIPE_TYPE_BYTE ������Ϊһ���������ֽ�������д��ܵ�
								PIPE_TYPE_MESSAGE ���������ݿ飨��Ϊ����Ϣ���򡰱��ġ�������ʽд��ܵ�
							  ��������֮һ��
								PIPE_READMODE_BYTE �����Ե����ֽڵ���ʽ�ӹܵ��ж���
								PIPE_READMODE_MESSAGE ��������Ϊ����Ϣ�������ݿ���ʽ�ӹܵ��ж�����Ҫ��ָ��PIPE_TYPE_MESSAGE��
							  ��������֮һ��
								PIPE_WAIT ͬ�������ڵȴ���ʱ������߳�
								PIPE_NOWAIT�����Ƽ����� ͬ�������������ء�������Ϊ�첽�����ṩһ������ʵ�ַ���������Win32���ص�ʽ�������ȡ���� 
							 DWORD dwMaxInstances, ����ܵ��ܹ����������ʵ��������������1������PIPE_UNLIMITED_INSTANCES���һ��ֵ�������ڹܵ�������ʵ����˵��Ӧ����ͬ��
							 DWORD dwOutBufferSize,�����������������ȣ����ʾ��Ĭ������
							 DWORD dwInBufferSize, ��������뻺�������ȣ����ʾ��Ĭ������ 
							 DWORD dwDefaultTimeOut, �ܵ���Ĭ�ϵȴ���ʱ����һ���ܵ�������ʵ����˵��Ӧ��ͬ 
							 LPSECURITY_ATTRIBUTES lpSecurityAttributes ָ��һ��SECURITY_ATTRIBUTES�ṹ�����ߴ�����ֵ������������ΪByVal As Long����������ֵ�����Ա�ʹ�ò�����̳е�һ��Ĭ�������� 
		4�� Return Value  �� ��
		5�� Dynamic memory�� 
		6�� Author        ��  RG 
		7�� Created       �� 2013-7-23 13:47:40
		8�� Remark        �� 
		****************************************************************************************************/
        void Create(LPCTSTR lpszName, DWORD dwOpenMode, DWORD dwPipeMode, DWORD dwMaxInstances, DWORD dwOutBufferSize,
                    DWORD dwInBufferSize, DWORD dwDefaultTimeOut, LPSECURITY_ATTRIBUTES lpSecurityAttributes = NULL)
        {
            ASSERT(!IsOpen());
            ASSERT(_tcslen(lpszName));

            TCHAR tszPipeName[MAX_PATH] = {0};
            _stprintf_s(tszPipeName, MAX_PATH * sizeof(TCHAR), _T("\\\\.\\PIPE\\%s"), lpszName);

			//����һ�������ܵ������صľ���ɹܵ��ķ�������ʹ�� INVALID_HANDLE_VALUE��ʾʧ�ܡ�������GetLastError 
            m_hPipe = ::CreateNamedPipe(tszPipeName, dwOpenMode, dwPipeMode, dwMaxInstances, dwOutBufferSize, dwInBufferSize, dwDefaultTimeOut, lpSecurityAttributes);
            if (m_hPipe == INVALID_HANDLE_VALUE)
                ThrowNamedPipeException();
        }

		//�ɿͻ��˵���
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

        //һ�㹦��
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
			//���ָ�����ļ������ˢ���ڲ��ļ������� 
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
			//Ԥ��һ���ܵ��е����ݣ���ȡ����ܵ��е������йص���Ϣ.
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
			//�ú����ڵ���һ��������ͬʱ�ϲ��˶Թܵ��Ķ���д�������ͻ��ͷ��������̶�������
            if (!::TransactNamedPipe(m_hPipe, lpInBuffer, dwInBufferSize, lpOutBuffer, dwOutBufferSize, &dwBytesRead, lpOverlapped))
                ThrowNamedPipeException();
        }

        BOOL IsOpen() const
        {
            return (m_hPipe != INVALID_HANDLE_VALUE);
        };

		//�ܵ��Ƿ�������ģʽ
        BOOL IsBlockingPipe() const
        {            
            ASSERT(IsOpen()); 

            DWORD dwState = 0;
			//��ȡһ�������ܵ���ǰ��״̬��Ϣ
            if (!::GetNamedPipeHandleState(m_hPipe, &dwState, NULL, NULL, NULL, NULL, 0))
                ThrowNamedPipeException();

            return ((dwState & PIPE_NOWAIT) == 0);
        }

        BOOL IsClientPipe() const
        {            
            ASSERT(IsOpen()); 

            DWORD dwFlags = 0;
			//���ָ�������ܵ�����Ϣ
            if (!::GetNamedPipeInfo(m_hPipe, &dwFlags, NULL, NULL, NULL))
                ThrowNamedPipeException();

            return ((dwFlags & PIPE_CLIENT_END) != 0);
        }

        BOOL IsServerPipe() const
        {            
            ASSERT(IsOpen()); 

            DWORD dwFlags = 0;
			//PIPE_CLIENT_END �������ǹ���һ�������ܵ��Ŀͻ��ˣ���ֵ��Ĭ��
			//PIPE_TYPE_BYTE �����ܵ���һ���ֽڹܵ��ͣ���ֵ��Ĭ��
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


        //����״̬
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

			//������һ�������ܵ��������йص���Ϣ
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

		//��̬����
        static DWORD Call(LPCTSTR lpszServerName, LPCTSTR lpszPipeName, LPVOID lpInBuffer, DWORD dwInBufferSize, LPVOID lpOutBuffer,
                          DWORD dwOutBufferSize, DWORD dwTimeOut)
        {
            //What will be the return value
            DWORD dwBytesRead = 0;

            //Construct the canonical pipe name
			TCHAR tszPipeName[MAX_PATH] = {0};
			_stprintf_s(tszPipeName, MAX_PATH * sizeof(TCHAR), _T("\\\\%s\\PIPE\\%s"), lpszServerName, lpszPipeName);

			//���жԷ���˵�����
            if (!::CallNamedPipe(tszPipeName, lpInBuffer, dwInBufferSize, lpOutBuffer, dwOutBufferSize, &dwBytesRead, dwTimeOut))
                ThrowNamedPipeException();

            return dwBytesRead;
        }

        static void ServerAvailable(LPCTSTR lpszServerName, LPCTSTR lpszPipeName, DWORD dwTimeOut)
        {
            //Construct the canonical pipe name
			TCHAR tszPipeName[MAX_PATH] = {0};
			_stprintf_s(tszPipeName, MAX_PATH * sizeof(TCHAR), _T("\\\\%s\\PIPE\\%s"), lpszServerName, lpszPipeName);

			//�ͻ������������ӵ�һ�������ܵ�
            if (!::WaitNamedPipe(tszPipeName, dwTimeOut))
                ThrowNamedPipeException();
        }

		//�����ܵ��쳣����
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