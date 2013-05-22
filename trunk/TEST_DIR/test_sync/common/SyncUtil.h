//file SyncUtil.h
/***************************************************************************************************
* 1�� File       �� SyncUtil.h
* 2�� Version    �� 1.0
* 3�� Description�� 
* 4�� Author     �� RG (http://www.9cpp.com/)
* 5�� Created    �� 2013-4-27 15:29:41
* 6�� History    �� 
* 7�� Remark     �� 
****************************************************************************************************/
#ifndef __SYNC_UTIL_H__
#define __SYNC_UTIL_H__

#ifndef __GENERAL_HELPER_H__
#include "GeneralHelper.h"
#endif

#ifndef __CRITICALSECTION_UTIL_H__
#include "CriticalSection.h"
#endif

#ifndef __MUTEX_UTIL_H__
#include "Mutex.h"
#endif

namespace RG
{
	//class CSyncUtil
	//{
	//public:
	//	// Operations
	//	virtual BOOL Lock(DWORD dwTimeout = INFINITE) = 0;
	//	virtual BOOL Unlock() = 0;
	//	virtual BOOL Unlock(LONG /* lCount */, LPLONG /* lpPrevCount=NULL */)

	//};
	

	//��ģ���� �ں������ڲ�ʹ�� ����ʱ���� ���ʱ�ͷ�
	template<class CLockObj> class CLocalLock: public CLockObj
	{
	public:
		CLocalLock(CLockObj& obj) : m_lock(obj) {m_lock.Lock();}
		~CLocalLock() {m_lock.Unlock();}
	private:
		CLockObj& m_lock;
	};
	//
	//typedef CLocalLock<SYNC::CCriSec>	g_clsCriSecLock;       //ջ�ٽ�����
	//typedef CLocalLock<SYNC::CCriSec2>	g_clsCriSecLock2;      //���ٽ�����
	//typedef CLocalLock<SYNC::CMTX>		g_clsMutexLock;        //��������

	template<class CObject> class CObjectLock
	{
	public:
		CObjectLock(CObject pObject):m_pObject(pObject){}
		~CObjectLock(){}

		void Lock(){m_clsCriSec.Lock();}
		void UnLock(){m_clsCriSec.Unlock();}
		CObject* GetObject(){return &m_pObject}
	private:
		CObjectLock(const CObjectLock& ObjectLock);
		CObjectLock operator = (const CObjectLock& ObjectLock);

	public:
		CObject m_pObject;
	private:
		SYNC::CCriSec m_clsCriSec;
	};
	typedef CObjectLock<map<string, DWORD>>	mapLockBarcodeID;        
	 
}
#endif /*__SYNC_UTIL_H__*/