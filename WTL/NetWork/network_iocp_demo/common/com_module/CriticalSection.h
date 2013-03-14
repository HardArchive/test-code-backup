/****************************************************************************
*																			*
* CriticalSection.h 														*
*																			*
* Create by :																*
* Kingfisher	2003-10-15													*
* 																			*
* Description: 																*
* 封装Win32临界量对象和互斥量内核对象										*
****************************************************************************/

#pragma once

//临界区类
class CCriSec
{
public:
	CCriSec()		{::InitializeCriticalSection(&m_crisec);}
	~CCriSec()		{::DeleteCriticalSection(&m_crisec);}

	void Lock()		{::EnterCriticalSection(&m_crisec);}
	void Unlock()	{::LeaveCriticalSection(&m_crisec);}

private:
	CCriSec(const CCriSec& cs);
	CCriSec operator = (const CCriSec& cs);

private:
	CRITICAL_SECTION    m_crisec;
};

class CCriSec2
{
public:
	CCriSec2(BOOL bInitialize = TRUE)
	{
		if(bInitialize)
		{
			m_pcrisec = new CRITICAL_SECTION;
			::InitializeCriticalSection(m_pcrisec);
		}
		else
			m_pcrisec = NULL;
	}

	~CCriSec2() {Reset();}

	void Attach(CRITICAL_SECTION* pcrisec)
	{
		Reset();
		m_pcrisec = pcrisec;
	}

	CRITICAL_SECTION* Detach()
	{
		CRITICAL_SECTION* pcrisec = m_pcrisec;
		m_pcrisec = NULL;
		return pcrisec;
	}

	void Lock()		{::EnterCriticalSection(m_pcrisec);}
	void Unlock()	{::LeaveCriticalSection(m_pcrisec);}

private:
	CCriSec2(const CCriSec2& cs);
	CCriSec2 operator = (const CCriSec2& cs);

	void Reset()
	{
		if(m_pcrisec)
		{
			::DeleteCriticalSection(m_pcrisec);
			delete m_pcrisec;
			m_pcrisec = NULL;
		}
	}

private:
	CRITICAL_SECTION*    m_pcrisec;
};

//互斥类
class CMTX
{
public:
	CMTX(BOOL bInitialOwner = FALSE, LPCTSTR pszName = NULL, LPSECURITY_ATTRIBUTES pSecurity = NULL)	
	{
		m_hMutex = ::CreateMutex(pSecurity, bInitialOwner, pszName);
		ASSERT(IsValid());
	}

	~CMTX()
	{
		if(IsValid())
			::CloseHandle(m_hMutex);
	}

	BOOL Open(DWORD dwAccess, BOOL bInheritHandle, LPCTSTR pszName)
	{
		if(IsValid())
			VERIFY(::CloseHandle(m_hMutex));

		m_hMutex = ::OpenMutex(dwAccess, bInheritHandle, pszName);
		return(IsValid());
	}

	void Lock(DWORD dwMilliseconds = INFINITE)	{::WaitForSingleObject(m_hMutex, dwMilliseconds);}
	void Unlock()								{::ReleaseMutex(m_hMutex);}

	HANDLE& GetHandle	() 	{return m_hMutex;}
	operator HANDLE		()	{return m_hMutex;}
	HANDLE* operator &	()	{return &m_hMutex;}
	BOOL IsValid		()	{return m_hMutex != NULL;}

private:
	CMTX(const CMTX& mtx);
	CMTX operator = (const CMTX& mtx);

private:
	HANDLE m_hMutex;
};

//锁模版类
template<class CLockObj> class CLocalLock
{
public:
	CLocalLock(CLockObj& obj) : m_lock(obj) {m_lock.Lock();}
	~CLocalLock() {m_lock.Unlock();}
private:
	CLockObj& m_lock;
};

typedef CLocalLock<CCriSec>		CCriSecLock;
typedef CLocalLock<CCriSec2>	CCriSecLock2;
typedef CLocalLock<CMTX>		CMutexLock;
