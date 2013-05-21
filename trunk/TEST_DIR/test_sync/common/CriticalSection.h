//file CriticalSection.h
/***************************************************************************************************
* 1�� File       �� CriticalSection.h
* 2�� Version    �� 1.0
* 3�� Description�� ��װWin32�ٽ�������,�����̼߳�ͬ��
* 4�� Author     �� RG (http://www.9cpp.com/)
* 5�� Created    �� 2013-5-21 13:57:02
* 6�� History    �� 
* 7�� Remark     �� �ٽ�����װ��1 ��ջ�ϴ������ٽ����������ֻ�ʺ����Լ������ٽ������� �ⲿ�����Ķ�����ʹ��
* 					�ٽ�����װ��2  �ڶ���new���ٽ������� ʹ���ٽ�������ָ��  ���Ե����ⲿ�������ٽ������� 
*					��Ƶ��������ɾ���Ķ����������1 ��Ϊ2���ܻ�����ڴ���Ƭ
****************************************************************************************************/
#ifndef __CRITICALSECTION_UTIL_H__
#define __CRITICALSECTION_UTIL_H__

#ifndef __GENERAL_HELPER_H__
#include "GeneralHelper.h"
#endif

namespace RG
{
	//�ٽ�����װ��1 ��ջ�ϴ������ٽ����������ֻ�ʺ����Լ������ٽ������� �ⲿ�����Ķ�����ʹ��
	class CCriSec
	{
	public:
		CCriSec()		{::InitializeCriticalSection(&m_stuCriSec);}
		~CCriSec()		{::DeleteCriticalSection(&m_stuCriSec);}

		void Lock()		{::EnterCriticalSection(&m_stuCriSec);}
		void Unlock()	{::LeaveCriticalSection(&m_stuCriSec);}

	private:
		CCriSec(const CCriSec& cs);
		CCriSec operator = (const CCriSec& cs);

	private:
		CRITICAL_SECTION    m_stuCriSec;
	};

	//�ٽ�����װ��2  �ڶ���new���ٽ������� ʹ���ٽ�������ָ��  ���Ե����ⲿ�������ٽ������� 
	//��Ƶ��������ɾ���Ķ����������1 ��Ϊ2���ܻ�����ڴ���Ƭ
	class CCriSec2
	{
	public:
		CCriSec2(bool bInitialize = TRUE)
		{
			if(bInitialize)
			{
				m_pstuCriSec = new CRITICAL_SECTION;
				::InitializeCriticalSection(m_pstuCriSec);
			}
			else
				m_pstuCriSec = NULL;
		}

		~CCriSec2() {Reset();}

		void Attach(CRITICAL_SECTION* pstuCriSec)
		{
			Reset();
			m_pstuCriSec = pstuCriSec;
		}

		CRITICAL_SECTION* Detach()
		{
			CRITICAL_SECTION* pstuCriSec = m_pstuCriSec;
			m_pstuCriSec = NULL;
			return pstuCriSec;
		}

		void Lock()		{::EnterCriticalSection(m_pstuCriSec);}
		void Unlock()	{::LeaveCriticalSection(m_pstuCriSec);}

	private:
		CCriSec2(const CCriSec2& cs);
		CCriSec2 operator = (const CCriSec2& cs);

		void Reset()
		{
			if(m_pstuCriSec)
			{
				::DeleteCriticalSection(m_pstuCriSec);
				delete m_pstuCriSec;
				m_pstuCriSec = NULL;
			}
		}

	private:
		CRITICAL_SECTION*    m_pstuCriSec;
	};
}
#endif /*__CRITICALSECTION_UTIL_H__*/

