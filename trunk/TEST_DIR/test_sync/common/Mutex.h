//file Mutex.h
/***************************************************************************************************
* 1�� File       �� Mutex.h
* 2�� Version    �� *.*
* 3�� Description�� ��װWin32�������ں˶�������ͬ��
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

namespace RG
{
	namespace SYNC
	{
		class CMTX
		{
		public:
			//���캯�� lpName������  bInitialOwner trueΪ�����߳�ӵ��  pSecurity��ȫ����
			CMTX(LPCTSTR lpName = NULL, bool bInitialOwner = false, LPSECURITY_ATTRIBUTES pSecurity = NULL)	
			{
				//�ҳ���ǰϵͳ�Ƿ��Ѿ�����ָ�����̵�ʵ�������û���򴴽�һ�������塣CreateMutex������������������һ�������������Ļ���������
				//ԭ�� HANDLE CreateMutex(LPSECURITY_ATTRIBUTES lpMutexAttributes, BOOL bInitialOwner, LPCTSTR lpName);
				//����
				//LPSECURITY_ATTRIBUTES lpMutexAttributes, //ָ��ȫ���Ե�ָ��
				//BOOL                  bInitialOwner,     //��ʼ���������������� FALSE��ʾMutex�������κ��߳� һ��Mutex�ڲ������κ��̵߳�ʱ���Ǵ��ڼ���̬�ģ� ���Դ������ź�״̬
				//LPCTSTR               lpName             //ָ�򻥳��������ָ��
				//����ֵ	Long����ִ�гɹ����ͷ��ػ��������ľ�������ʾ����������GetLastError��
				//��ʹ���ص���һ����Ч�����������ָ���������Ѿ����ڣ�GetLastErrorҲ����ΪERROR_ALREADY_EXISTS 
				__try
				{
					m_hMutex = ::CreateMutex(pSecurity, bInitialOwner, lpName);
				}
				__except (EXCEPTION_EXECUTE_HANDLER)
				{
					//EXCEPTION_EXECUTE_HANDLER      �ҵ��쳣����ģ��
					//EXCEPTION_CONTINUE_SEARCH      ����һ���try except�������� 
					//EXCEPTION_CONTINUE_EXECUTION   ��������쳣
					int iError = ::GetLastError();
					//������Ӧ�ô�ӡ�� ������				
					//ASSERT(IsValid());       //����
				}		
			}

			~CMTX(){void Close();}

			//��һ�����е���������������� lpName������ dwAccess��ȫ���ʷ������� bInheritHandle �Ƿ�Ϊ�ӽ��̼̳� 
			bool Open(LPCTSTR lpName, DWORD dwAccess = MUTEX_ALL_ACCESS, bool bInheritHandle = false)
			{
				Close();

				__try
				{
					//�������ܣ�Ϊ���е�һ����������������󴴽�һ���¾��
					//����ԭ�ͣ� HANDLE OpenMutex(
					//				DWORDdwDesiredAccess, // access
					//				BOOLbInheritHandle, // inheritance option
					//				LPCTSTRlpName // object name
					//				);
					//������	dwDesiredAccess��
					//			MUTEX_ALL_ACCESS ����Ի��������ȫ����
					//			MUTEX_MODIFY_STATE ����ʹ�� ReleaseMutex ����
					//			SYNCHRONIZE �����������ͬ��ʹ��
					//bInheritHandle : ��ϣ���ӽ����ܹ��̳о������ΪTRUE
					//lpName ��Ҫ�򿪶��������
					//����ֵ����ִ�гɹ������ض���ľ�������ʾʧ�ܡ������ø��������Ϣ�������GetLastError������
					//��ע��һ��������Ҫ��ע��һ��Ҫ�� CloseHandle �رջ������������������о�����ѹرգ���ô����Ҳ��ɾ��		
					m_hMutex = ::OpenMutex(dwAccess, bInheritHandle, lpName);
				}
				__except(EXCEPTION_EXECUTE_HANDLER) 
				{
					//EXCEPTION_EXECUTE_HANDLER      �ҵ��쳣����ģ��
					//EXCEPTION_CONTINUE_SEARCH      ����һ���try except�������� 
					//EXCEPTION_CONTINUE_EXECUTION   ��������쳣
					int iError = ::GetLastError();
					//������Ӧ�ô�ӡ�� ������	
					//ASSERT(IsValid());
				}	
				return(IsValid());
			}

			//���̻߳���˻����������Ի����������źű�Ϊû�ź�
			void Lock(DWORD dwMilliseconds = INFINITE)	{::WaitForSingleObject(m_hMutex, dwMilliseconds);}
			//�ѻ�������Ϊ���ź�
			void Unlock()								{::ReleaseMutex(m_hMutex);}

			HANDLE& GetHandle	() 	{return m_hMutex;}
			operator HANDLE		()	{return m_hMutex;}
			HANDLE* operator &	()	{return &m_hMutex;}
			bool IsValid		()	{return m_hMutex != NULL;}

		private:
			CMTX(const CMTX& mtx);
			CMTX operator = (const CMTX& mtx);
			void Close()
			{
				if(!IsValid()) return;
				VERIFY(::CloseHandle(m_hMutex));
				m_hMutex = NULL;
			}

		private:
			HANDLE m_hMutex;
		};
	}
}
#endif /*__MUTEX_UTIL_H__*/

