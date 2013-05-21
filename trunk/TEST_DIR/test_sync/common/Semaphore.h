//file Semaphore.h
/***************************************************************************************************
* 1�� File       �� Semaphore.h
* 2�� Version    �� 1.0
* 3�� Description�� �ź���������
* 4�� Author     �� RG (http://www.9cpp.com/)
* 5�� Created    �� 2013-5-21 14:30:23
* 6�� History    �� 
* 7�� Remark     �� 	�ź��������̵߳�ͬ��
*					ͬ������ָ�ڻ���Ļ����ϣ�������������ͨ����������ʵ�ַ����߶���Դ��������ʡ�
*						�ڴ��������£�ͬ���Ѿ�ʵ���˻��⣬�ر�������д����Դ������ض��ǻ���ġ����������ָ����������������ͬʱ������Դ
*						�ź�������ʵ�ֶ��ͬ����Դ�Ķ��̻߳����ͬ�������ź���Ϊ��ֵ�ź���ʱ��Ҳ�������һ����Դ�Ļ�����ʡ�
*					�ź������������£��ź�����һ���Ǹ���������λ����������ͨ�������߳�/���̣����������Ὣ��������һ��ͨ������Ȼ��Ϊ��ʹ����Դ����
*						��������ֵΪ��ʱ��������ͼͨ�������̶߳������ڵȴ�״̬�����ź��������Ƕ������ֲ����� Wait���ȴ��� �� Release���ͷţ���
*						��һ���̵߳���Wait����ʱ����Ҫô�õ���ԴȻ���ź�����һ��Ҫôһֱ����ȥ��ָ�����������У���ֱ���ź������ڵ���һʱ��
*						Release���ͷţ�ʵ���������ź�����ִ�мӲ�������Ӧ�ڳ����뿪ͣ�������ò���֮���Խ������ͷš�����Ϊ�ͷ������ź����ػ�����Դ��
*					���ź���ֻ��ʵʩ���ֲ�����
*						1. ��ʼ����initialize����Ҳ����������create��
*						2. ���źţ�wait����Ҳ�ɽ�������suspend��
*						3. ���źţ�signal�����źţ�post��
*					CSEMʹ�ù��캯����ʼ��  Wait�����ȴ�-1  Release�����ͷ�(���ź�)+1 ʹ�ø�CMTX����
****************************************************************************************************/
#ifndef __SEMAPHORE_UTIL_H__
#define __SEMAPHORE_UTIL_H__

#ifndef __GENERAL_HELPER_H__
#include "GeneralHelper.h"
#endif

namespace RG
{
	class CSEM
	{
	public:
		//���캯�� lMaximumCount�ź����������� lInitialCount�����ź����ĳ�ʼ���� lpName������  bInitialOwner trueΪ�����߳�ӵ��  pSecurity��ȫ����
		CSEM(LONG lMaximumCount, LONG lInitialCount = 0, LPCTSTR lpName = NULL, LPSECURITY_ATTRIBUTES pSecurity = NULL)
		{
			//���� ����һ���µ��ź���
			//����ֵ	Long���ɹ� �����ź������������� ����,������GetLastError����ʹ����һ����Ч�����������ָ��ͬ����һ���ź����Ѵ��ڣ���GetLastErrorҲ�᷵��ERROR_ALREADY_EXISTS
			//����
			//lpSemaphoreAttributes SECURITY_ATTRIBUTES,�����ź����İ�ȫ����;ָ��һ��SECURITY_ATTRIBUTES�ṹ,�򴫵���ֵ������������ΪByVal As Long����������ֵ����ʾ���ò�����̳е�Ĭ����������
			//lInitialCount Long,                       �����ź����ĳ�ʼ�������������㵽lMaximumCount֮���һ��ֵ
			//lMaximumCount Long,                       �����ź�����������
			//lpName String,                            ָ���ź�����������ơ���vbNullString�ɴ���һ��δ�������ź�����������Ѿ�����ӵ��������ֵ�һ���ź�������ֱ�Ӵ��ֳɵ��ź�����������ֿ��ܲ���һ�����еĻ����塢�¼����ɵȴ���ʱ�����ļ�ӳ����������
			//ע��
			//һ��������Ҫ��һ����ס��CloseHandle�ر��ź����ľ�����������о�����ر��Ժ󣬶����Լ�Ҳ��ɾ��
			//һ��ֵ�����㣬�ź����ͻᴥ���������źţ���ReleaseSemaphore�����������������ź����ļ���������ɹ����͵����ź����ϵ�һ���ȴ��������������ļ���			
			__try
			{
				m_hsem = ::CreateSemaphore(pSecurity, lInitialCount, lMaximumCount, lpName);
			}
			__except(EXCEPTION_EXECUTE_HANDLER) 
			{
				int iError = ::GetLastError();
				//������Ӧ�ô�ӡ�� ������	
				//ASSERT(IsValid());
			}				
			ASSERT(IsValid());
		}

