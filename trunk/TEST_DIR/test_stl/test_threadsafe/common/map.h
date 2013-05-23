//file map.h
/***************************************************************************************************
* 1�� File       �� map.h
* 2�� Version    �� *.*
* 3�� Description�� ����C++��׼ģ���STL��map�����������̰߳�ȫmap��ʵ�ֶ��еĻ������ܣ�����ӣ����ӣ��������ö������ֵ�����������߳�ͬ��
* 4�� Author     �� RG (http://www.9cpp.com/)
* 5�� Created    �� 2013-4-28 15:42:35
* 6�� History    �� 
* 7�� Remark     �� �����������̵߳Ļ���
*					���⣺��ָĳһ��Դͬʱֻ����һ�������߶�����з��ʣ�����Ψһ�Ժ������ԡ��������޷����Ʒ����߶���Դ�ķ���˳�򣬼�����������ġ�
*					һ��������ֻ������һ����Դ�Ļ������
****************************************************************************************************/
#ifndef __MUTEX_UTIL_H__
#define __MUTEX_UTIL_H__

#ifndef __GENERAL_HELPER_H__
#include "GeneralHelper.h"
#endif

#include <map>
namespace RG
{
	namespace SAFESTL
	{
		template<class _Kty,
		class _Ty,
		class _Pr = less<_Kty>,
		class _Alloc = allocator<pair<const _Kty, _Ty> > >
		class CMap : protected std::map<_Kty, _Ty, _Pr, _Alloc>
		{
		public:
			iterator begin()
			{	// return iterator for beginning of mutable sequence
				return (_TREE_ITERATOR(_Lmost()));
			}

			iterator end()
			{	// return iterator for end of mutable sequence
				return (_TREE_ITERATOR(_Myhead));
			}

			void clear()
			{	// erase all

#if _HAS_ITERATOR_DEBUGGING
				this->_Orphan_ptr(*this, 0);
#endif /* _HAS_ITERATOR_DEBUGGING */

				_Erase(_Root());
				_Root() = _Myhead, _Mysize = 0;
				_Lmost() = _Myhead, _Rmost() = _Myhead;
			}
			size_type size() const
			{	// return length of sequence
				return (_Mysize);
			}

		};
	}
}
#endif /*__MUTEX_UTIL_H__*/

