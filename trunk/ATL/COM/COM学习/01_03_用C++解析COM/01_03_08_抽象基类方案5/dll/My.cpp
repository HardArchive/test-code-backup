//My.cpp
#include "StdAfx.h"
#include ".\my.h"
#include <cstring>
using namespace std;

//��ָ�������ʼ��Ϊ0
CMy::CMy(void):m_iRes(0)
{
}

CMy::~CMy(void)
{
}

void CMy::Add (int x1,int x2)
{
   m_iResult=x1+x2;
}



void * CMy::QueryInterface(const char* pszType)
{
	void * pvResult=0;
  if(strcmp(pszType,"IMy")==0)
	  pvResult= static_cast<IMy*>(this);

  else if(strcmp(pszType,"IMy2")==0)
	  pvResult= static_cast<IMy2*>(this);

  else if(strcmp(pszType,"IMyUnkown")==0)
	  //����ע�⣬�����󹫹��ӿ�ʱ������IMy��IMy2���̳���IMyUnkown���������ж�����
	  pvResult= static_cast<IMy*>(this); 
  else
	//δ֧�ֵĽӿ�
	return 0;
  ((IMyUnkown *)pvResult)->AddRef ();
  return pvResult;
}

void CMy::AddRef ()
{
   //ָ�븴���ˣ����Ӽ���
	++m_iRes;
}
void CMy::Release()
{
	//ָ������ʱ������������
	if(--m_iRes==0)
       delete this;
}