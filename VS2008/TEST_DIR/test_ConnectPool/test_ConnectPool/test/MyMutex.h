#ifndef __MY_MUTEX_H
#define __MY_MUTEX_H
/******************************************************************************* 
* 1�� �ļ����ƣ� MyMutex
* 2�� �桡������ Version *.*
* 3�� ��    ���� ��MFC����CCriticalSection������ʹ�òο�CCriticalSection��
				��CCriticalSection�Ķ����ʾһ�����ٽ�����������һ������ͬ���Ķ���ͬһʱ��ֻ����һ���̴߳�ȡ��Դ���������
				�ٽ����ڿ���һ��ֻ��һ���߳��޸����ݻ������Ŀ�����Դʱ�ǳ����á�
* 4�� ������ƣ� ��ë
* 5�� �������ڣ� 2012-2-20 14:09:47
* 6�� �� �� �ˣ� 
* 7�� �޸����ڣ� 
********************************************************************************/ 
#include <process.h>
#include <windows.h>

namespace QNA
{
	class CCriticalSection
	{
	public:
		CCriticalSection()
		{
			::InitializeCriticalSection(&fMutex);
		}

		~CCriticalSection()
		{
			::DeleteCriticalSection(&fMutex);
		}

		//��CCriticalSection�����ϼ���
		void Lock()
		{
			::EnterCriticalSection(&fMutex);
		}

		//�ͷ�CCriticalSection�����ϵ���
		void Unlock()
		{
			::LeaveCriticalSection(&fMutex);
		}

	private:
		CRITICAL_SECTION fMutex;
	};
}

#endif