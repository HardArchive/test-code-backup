//file Queue.h
/***************************************************************************************************
* 1、 File       ： TSQueue.h
* 2、 Version    ： *.*
* 3、 Description： Thread Safe Queue
* 4、 Author     ： RG (http://www.9cpp.com/)
* 5、 Created    ： 2013-5-22 14:22:20
* 6、 History    ： 
* 7、 Remark     ： 
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
			// 加互斥锁。  
			// 这儿加的是自旋锁，在多处理器平台上，  
			// 这可以在一定程度上避免线程上下文切换，使性能更出色。  
			// 加锁过程在尝试QUEUE_LOCK_MAX_TRY_TIMES次失败后就换出线程。 
			inline void Lock()
			{
#ifndef RG_QUEUE_UNSAFE  
				DWORD dwTried = 0;  
				//能以原子操作的方式交换俩个参数a, b，并返回a以前的值
				//假设有线程１和线程２调用ｆ（）函数，线程１先调用到InterlockedExchange(&g, TRUE);线程２再调用时，函数InterlockedExchange()总返回ＴＲＵＥ，则线程２sleep(0);
				//而直到线程１调用InterlockedExchange(&g, FALSE);后线程２才可能由于调用InterlockedExchange(&g,, TRUE)，返回ＦＡＬＳＥ而退出循环接着工作。
				//这样在进行其他的操作这里就能操作共享数据而不会引起争议。当然这种方法会浪费ｃｐｕ时间，因为ｃｐｕ要不断地执行InterlockedExchange()函数，
				while(InterlockedExchange(&_Locked, true))  
				{  
					dwTried++;  
					if(QUEUE_LOCK_MAX_TRY_TIMES == dwTried)  
					{  
						//放弃CPU时间片
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

			// 解除互斥锁   
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