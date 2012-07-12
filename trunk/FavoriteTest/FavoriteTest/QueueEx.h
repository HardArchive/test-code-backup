//#include "../stdafx.h"
#pragma once
#include <memory.h>

template<class T>      
class CQueueEx
{  
protected:   int head;     //��ͷָ��   
			 int tail;     //��βָ�� 
			 int qsize;    //���г���
			 T *elements;  //��Ŷ���Ԫ�ص�����   
			 int maxsize;  //������������Ԫ�ظ���   
			 HANDLE    m_hPushEvent;                       //���������¼�����
			 HANDLE    m_hPopEvent;                        //���Ƴ����¼�����
			 CRITICAL_SECTION m_lsGS;				       //�ٽ����
public:   CQueueEx(int i = 32);			//���캯��   
		  ~CQueueEx();					//��������   

		  void PushTail(T);				//����Ԫ�ز����ڶ�β   
		  bool PopFront(T&);			//�Ӷ�ͷȡһ��Ԫ��   

		  bool IsFull()const			//�ж����Ƿ�Ϊ��   
		  {
			  return (tail+1) % maxsize == head;
		  } 

		  bool IsEmpty()				//�ж����Ƿ�Ϊ��   
		  {
			  return head == tail;
		  }

		  bool append();				//���ռ���ʱ׷�ӷ���ռ�

		  int get_maxsize()
		  {
			  return maxsize-1;
		  }

		  int get_qsize()
		  {
			  return qsize;
		  }
};



//���캯��
template <class T>
CQueueEx<T>::CQueueEx(int i)
{
	m_hPushEvent = CreateEvent(NULL, TRUE, TRUE, NULL);
	m_hPopEvent = CreateEvent(NULL, TRUE, TRUE, NULL);
	InitializeCriticalSection(&m_lsGS);

	EnterCriticalSection(&m_lsGS);

	head = tail = 0;		//����������Ϊ�ն���  
	maxsize = i<10?10:i;		//���ö��е���󳤶ȣ���СΪ 10  
	qsize = 0;  
	elements = new T[maxsize];//�����з����ڴ�ռ� 
	if(elements == 0)			//ʧ�ܣ�����
	{
		OutputDebugString("CQueueEx�ڴ����ʧ��");
	}

	LeaveCriticalSection(&m_lsGS);
}

//��������
template <class T>
CQueueEx<T>::~CQueueEx()
{
	SetEvent(m_hPushEvent);//���������Ϊ���ź���
	SetEvent(m_hPopEvent); //���ó�����Ϊ���ź���
	EnterCriticalSection(&m_lsGS);
	delete []elements;
	DeleteCriticalSection(&m_lsGS);

} 

//����Ԫ�ز����ڶ�β
template <class T>
void CQueueEx<T>::PushTail(T element)
{
	if(IsFull())                //��������׷�ӿռ�
	{
		append();
	}
	//if (IsFull())
	//{
	//	ResetEvent(m_hPushEvent);//�������ź���
	//}

	WaitForSingleObject(m_hPushEvent, INFINITE);

	EnterCriticalSection(&m_lsGS);
	tail = (tail+1) % maxsize;  //βָ��� 1  
	elements[tail] = element;   //����β��ֵ
	qsize++;                    //�ӳ��� 1
	LeaveCriticalSection(&m_lsGS);
	SetEvent(m_hPopEvent);//���ó�����Ϊ���ź���
}

//�Ӷ�ͷȡһ��Ԫ��
template <class T>
bool CQueueEx<T>::PopFront(T &element)
{	
	if(IsEmpty())
	{
		//cout<<"����Ϊ��"<<endl;    //���ӿգ����棬������
		ResetEvent(m_hPopEvent);   //�Ѷ�����Ϊ���ź�״̬
	}
	//TRACE("ȡ�����������ݣ����еȴ�\n");
	WaitForSingleObject(m_hPopEvent, INFINITE);
	//TRACE("ȡ�����������ݣ��ȴ����\n");
	EnterCriticalSection(&m_lsGS);
	head = (head+1) % maxsize;    //ͷָ��� 1  
	element =  elements[head];    //ȡ����ͷԪ��  
	qsize--;                      //�ӳ��� 1  
	LeaveCriticalSection(&m_lsGS);
	SetEvent(m_hPushEvent);//���������Ϊ���ź���
	return true; 
}

//���ռ���ʱ׷�ӷ���ռ�
template <class T>
bool CQueueEx<T>::append()
{
	T *p,*temp;
	p = new T[maxsize+10];        //ÿ��׷��10���ڴ�ռ�
	if(!p)
	{
		//cout<<"�ڴ�׷��ʧ��"<<endl;
		return false;
	}
	EnterCriticalSection(&m_lsGS);
	memmove(p, elements, sizeof(T)*maxsize);
	temp = elements;
	elements = p;
	delete []temp;
	maxsize = maxsize + 10;
	LeaveCriticalSection(&m_lsGS);
	return true;
}