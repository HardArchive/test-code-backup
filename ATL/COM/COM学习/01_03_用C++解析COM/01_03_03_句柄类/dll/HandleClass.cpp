#include "StdAfx.h"
#include ".\handleclass.h"

CHandleClass::CHandleClass(void):m_pThis(new CMy)
{
}

CHandleClass::~CHandleClass(void)
{
}

void CHandleClass::Add (int x1,int x2)
{
   m_pThis->Add(x1,x2);
}

int CHandleClass::Get ()
{
   return m_pThis->Get();
}
