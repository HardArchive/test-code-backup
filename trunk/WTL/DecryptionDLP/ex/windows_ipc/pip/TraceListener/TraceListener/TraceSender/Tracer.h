//---------------------------------------------------------------------------
//
//	"Tracer.h"
//
//  Puts trace data in a named pipe
//
//	by Holger Kloos
//
//---------------------------------------------------------------------------


#ifndef	__TRACER_H__
#define __TRACER_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CTracer  
{
public:
	CTracer();
	virtual ~CTracer();

	bool Connect();
	bool MakeTrace(const char* pStr,...) const;


private:
	HANDLE m_hPipe;

};

#endif
