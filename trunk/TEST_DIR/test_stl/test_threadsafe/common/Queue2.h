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

		class queue
		{	// FIFO queue implemented with a container
		public:
			TYPE &back();  // �������һ��Ԫ�� 
			bool empty();  // ������п��򷵻��� 
			TYPE &front(); // ���ص�һ��Ԫ�� 
			void pop();   // ɾ����һ��Ԫ�� 
			void push( const TYPE &val );// ��ĩβ����һ��Ԫ�� 
			size_type size(); // ���ض�����Ԫ�صĸ��� 

	
		};
	
	}
}

#endif /*__QUEUE_UTIL_H__*/
