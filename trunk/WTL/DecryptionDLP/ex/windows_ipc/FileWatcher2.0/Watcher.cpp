// Watcher.cpp: implementation of the CWatcher class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "FileWatcher.h"
#include "Watcher.h"
#include "FileWatcherDlg.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CWatcher::CWatcher()
{

}

CWatcher::~CWatcher()
{
	this->Destroy();
}

//@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

//【名称】创建目录监控。
//【参数】csStrPath 监控目录的路径
//【描述】打开目录，保存句柄。创建空完成端口，保存句柄。
//	把目录句柄与完成端口关联，保存句柄。创建线程，并挂起，保存句柄。
//	投递第一次IO请求，调用Watch。
BOOL CWatcher::Create(CString csStrPath)
{
	//===========> 打开目录，保存句柄
	if ((this->m_hPath = CreateFile(
		csStrPath,
		GENERIC_READ | GENERIC_WRITE,
		FILE_SHARE_READ | FILE_SHARE_WRITE | FILE_SHARE_DELETE,
		NULL,
		OPEN_EXISTING,
		FILE_FLAG_BACKUP_SEMANTICS | FILE_FLAG_OVERLAPPED,
		NULL
        )) == INVALID_HANDLE_VALUE)
	{
		return FALSE;
	}
	//============> 创建新的完成端口
	if ((this->m_hIOCPApp = ::CreateIoCompletionPort(
		INVALID_HANDLE_VALUE,
		NULL,
		0,
		0
		)) == NULL)
	{
		this->CloseAllHandle();

		return FALSE;
	}
	//============> 关联完成端口，启用单线程
	if ((this->m_hIOCPNew = ::CreateIoCompletionPort(
		this->m_hPath,
		this->m_hIOCPApp,
		0,
		2
		)) == NULL)
	{
		this->CloseAllHandle();
		return FALSE;
	}
	//=========> 创建监控线程，挂起，保存句柄
	if ((this->m_hThread = ::CreateThread(
		NULL,
		0,
		CWatcher::WatchThreadProc,
		this,
		CREATE_SUSPENDED,
		NULL
		)) == NULL)
	{
		this->CloseAllHandle();
		return FALSE;
	}
	//=============> 投递IO请求
	if (this->Watch() == FALSE)
	{
		//=======> 强行退出线程，因为线程没有工作，所以是安全操作
		::TerminateThread(this->m_hThread, 0);

		this->CloseAllHandle();

		return FALSE;
	}
	//============> 恢复线程，让线程工作
	::ResumeThread(this->m_hThread);
	return TRUE;
}

//@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

//【名称】关闭所有打开的句柄
//【参数】无
//【描述】关闭新完成端口、关联完成端口、目录、线程句柄
VOID CWatcher::CloseAllHandle()
{
	::CloseHandle(this->m_hIOCPApp);
	::CloseHandle(this->m_hIOCPNew);
	::CloseHandle(this->m_hPath);
	::CloseHandle(this->m_hThread);
}

//@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

//【名称】监控
//【参数】无
//【描述】调用ReadDirectoryChangesW，相当于投递第一次IO请求
BOOL CWatcher::Watch()
{
	DWORD dwByteRet = 0;

	//==========> 清空OVERLAPPED结构
	::memset(&this->m_stOverlapped, 0, sizeof(OVERLAPPED));

	//=========> 监控目录
	if (::ReadDirectoryChangesW(
		this->m_hPath,
		this->m_szBuffer,
		WTH_BUF_LEN,
		TRUE,
		FILE_NOTIFY_CHANGE_FILE_NAME |
		FILE_NOTIFY_CHANGE_DIR_NAME |
		FILE_NOTIFY_CHANGE_ATTRIBUTES |
		FILE_NOTIFY_CHANGE_SIZE |
		FILE_NOTIFY_CHANGE_LAST_WRITE |
		FILE_NOTIFY_CHANGE_LAST_ACCESS |
		FILE_NOTIFY_CHANGE_CREATION |
		FILE_NOTIFY_CHANGE_SECURITY,
		&dwByteRet,
		&this->m_stOverlapped,
		NULL 
		) == FALSE)
	{
		return FALSE;
	}
	return TRUE;
}

//@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

