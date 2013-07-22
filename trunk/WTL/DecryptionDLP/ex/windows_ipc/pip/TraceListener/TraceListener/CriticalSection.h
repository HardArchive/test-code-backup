//---------------------------------------------------------------------------
//
//	"CriticalSection.h"
//
//  Hides CRITICAL_SECTION
//
//	by Holger Kloos
//
//---------------------------------------------------------------------------


#ifndef __CRITICALSECTION_H__
#define __CRITICALSECTION_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000



class CCriticalSection  
{
public:
	CCriticalSection();
	virtual ~CCriticalSection();

	void Lock();
	void Unlock();

private:
	CRITICAL_SECTION m_CritSect;
};



class CAutoCriticalSection
{
public:
	CAutoCriticalSection(CCriticalSection& rCritSect);
	virtual ~CAutoCriticalSection();

private:
	CCriticalSection& m_rCritSect;
};

#endif
