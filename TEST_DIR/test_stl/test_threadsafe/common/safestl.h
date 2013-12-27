//file safestl.h
/***************************************************************************************************
* 1、 File       ： safestl.h
* 2、 Version    ： *.*
* 3、 Description： 基于C++标准模板块STL的map queue list容器开发的线程安全map queue list，可以用于线程同步
* 4、 Author     ： RG (http://www.9cpp.com/)
* 5、 Created    ： 2013-4-28 15:42:35
* 6、 History    ： 在使用前 lock 用完之unlock
* 7、 Remark     ： 互斥量用于线程的互斥
*					互斥：是指某一资源同时只允许一个访问者对其进行访问，具有唯一性和排它性。但互斥无法限制访问者对资源的访问顺序，即访问是无序的。
*					一个互斥量只能用于一个资源的互斥访问
****************************************************************************************************/
#ifndef __SAFE_STL_UTIL_H__
#define __SAFE_STL_UTIL_H__

//#ifndef __GENERAL_HELPER_H__
//#include "GeneralHelper.h"
//#endif

#include <map>
#include <queue>
#include <list>
namespace RG
{
	namespace SAFESTL
	{
		template<class _Kty,
		class _Ty,
		class _Pr = less<_Kty>,
		class _Alloc = allocator<pair<const _Kty, _Ty> > >
		class CMap : public std::map<_Kty, _Ty, _Pr, _Alloc>
		{
		public:
			CMap()		{::InitializeCriticalSection(&m_stuCriSec);}
			~CMap()		{::DeleteCriticalSection(&m_stuCriSec);}

		public:
			void Lock()		{::EnterCriticalSection(&m_stuCriSec);}
			void Unlock()	{::LeaveCriticalSection(&m_stuCriSec);}

		private:
			CRITICAL_SECTION    m_stuCriSec;
		};

		template<class _Ty,
		class _Container = deque<_Ty> >
		class CQueue : public std::queue<_Ty, _Container>
		{
		public:
			CQueue()		{::InitializeCriticalSection(&m_stuCriSec);}
			~CQueue()		{::DeleteCriticalSection(&m_stuCriSec);}

		public:
			void Lock()		{::EnterCriticalSection(&m_stuCriSec);}
			void Unlock()	{::LeaveCriticalSection(&m_stuCriSec);}

		private:
			CRITICAL_SECTION    m_stuCriSec;
		};

		template<class _Ty,
		class _Ax = allocator<_Ty> >
		class CList : public std::list<_Ty, _Ax>
		{
		public:
			CList()		{::InitializeCriticalSection(&m_stuCriSec);}
			~CList()		{::DeleteCriticalSection(&m_stuCriSec);}

		public:
			void Lock()		{::EnterCriticalSection(&m_stuCriSec);}
			void Unlock()	{::LeaveCriticalSection(&m_stuCriSec);}

		private:
			CRITICAL_SECTION    m_stuCriSec;
		};
	}
}
#endif /*__SAFE_STL_UTIL_H__*/

