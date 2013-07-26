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

		//�ж��Ƿ�Ϊ�ͻ���
		bool IsClientPipe() const
		{           
			if (!IsOpen()) return false; 
			DWORD dwFlags = 0;
			//���ָ�������ܵ�����Ϣ
			if (!::GetNamedPipeInfo(m_hPipe, &dwFlags, NULL, NULL, NULL)) return false;

			return ((dwFlags & PIPE_CLIENT_END) != 0);
		}

		//�ж��Ƿ�Ϊ������
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
			//���ָ�����ļ������ˢ���ڲ��ļ������� 
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

		/////////////////////////////////////////��������ר�ú���//////////////////////////////////////////////////
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
		4�� Return Value  �� �ɹ�����trueʧ�ܷ���false
		5�� Dynamic memory�� ��
		6�� Author        ��  RG 
		7�� Created       �� 2013-7-23 13:47:40
		8�� Remark        �� �˺����ɷ���������
		****************************************************************************************************/
		bool Create(LPCTSTR lpszName, DWORD dwOpenMode = PIPE_ACCESS_DUPLEX, DWORD dwPipeMode = PIPE_TYPE_BYTE | PIPE_READMODE_BYTE , 
			DWORD dwMaxInstances = 1, DWORD dwOutBufferSize = 0, DWORD dwInBufferSize = 0, DWORD dwDefaultTimeOut = 1000, 
			LPSECURITY_ATTRIBUTES lpSecurityAttributes = NULL)
		{
			if (IsOpen()) return false;
			if (!_tcslen(lpszName)) return false;

			TCHAR tszPipeName[MAX_PATH] = {0};
			_stprintf_s(tszPipeName, MAX_PATH * sizeof(TCHAR), _T("\\\\.\\PIPE\\%s"), lpszName);

			//����һ�������ܵ������صľ���ɹܵ��ķ�������ʹ�� INVALID_HANDLE_VALUE��ʾʧ�ܡ�������GetLastError 
			m_hPipe = ::CreateNamedPipe(tszPipeName, dwOpenMode, dwPipeMode, dwMaxInstances, dwOutBufferSize, dwInBufferSize, dwDefaultTimeOut, lpSecurityAttributes);
			if (m_hPipe == INVALID_HANDLE_VALUE) return false;
			return true;
		}
		//ָʾһ̨�������ȴ���ȥ��ֱ���ͻ���ͬһ�������ܵ�����
		bool ConnectClient(LPOVERLAPPED lpOverlapped = NULL)
		{       
			if (IsOpen()) return false;
			//�����Ƿ������˵���
			if (!IsServerPipe()) return false;

			if (!::ConnectNamedPipe(m_hPipe, lpOverlapped))	return false;
			return true;
		}

		//�Ͽ�һ���ͻ���һ�������ܵ�������
		bool DisconnectClient()
		{
			if (IsOpen()) return false;
			//�����Ƿ������˵���
			if (!IsServerPipe()) return false;

			if (!::DisconnectNamedPipe(m_hPipe)) return false;
			return true;
		}

		tstring GetClientUserName() const
		{            
			if (IsOpen()) return "";
			//�����Ƿ������˵���
			if (!IsServerPipe()) return "";

			TCHAR pszUserName[_MAX_PATH];
			if (!::GetNamedPipeHandleState(m_hPipe, NULL, NULL, NULL, NULL, pszUserName, _MAX_PATH))
				return "";

			return tstring(pszUserName);
		}


		/////////////////////////////////////////�ͻ���ר�ú���//////////////////////////////////////////////////
		//�򿪹ܵ�
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

		//�ȴ�����������  ���������״̬  �ڴ򿪿ͻ���֮ǰ����
		bool ServerAvailable(LPCTSTR lpszPipeName, LPCTSTR lpszServerName = ".", DWORD dwTimeOut = 5000)
		{
			TCHAR tszPipeName[MAX_PATH] = {0};
			_stprintf_s(tszPipeName, MAX_PATH * sizeof(TCHAR), _T("\\\\%s\\PIPE\\%s"), lpszServerName, lpszPipeName);

			//�ͻ������������ӵ�һ�������ܵ�
			if (!::WaitNamedPipe(tszPipeName, dwTimeOut)) return false;
			return true;				
		}

	protected:
		HANDLE m_hPipe;
	};
}
#endif /*__NPIPE_H__*/