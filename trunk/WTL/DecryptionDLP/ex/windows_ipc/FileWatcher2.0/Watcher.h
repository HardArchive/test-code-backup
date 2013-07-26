// Watcher.h: interface for the CWatcher class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_WATCHER_H__59FB1B23_A774_4A4A_8D09_534FC5CFD9E1__INCLUDED_)
#define AFX_WATCHER_H__59FB1B23_A774_4A4A_8D09_534FC5CFD9E1__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#define WTH_BUF_LEN		(MAX_PATH * 2)

class CWatcher  
{ 
public:
	enum enmACTION
	{
		actCreate = FILE_ACTION_ADDED,
		actRemote = FILE_ACTION_REMOVED,
		actRenameNew = FILE_ACTION_RENAMED_NEW_NAME,
		actRenameOld = FILE_ACTION_RENAMED_OLD_NAME,
		actChange = FILE_ACTION_MODIFIED
	};
public:
	CWatcher();
	virtual ~CWatcher();
public:
	VOID Destroy();

	BOOL Create(CString csStrPath);
protected:
	virtual VOID OnWatch(DWORD dwAction, CString csStrName1, CString csStrName2);
private:
	BOOL Watch();
	static DWORD WINAPI WatchThreadProc(LPVOID lpvPara);
	VOID CloseAllHandle();
	CString WideCharToCString(LPCWSTR lpszWideChar, UINT nLength);
private:
	char m_szBuffer[WTH_BUF_LEN];		// ������
	OVERLAPPED m_stOverlapped;			// �ص��ṹ
	HANDLE m_hThread;					// �߳̾��
	HANDLE m_hIOCPNew;					// �µ���ɶ˿ھ��
	HANDLE m_hIOCPApp;					// ʹ�õ���ɶ˿ھ��
	HANDLE m_hPath;						// Ŀ¼���
};

#endif // !defined(AFX_WATCHER_H__59FB1B23_A774_4A4A_8D09_534FC5CFD9E1__INCLUDED_)
