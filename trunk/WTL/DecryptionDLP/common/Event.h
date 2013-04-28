/*
 * Copyright Bruce Liang (ldcsaa@gmail.com)
 *
 * Version	: 2.1.1
 * Author	: Bruce Liang
 * Porject	: https://code.google.com/p/ldcsaa
 * Bolg		: http://www.cnblogs.com/ldcsaa
 * WeiBo	: http://weibo.com/u/1402935851
 * QQ Group	: http://qun.qq.com/#jointhegroup/gid/75375912
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */
 
/******************************************************************************
Module:  Event.h
Notices: Copyright (c) 2006 Bruce Liang
Purpose: ��װWin32�¼��ں˶���, ���������ü���ȡ����ʱͬ���߳�.
Desc:
Usage:
******************************************************************************/

#pragma once
#include "GeneralHelper.h"
class CEvt
{
public:	
	//bManualReset  ָ�����¼����󴴽����ֶ���ԭ�����Զ���ԭ��
	//			    �����TRUE����ô������ResetEvent�������ֹ����¼���״̬��ԭ�����ź�״̬��
	//		        �������ΪFALSE�����¼���һ���ȴ��߳��ͷ��Ժ�ϵͳ�����Զ����¼�״̬��ԭΪ���ź�״̬��
	//bInitialState	ָ���¼�����ĳ�ʼ״̬�����ΪTRUE����ʼ״̬Ϊ���ź�״̬������Ϊ���ź�״̬
	//pszName       ָ���¼��Ķ�������ƣ���һ����0�������ַ���ָ��
	//pSecurity     ȷ�����صľ���Ƿ�ɱ��ӽ��̼̳С����ֵ��NULL���˾�����ܱ��̳С�
	CEvt(BOOL bManualReset = FALSE, BOOL bInitialState = FALSE, LPCTSTR pszName = nullptr, LPSECURITY_ATTRIBUTES pSecurity = nullptr)
	{
		m_hEvent = ::CreateEvent(pSecurity, bManualReset, bInitialState, pszName);
		ASSERT(IsValid());
	}

	~CEvt()
	{
		if(IsValid())
			VERIFY(::CloseHandle(m_hEvent));
	}

	//���´��¼�
	BOOL Open(DWORD dwAccess, BOOL bInheritHandle, LPCTSTR pszName)
	{
		if(IsValid())
			VERIFY(::CloseHandle(m_hEvent));

		m_hEvent = ::OpenEvent(dwAccess, bInheritHandle, pszName);
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
	BOOL Pulse()	{return(::PulseEvent(m_hEvent));}

	//��ָ�����¼���������Ϊ���ź�״̬��
	BOOL Reset()	{return(::ResetEvent(m_hEvent));}
	//��ָ�����¼���������Ϊ���ź�
	BOOL Set()		{return(::SetEvent(m_hEvent));}

	//�ȴ�  dwTimeout��ʱʱ��
	DWORD Wait(DWORD dwTimeout = INFINITE) {return ::WaitForSingleObject(m_hEvent, dwTimeout);}

	HANDLE& GetHandle	() 	{return m_hEvent;}
	operator HANDLE		()	{return m_hEvent;}
	HANDLE* operator &	()	{return &m_hEvent;}
	//����¼��Ƿ��Ѿ�����
	BOOL IsValid		()	{return m_hEvent != nullptr;}

private:
	CEvt(const CEvt&);
	CEvt operator = (const CEvt&);

private:
	HANDLE m_hEvent;
};

