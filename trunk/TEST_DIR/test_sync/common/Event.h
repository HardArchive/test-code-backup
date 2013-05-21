//file Event.h
/***************************************************************************************************
* 1�� File       �� Event.h
* 2�� Version    �� 1.0
* 3�� Description�� ��װWin32�¼��ں˶���, ���������ü���ȡ����ʱͬ���߳�.
* 4�� Author     �� RG (http://www.9cpp.com/)
* 5�� Created    �� 2013-5-21 13:40:08
* 6�� History    �� 
* 7�� Remark     �� 
****************************************************************************************************/
#ifndef __EVENT_UTIL_H__
#define __EVENT_UTIL_H__

#ifndef __GENERAL_HELPER_H__
#include "GeneralHelper.h"
#endif

namespace RG
{
	class CEvt
	{
	public:	
		//���캯��
		//lpName       ָ���¼��Ķ�������ƣ���һ����0�������ַ���ָ��
		//bManualReset  ָ�����¼����󴴽����ֶ���ԭ�����Զ���ԭ��
		//			    �����TRUE����ô������ResetEvent�������ֹ����¼���״̬��ԭ�����ź�״̬��
		//		        �������Ϊfalse�����¼���һ���ȴ��߳��ͷ��Ժ�ϵͳ�����Զ����¼�״̬��ԭΪ���ź�״̬��
		//bInitialState	ָ���¼�����ĳ�ʼ״̬�����ΪTRUE����ʼ״̬Ϊ���ź�״̬������Ϊ���ź�״̬		
		//pSecurity     ȷ�����صľ���Ƿ�ɱ��ӽ��̼̳С����ֵ��NULL���˾�����ܱ��̳С�
		CEvt(LPCTSTR lpName = NULL, bool bManualReset = false, bool bInitialState = false, LPSECURITY_ATTRIBUTES pSecurity = NULL)
		{			
			__try
			{
				m_hEvent = ::CreateEvent(pSecurity, bManualReset, bInitialState, lpName);
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

			ASSERT(IsValid());
		}

		~CEvt(){Close();}

		//���´��¼�
		bool Open(DWORD dwAccess, bool bInheritHandle, LPCTSTR lpName)
		{
			Close();
			__try
			{
				m_hEvent = ::OpenEvent(dwAccess, bInheritHandle, lpName);
			}
			__except(EXCEPTION_EXECUTE_HANDLER) 
			{
				int iError = ::GetLastError();
				//������Ӧ�ô�ӡ�� ������	
				//ASSERT(IsValid());
			}			
			return(IsValid());
		}

		//ϵͳ���Ķ����е�Event�¼������ڽ��̡��̼߳�ͬ����ʱ���ǱȽϳ��ã�����������������������һ����SetEvent������һ��PulseEvent�����ߵ������ǣ�
		//SetEventΪ�����¼�����Ϊ���ź�״̬����PulseEventҲ�ǽ�ָ�����¼���Ϊ���ź�״̬��
		//��ͬ���������һ���˹������¼������ڵȺ��¼��ġ�������������̶߳������״̬����������¼���أ������أ�
		//�����һ�� �Զ������¼��������ڵȺ��¼��ġ�������ĵ����̻߳����״̬���¼����������źţ����Һ������ء�
		//Ҳ����˵���Զ�����ģʽ��PulseEvent��SetEvent������û��ʲô���𣬵����ֶ�ģʽ��PulseEvent�������ԵĲ�ͬ�����ԱȽ����׵Ŀ��Ƴ����ǵ����ߣ����������ߡ�
		//�����ѭ����Ҫ��ִ��һ�ξ���PulseEvent���������ѭ��������ͣ����ת����SetEvent����Ҫ��ֹͣ�ĵط�����ResetEvent��OK�ˡ�
		// 
		//PulseEventʹ���¼���Ϊ��֪ͨ״̬��Ȼ����������Ϊδ֪ͨ״̬�����������SetEvent֮������������ResetEventһ����
		//�����ڵ���PulseEventʱ�޷�֪���κ��̵߳�״̬����˸ú���������ô���á�	
		bool Pulse()	{return(::PulseEvent(m_hEvent));}

		//��ָ�����¼���������Ϊ���ź�״̬��
		bool Reset()	{return(::ResetEvent(m_hEvent));}
		//��ָ�����¼���������Ϊ���ź�
		bool Set()		{return(::SetEvent(m_hEvent));}

		//�ȴ�  dwTimeout��ʱʱ��
		DWORD Wait(DWORD dwTimeout = INFINITE) {return ::WaitForSingleObject(m_hEvent, dwTimeout);}

		HANDLE& GetHandle	() 	{return m_hEvent;}
		operator HANDLE		()	{return m_hEvent;}
		HANDLE* operator &	()	{return &m_hEvent;}
		//����¼��Ƿ��Ѿ�����
		bool IsValid		()	{return m_hEvent != nullptr;}

	private:
		CEvt(const CEvt&);
		CEvt operator = (const CEvt&);
		void Close()
		{
			if(!IsValid()) return;
			VERIFY(::CloseHandle(m_hEvent));
			m_hEvent = NULL;
		}

	private:
		HANDLE m_hEvent;
	};
}

#endif /*__EVENT_UTIL_H__*/