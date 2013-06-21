/******************************************************************************
Module:  Event.h
Notices: Copyright (c) 2006 Bruce Liang
Purpose: ��װWin32�¼��ں˶���, ���������ü���ȡ����ʱͬ���߳�.
Desc:
Usage:
******************************************************************************/

#pragma once

class CEvt
{
public:
	CEvt(BOOL bManualReset = FALSE, BOOL bInitialState = FALSE, LPCTSTR pszName = NULL, LPSECURITY_ATTRIBUTES pSecurity = NULL)
	{
		m_hEvent = ::CreateEvent(pSecurity, bManualReset, bInitialState, pszName);
		//����һ�����ܱ��̳У�pSecurity NULL���Զ�����ԭΪ���ź�״̬��bManualReset����ʼ״̬Ϊ���źţ�bInitialState������(pszName)���¼�����
		ASSERT(IsValid());
	}

	~CEvt()
	{
		if(IsValid())
			VERIFY(::CloseHandle(m_hEvent));
	}

	BOOL Open(DWORD dwAccess, BOOL bInheritHandle, LPCTSTR pszName)
	{
		if(IsValid())
			VERIFY(::CloseHandle(m_hEvent));

		m_hEvent = ::OpenEvent(dwAccess, bInheritHandle, pszName);
		return(IsValid());
	}

	BOOL Pulse()	{return(::PulseEvent(m_hEvent));}
	BOOL Reset()	{return(::ResetEvent(m_hEvent));}
	BOOL Set()		{return(::SetEvent(m_hEvent));}

	HANDLE& GetHandle	() 	{return m_hEvent;}
	operator HANDLE		()	{return m_hEvent;}
	HANDLE* operator &	()	{return &m_hEvent;}
	BOOL IsValid		()	{return m_hEvent != NULL;}

private:
	CEvt(const CEvt&);
	CEvt operator = (const CEvt&);

private:
	HANDLE m_hEvent;
};

