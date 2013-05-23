//file safemap.h
/***************************************************************************************************
* 1、 File       ： safemap.h
* 2、 Version    ： *.*
* 3、 Description： 基于C++标准模板块STL的map容器开发的线程安全map，实现队列的基本功能，如入队，出队，可以设置队列最大值，可以用于线程同步
* 4、 Author     ： RG (http://www.9cpp.com/)
* 5、 Created    ： 2013-4-28 15:42:35
* 6、 History    ： 
* 7、 Remark     ： 互斥量用于线程的互斥
*					互斥：是指某一资源同时只允许一个访问者对其进行访问，具有唯一性和排它性。但互斥无法限制访问者对资源的访问顺序，即访问是无序的。
*					一个互斥量只能用于一个资源的互斥访问
****************************************************************************************************/
#ifndef __SAFE_MAP_UTIL_H__
#define __SAFE_MAP_UTIL_H__

#ifndef __GENERAL_HELPER_H__
#include "GeneralHelper.h"
#endif

#include <map>
#include <queue>
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
	}
}
#endif /*__SAFE_MAP_UTIL_H__*/