		~CSEM(){Close();}

		//������������(�߳�)�д������ź���  lpName������ dwAccess��ȫ���ʷ������� bInheritHandle �Ƿ�Ϊ�ӽ��̼̳� 
		bool Open(LPCTSTR lpName, DWORD dwAccess = SEMAPHORE_ALL_ACCESS, bool bInheritHandle = false)
		{
			Close();
			__try
			{
				//˵�� Ϊ���е�һ���������źŻ����󴴽�һ���¾����
				//����ֵ Long����ִ�гɹ������ض����������ʾʧ�ܡ�������GetLastError��
				//����
				//dwDesiredAccess Long����������֮һ��
				//		SEMAPHORE_ALL_ACCESS Ҫ����¼��������ȫ���ʣ�
				//		SEMAPHORE_MODIFY_STATE ����ʹ��ReleaseSemaphore������
				//		SYNCHRONIZE ����ͬ��ʹ���źŻ�����
				//bInheritHandle Long����������ӽ��̼̳о��������ΪTRUE��
				//lpName String��ָ��Ҫ�򿪵Ķ�������֡�
				//ע�� һ��������Ҫ��ע��һ��Ҫ��CloseHandle�ر��źŻ���������������о�����ѹرգ���ô����Ҳ��ɾ����
				m_hsem = ::OpenSemaphore(dwAccess, bInheritHandle, lpName); 
			}
			__except(EXCEPTION_EXECUTE_HANDLER) 
			{
				int iError = ::GetLastError();
				//������Ӧ�ô�ӡ�� ������	
				//ASSERT(IsValid());
			}		
			
			return(IsValid());
		}

		//�ȴ��ź���>0 ���ź�������������ʱ ����Ҫ�ȴ� ����WaitForSingleObject���ź���������1
		void Wait(DWORD dwMilliseconds = INFINITE)
		{
			::WaitForSingleObject(m_hsem, dwMilliseconds);
		}

		//�ź���++
		bool Release(LONG lReleaseCount = 1, LPLONG lpPreviousCount = NULL)
		{
			//�����ź����ĵ�ǰ��Դ����
			return ::ReleaseSemaphore(m_hsem, lReleaseCount, lpPreviousCount);
		}

		HANDLE& GetHandle	() 	{return m_hsem;}
		operator HANDLE		()	{return m_hsem;}
		HANDLE* operator &	()	{return &m_hsem;}
		bool IsValid		()	{return m_hsem != NULL;}

	private:
		CSEM(const CSEM& sem);
		CSEM operator = (const CSEM& sem);
		void Close()
		{
			if(!IsValid()) return;
			VERIFY(::CloseHandle(m_hsem));
			m_hsem = NULL;
		}
	private:
		HANDLE m_hsem;
	};
}


#endif /*__SEMAPHORE_UTIL_H__*/