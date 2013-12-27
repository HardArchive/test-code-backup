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

		class queue
		{	// FIFO queue implemented with a container
		public:
			TYPE &back();  // 返回最后一个元素 
			bool empty();  // 如果队列空则返回真 
			TYPE &front(); // 返回第一个元素 
			void pop();   // 删除第一个元素 
			void push( const TYPE &val );// 在末尾加入一个元素 
			size_type size(); // 返回队列中元素的个数 

	
		};
	
	}
}

#endif /*__QUEUE_UTIL_H__*/
