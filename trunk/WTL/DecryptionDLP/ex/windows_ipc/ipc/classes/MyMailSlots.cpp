// MyMailSlots.cpp: ʵ��CMyMailSlots��.
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

	// Ϊ������Ϣ����mailslot�ļ����.
	m_hSMS_Slot=CreateFile(szMailslotName,
		GENERIC_READ|GENERIC_WRITE, 
		FILE_SHARE_READ| FILE_SHARE_WRITE,NULL, OPEN_EXISTING, 
        FILE_ATTRIBUTE_ARCHIVE|FILE_FLAG_WRITE_THROUGH,
		// FILE_FLAG_WRITE_THROUGH �趨����.
		NULL); 

	// ���mailslot�ļ��Ƿ��,������û�д�,��ֹ����.
	if (m_hSMS_Slot == INVALID_HANDLE_VALUE)
		TRACE("Unable to create mailslot. Error: %d\n", GetLastError());
}

//---------------------------------------------------------------------------
// ���ڿͻ�����ĺ���.
void CMyMailSlots::ClientSend(CString strRequest) 
{
	DWORD bufferLen=100;
	BOOL bStatus;
	DWORD dwNumBytesWritten;
	m_strRequest=strRequest;

	char toSendTxt[100];
 	
	// ���ɷ��͵��ַ���.
	strcpy(toSendTxt, strRequest);
	
	TRACE("Sending...\n");

	// ��mailslotд����Ϣ.
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
// ���ڷ������ĺ���.
void CMyMailSlots::ServerCreateMailslot()
{
    LPCSTR szMailslotName= TEXT("\\\\.\\mailslot\\sms");

	// Ϊ������Ϣ����mailslot.
	m_hSMS_Slot=CreateMailslot(szMailslotName,
		0, 0, (LPSECURITY_ATTRIBUTES) NULL);
	
	// ���mailslot�Ƿ񱻴���.
	if (m_hSMS_Slot == INVALID_HANDLE_VALUE)
	{
		TRACE("Unable to create mailslot. Error: %d\n",GetLastError());
		return;
	}

	m_pThread=AfxBeginThread(ServerReadProc, this); // �����߳�.
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

	// ���������Ϣֱ��������ֹ.
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
		
		// �������Ϣ,��ȡ��.
		if (dwMsgs&&dwNextSize!=-1)
		{
			// ��ȡ��Ϣ������ȡ�Ƿ�ɹ�.
			if (!ReadFile(Parent->m_hSMS_Slot, toDisptxt, dwNextSize,
				&dwNumBytesRead, (LPOVERLAPPED) NULL))
			{
				TRACE("Unable to read from mailslot. Error: %d\n",GetLastError());
				CloseHandle(Parent->m_hSMS_Slot);
				return 1;
			}
			else{
				// ������Ϣ.
				Parent->m_strRequest=toDisptxt;
			}
		}
		else
			// ÿ�������μ������Ϣ.
			Sleep(500);
	} 

	return 0;
}
