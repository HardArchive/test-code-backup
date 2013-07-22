//---------------------------------------------------------------------------
//
//	"CriticalSection.cpp"
//
//  Hides CRITICAL_SECTION
//
//	by Holger Kloos
//
//---------------------------------------------------------------------------

#include "stdafx.h"
#include "CriticalSection.h"




CCriticalSection::CCriticalSection()
{
	InitializeCriticalSection(&m_CritSect);
}

CCriticalSection::~CCriticalSection()
{
	DeleteCriticalSection(&m_CritSect);
}

void CCriticalSection::Lock()
{
	EnterCriticalSection(&m_CritSect);
}

void CCriticalSection::Unlock()
{
	LeaveCriticalSection(&m_CritSect);
}



CAutoCriticalSection::CAutoCriticalSection(CCriticalSection& rCritSect) 
					: m_rCritSect(rCritSect)	
{	
	m_rCritSect.Lock();		
}

CAutoCriticalSection::~CAutoCriticalSection()	
{	
	m_rCritSect.Unlock();	
}
