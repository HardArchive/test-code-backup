//---------------------------------------------------------------------------
//
//	"TraceListener.cpp"
//
//  Console sample for multiple named pipes communication
//
//	by Holger Kloos
//
//---------------------------------------------------------------------------


#include "stdafx.h"
#include "conio.h"

#include "TraceCollector.h"

BOOL OnTrace(const char* pStr, DWORD nLength)
{
	_cputs(pStr);
	_cputs("\r\n");
	return TRUE;
}

int main(int argc, char* argv[])
{
	CTraceCollector tracer(OnTrace);
	tracer.Run();


	_getch();
	return 0;
}
