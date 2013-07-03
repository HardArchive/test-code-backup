/******************************************************************************
Module:  Event.h
Notices: Copyright (c) 2006 Bruce Liang
Purpose: 封装Win32事件内核对象, 用于在设置及读取变量时同步线程.
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
		//创建一个不能被继承（pSecurity NULL）自动将复原为无信号状态（bManualReset）初始状态为无信号（bInitialState）无名(pszName)的事件对象
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

