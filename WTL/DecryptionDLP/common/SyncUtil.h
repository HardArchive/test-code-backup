//file SyncUtil.h
/***************************************************************************************************
* 1¡¢ File       £º 
* 2¡¢ Version    £º *.*
* 3¡¢ Description£º 
* 4¡¢ Author     £º RG (http://www.9cpp.com/)
* 5¡¢ Created    £º 2013-4-27 15:29:41
* 6¡¢ History    £º 
* 7¡¢ Remark     £º 
****************************************************************************************************/
#ifndef __SYNC_UTIL_H__
#define __SYNC_UTIL_H__

namespace RG
{
	class CSyncUtil
	{
	public:
		// Operations
		virtual BOOL Lock(DWORD dwTimeout = INFINITE) = 0;
		virtual BOOL Unlock() = 0;
		virtual BOOL Unlock(LONG /* lCount */, LPLONG /* lpPrevCount=NULL */)

	};


//	class CSyncObject : public CObject
//	{
//		DECLARE_DYNAMIC(CSyncObject)
//
//		// Constructor
//	public:
//		explicit CSyncObject(LPCTSTR pstrName);
//
//		// Attributes
//	public:
//		operator HANDLE() const;
//		HANDLE  m_hObject;
//
//		// Operations
//		virtual BOOL Lock(DWORD dwTimeout = INFINITE);
//		virtual BOOL Unlock() = 0;
//		virtual BOOL Unlock(LONG /* lCount */, LPLONG /* lpPrevCount=NULL */)
//		{ return TRUE; }
//
//		// Implementation
//	public:
//		virtual ~CSyncObject();
//#ifdef _DEBUG
//		CString m_strName;
//		virtual void AssertValid() const;
//		virtual void Dump(CDumpContext& dc) const;
//#endif
//		friend class CSingleLock;
//		friend class CMultiLock;
//	};
}
#endif /*__SYNC_UTIL_H__*/