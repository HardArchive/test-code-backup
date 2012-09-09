//IMyCreate.cpp
#include "StdAfx.h"
#include "IMyCreate.h"
 IMy * CreateMy()
{
   return new CMy;
}