//【名称】监控线程过程
//【参数】lpPara 创建线程时候传递的指针
//【描述】循环，查询完成端口状态，如果查询失败、触发OnError，中断。
//	如果dwByteRet为-1，那么说明收到，退出通知，中断
//	如果没有意外，处理缓冲区以后，触发OnWatch
DWORD WINAPI CWatcher::WatchThreadProc(LPVOID lpvPara)
{
	CWatcher *lpcsWatcher = NULL;
	DWORD dwByteRet = 0, dwIOKey = 0;
	LPOVERLAPPED lpstOverlapped = NULL;
	PFILE_NOTIFY_INFORMATION lpstNotify = NULL;
	char *lpszBuffer = NULL;
	CString csStrName1, csStrName2;

	lpcsWatcher = (CWatcher *)lpvPara;

	while (TRUE)
	{
		//========> 查询完成端口状态
		if (::GetQueuedCompletionStatus(
			lpcsWatcher->m_hIOCPApp,
			&dwByteRet,
			&dwIOKey,
			&lpstOverlapped,
			INFINITE
			) == TRUE)
		{
			//=========> 判断是否失败
			if (dwByteRet == 0)
			{
				lpcsWatcher->CloseAllHandle();
				break;
			}
			else
			{
				//========> 分配内存，复制内存
				lpszBuffer = new char[dwByteRet];
				memcpy(lpszBuffer, lpcsWatcher->m_szBuffer, dwByteRet);

				//======> 缓冲区并且转换成FILE_NOTIFY_INFORMATION结构
				lpstNotify = (PFILE_NOTIFY_INFORMATION)lpszBuffer;

				//========= 转换成CString，得到名称1
				csStrName1 = lpcsWatcher->WideCharToCString(
					lpstNotify->FileName,
					lpstNotify->FileNameLength);
				//========> 判断是否存在偏移
				if (lpstNotify->NextEntryOffset != 0)
				{
					//==========> 计算第二个字符偏移
					//======> 一定要记得是
					// (PFILE_NOTIFY_INFORMATION)(lpszBuffer + lpstNotify->NextEntryOffset)
					lpstNotify = 
						(PFILE_NOTIFY_INFORMATION)(lpszBuffer + 
						lpstNotify->NextEntryOffset);
					//========= 转换成CString，得到名称2
					csStrName2 = lpcsWatcher->WideCharToCString(
						lpstNotify->FileName,
						lpstNotify->FileNameLength);
				}

				//=======> 调用OnWatch，剩下的交给此过程处理
				lpcsWatcher->OnWatch(
					lpstNotify->Action, 
					csStrName1, 
					csStrName2);

				// 释放内存
				delete[] lpszBuffer;

				//======> 再次投递IO请求
				if (lpcsWatcher->Watch() == FALSE)
				{
					lpcsWatcher->CloseAllHandle();
				}
			}
		}
		else
		{
				lpcsWatcher->CloseAllHandle();
		}
	}
	return 0;
}

//@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

//【名称】宽字符转换成多字节CString
//【参数】lpszWideChar 宽字符缓冲区。nLength 缓冲区长度
//【描述】宽字符转换成多字节以后，再转换成CString对象，然后返回
CString CWatcher::WideCharToCString(LPCWSTR lpszWideChar, UINT nLength)
{
	char szString[MAX_PATH] = {0};

	//======> 转换
	WideCharToMultiByte(
		CP_ACP,
		0,
		lpszWideChar,
		nLength / 2,
		szString,
		MAX_PATH,
		NULL,
		NULL 
		);
	return szString;
}

//@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

//【名称】销毁目录监控
//【参数】无
//【描述】关闭句柄，让它在线程检测到到失败，安全退出
VOID CWatcher::Destroy()
{
	this->CloseAllHandle();
}

//@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

//【名称】监控过程
//【参数】dwAction 动作类型。csStrName 相关名称
//【描述】处理动作，不要调用Destroy
VOID CWatcher::OnWatch(DWORD dwAction, CString csStrName1, CString csStrName2)
{
	//========> 交托给窗口的过程处理
	((CFileWatcherDlg *)::AfxGetMainWnd())->OnWatch(
		dwAction,
		csStrName1,
		csStrName2);

}

