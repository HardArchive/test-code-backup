//file Mutex.h
/***************************************************************************************************
* 1、 File       ： Mutex.h
* 2、 Version    ： *.*
* 3、 Description： 互斥量同步类
* 4、 Author     ： RG (http://www.9cpp.com/)
* 5、 Created    ： 2013-4-28 15:42:35
* 6、 History    ： 
* 7、 Remark     ： 
****************************************************************************************************/
#ifndef __MUTEX_UTIL_H__
#define __MUTEX_UTIL_H__

namespace RG
{

	class CMTX
	{
	public:
		CMTX(BOOL bInitialOwner = FALSE, LPCTSTR pszName = nullptr, LPSECURITY_ATTRIBUTES pSecurity = nullptr)	
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
		BOOL IsValid		()	{return m_hMutex != nullptr;}

	private:
		CMTX(const CMTX& mtx);
		CMTX operator = (const CMTX& mtx);

	private:
		HANDLE m_hMutex;
	};


}
#endif /*__MUTEX_UTIL_H__*/

