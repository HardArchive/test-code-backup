//IMyCreate.cpp
#include "StdAfx.h"
#include "IMyCreate.h"
 IMy * CreateMy()
{
  IMy* pResult=new CMy;
  if(pResult)
	  pResult->AddRef ();
  return pResult;
}