//My.cpp
#include "StdAfx.h"
#include ".\my.h"
#include <cstring>
using namespace std;

//将指针计数初始化为0
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
	  //这里注意，当请求公共接口时，由于IMy与IMy2都继承自IMyUnkown，这样就有二义性
	  pvResult= static_cast<IMy*>(this); 
  else
	//未支持的接口
	return 0;
  ((IMyUnkown *)pvResult)->AddRef ();
  return pvResult;
}

void CMy::AddRef ()
{
   //指针复制了，增加计数
	++m_iRes;
}
void CMy::Release()
{
	//指针销毁时，将对象销毁
	if(--m_iRes==0)
       delete this;
}