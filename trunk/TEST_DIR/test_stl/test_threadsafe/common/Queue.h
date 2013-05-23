//file Queue.h
/***************************************************************************************************
* 1�� File       �� TSQueue.h
* 2�� Version    �� *.*
* 3�� Description�� Thread Safe Queue
* 4�� Author     �� RG (http://www.9cpp.com/)
* 5�� Created    �� 2013-5-22 14:22:20
* 6�� History    �� 
* 7�� Remark     �� 
****************************************************************************************************/
#ifndef __QUEUE_UTIL_H__
#define __QUEUE_UTIL_H__

#define QUEUE_LOCK_MAX_TRY_TIMES       400  
#include <deque>

namespace RG
{
	namespace UTIL
	{
		// TEMPLATE CLASS queue
		template<class _Ty,
		class _Container = deque<_Ty> >
		class queue
		{	// FIFO queue implemented with a container
		public:
			typedef _Container container_type;
			typedef typename _Container::value_type value_type;
			typedef typename _Container::size_type size_type;
			typedef typename _Container::reference reference;
			typedef typename _Container::const_reference const_reference;

			queue()
				: c()
			{	// construct with empty container
			}

			explicit queue(const _Container& _Cont)
				: c(_Cont)
			{	// construct by copying specified container
			}

			bool empty() const
			{	// test if queue is empty
				return (c.empty());
			}

			size_type size() const
			{	// return length of queue
				return (c.size());
			}

			reference front()
			{	// return first element of mutable queue
				return (c.front());
			}

			const_reference front() const
			{	// return first element of nonmutable queue
				return (c.front());
			}

			reference back()
			{	// return last element of mutable queue
				return (c.back());
			}

			const_reference back() const
			{	// return last element of nonmutable queue
				return (c.back());
			}

			void push(const value_type& _Val)
			{	// insert element at beginning
				c.push_back(_Val);
			}

			void pop()
			{	// erase element at end
				c.pop_front();
			}

			const _Container& _Get_container() const
			{	// get reference to container
				return (c);
			}

		private:
			// �ӻ�������  
			// ����ӵ������������ڶദ����ƽ̨�ϣ�  
			// �������һ���̶��ϱ����߳��������л���ʹ���ܸ���ɫ��  
			// ���������ڳ���QUEUE_LOCK_MAX_TRY_TIMES��ʧ�ܺ�ͻ����̡߳� 
			inline void Lock()
			{
#ifndef RG_QUEUE_UNSAFE  
				DWORD dwTried = 0;  
				//����ԭ�Ӳ����ķ�ʽ������������a, b��������a��ǰ��ֵ
				//�������̣߳����̣߳����ã棨���������̣߳��ȵ��õ�InterlockedExchange(&g, TRUE);�̣߳��ٵ���ʱ������InterlockedExchange()�ܷ��أԣңգţ����̣߳�sleep(0);
				//��ֱ���̣߳�����InterlockedExchange(&g, FALSE);���̣߳��ſ������ڵ���InterlockedExchange(&g,, TRUE)�����أƣ��̣ӣŶ��˳�ѭ�����Ź�����
				//�����ڽ��������Ĳ���������ܲ����������ݶ������������顣��Ȼ���ַ������˷ѣ���ʱ�䣬��Ϊ����Ҫ���ϵ�ִ��InterlockedExchange()������
				while(InterlockedExchange(&_Locked, true))  
				{  
					dwTried++;  
					if(QUEUE_LOCK_MAX_TRY_TIMES == dwTried)  
					{  
						//����CPUʱ��Ƭ
#if _WIN32_WINNT >= 0x0400  
						SwitchToThread();  
#else 
						Sleep(0);  
#endif  
						dwTried = 0;  
					}  
				}  
#endif  
			}

			// ���������   
			inline void Unlock()  
			{  
#ifndef RG_QUEUE_UNSAFE  
				InterlockedExchange(&_Locked, false);  
#endif  
			}  

		protected:
			volatile LONG   _Locked; 
			_Container c;	// the underlying container
		};
	
	}
}

#endif /*__QUEUE_UTIL_H__*/


//
//// queue TEMPLATE FUNCTIONS
//template<class _Ty,
//class _Container> inline
//	bool operator==(const queue<_Ty, _Container>& _Left,
//	const queue<_Ty, _Container>& _Right)
//{	// test for queue equality
//	return (_Left._Get_container() == _Right._Get_container());
//}
//
//template<class _Ty,
//class _Container> inline
//	bool operator!=(const queue<_Ty, _Container>& _Left,
//	const queue<_Ty, _Container>& _Right)
//{	// test for queue inequality
//	return (!(_Left == _Right));
//}
//
//template<class _Ty,
//class _Container> inline
//	bool operator<(const queue<_Ty, _Container>& _Left,
//	const queue<_Ty, _Container>& _Right)
//{	// test if _Left < _Right for queues
//	return (_Left._Get_container() < _Right._Get_container());
//}
//
//template<class _Ty,
//class _Container> inline
//	bool operator>(const queue<_Ty, _Container>& _Left,
//	const queue<_Ty, _Container>& _Right)
//{	// test if _Left > _Right for queues
//	return (_Right < _Left);
//}
//
//template<class _Ty,
//class _Container> inline
//	bool operator<=(const queue<_Ty, _Container>& _Left,
//	const queue<_Ty, _Container>& _Right)
//{	// test if _Left <= _Right for queues
//	return (!(_Right < _Left));
//}
//
//template<class _Ty,
//class _Container> inline
//	bool operator>=(const queue<_Ty, _Container>& _Left,
//	const queue<_Ty, _Container>& _Right)
//{	// test if _Left >= _Right for queues
//	return (!(_Left < _Right));
//}