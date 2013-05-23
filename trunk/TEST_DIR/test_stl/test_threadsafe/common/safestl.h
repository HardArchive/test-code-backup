//file safemap.h
/***************************************************************************************************
* 1�� File       �� safemap.h
* 2�� Version    �� *.*
* 3�� Description�� ����C++��׼ģ���STL��map�����������̰߳�ȫmap��ʵ�ֶ��еĻ������ܣ�����ӣ����ӣ��������ö������ֵ�����������߳�ͬ��
* 4�� Author     �� RG (http://www.9cpp.com/)
* 5�� Created    �� 2013-4-28 15:42:35
* 6�� History    �� 
* 7�� Remark     �� �����������̵߳Ļ���
*					���⣺��ָĳһ��Դͬʱֻ����һ�������߶�����з��ʣ�����Ψһ�Ժ������ԡ��������޷����Ʒ����߶���Դ�ķ���˳�򣬼�����������ġ�
*					һ��������ֻ������һ����Դ�Ļ������
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

