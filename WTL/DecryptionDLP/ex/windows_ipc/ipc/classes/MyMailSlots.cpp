// MyMailSlots.cpp: 实现CMyMailSlots类.
//
#include "stdafx.h"
#include "MyMailSlots.h"

//---------------------------------------------------------------------------
CMyMailSlots::CMyMailSlots()
{
	m_pThread = NULL;
}

//---------------------------------------------------------------------------
CMyMailSlots::~CMyMailSlots()
{
	if(m_pThread){ 
		if(TerminateThread(m_pThread->m_hThread,0))
		{
			if(m_pThread) 
				delete	m_pThread;
			m_pThread = NULL;
		}
	}
}

//---------------------------------------------------------------------------
void CMyMailSlots::ClientCreateFile() 
{
    LPCSTR szMailslotName= TEXT("\\\\.\\mailslot\\sms");

	// 为发送消息创建mailslot文件句柄.
	m_hSMS_Slot=CreateFile(szMailslotName,
		GENERIC_READ|GENERIC_WRITE, 
		FILE_SHARE_READ| FILE_SHARE_WRITE,NULL, OPEN_EXISTING, 
        FILE_ATTRIBUTE_ARCHIVE|FILE_FLAG_WRITE_THROUGH,
		// FILE_FLAG_WRITE_THROUGH 设定阻塞.
		NULL); 

	// 检查mailslot文件是否打开,如果如果没有打开,终止程序.
	if (m_hSMS_Slot == INVALID_HANDLE_VALUE)
		TRACE("Unable to create mailslot. Error: %d\n", GetLastError());
}

//---------------------------------------------------------------------------
// 用于客户程序的函数.
void CMyMailSlots::ClientSend(CString strRequest) 
{
	DWORD bufferLen=100;
	BOOL bStatus;
	DWORD dwNumBytesWritten;
	m_strRequest=strRequest;

	char toSendTxt[100];
 	
	// 生成发送的字符串.
	strcpy(toSendTxt, strRequest);
	
	TRACE("Sending...\n");

	// 向mailslot写入信息.
	bStatus=WriteFile(m_hSMS_Slot,
		toSendTxt, (DWORD) sizeof(toSendTxt),
		&dwNumBytesWritten, (LPOVERLAPPED) NULL);
	
	if (!bStatus)
	{
		TRACE("Unable to write to mailslot. Error: %d\n",GetLastError());
        CloseHandle(m_hSMS_Slot);
	}
}

//---------------------------------------------------------------------------
// 用于服务程序的函数.
void CMyMailSlots::ServerCreateMailslot()
{
    LPCSTR szMailslotName= TEXT("\\\\.\\mailslot\\sms");

	// 为接收消息创建mailslot.
	m_hSMS_Slot=CreateMailslot(szMailslotName,
		0, 0, (LPSECURITY_ATTRIBUTES) NULL);
	
	// 检查mailslot是否被创建.
	if (m_hSMS_Slot == INVALID_HANDLE_VALUE)
	{
		TRACE("Unable to create mailslot. Error: %d\n",GetLastError());
		return;
	}

	m_pThread=AfxBeginThread(ServerReadProc, this); // 启动线程.
}

//---------------------------------------------------------------------------
UINT CMyMailSlots::ServerReadProc(LPVOID lpVoid) 
{
	DWORD dwNextSize;
	DWORD dwMsgs;
	DWORD dwNumBytesRead;
	BOOL  bStatus;

	char toDisptxt[100];
	
	CMyMailSlots* Parent=(CMyMailSlots*)lpVoid;

	// 反复检查消息直到程序终止.
	while(1)
	{
		bStatus=GetMailslotInfo(Parent->m_hSMS_Slot,
			(LPDWORD) NULL, &dwNextSize, &dwMsgs,
			(LPDWORD) NULL);
	
		if (!bStatus)
		{
			TRACE("Unable to get status. Error:%d\n",GetLastError());
			CloseHandle(Parent->m_hSMS_Slot);
			return 1;
		}
		
		TRACE("%d\n",dwNextSize);
		
		// 如果有消息,获取它.
		if (dwMsgs&&dwNextSize!=-1)
		{
			// 读取消息并检查读取是否成功.
			if (!ReadFile(Parent->m_hSMS_Slot, toDisptxt, dwNextSize,
				&dwNumBytesRead, (LPOVERLAPPED) NULL))
			{
				TRACE("Unable to read from mailslot. Error: %d\n",GetLastError());
				CloseHandle(Parent->m_hSMS_Slot);
				return 1;
			}
			else{
				// 保存消息.
				Parent->m_strRequest=toDisptxt;
			}
		}
		else
			// 每秒钟两次检查新消息.
			Sleep(500);
	} 

	return 0;
}
