// test.cpp : �������̨Ӧ�ó������ڵ㡣
//

#include "stdafx.h"
#include <Windows.h>
#include "DebugTrace.h"
using namespace TRACE_9CPP;

int _tmain(int argc, _TCHAR* argv[])
{
	TRACE(TEXT("TRACE���У�����ʼ����������-9CPP"));
	//Sleep(5000);
	TRACE(TEXT("TRACE����,�����������������-9CPP"));

	TRACEA("TRACEA����,�����������������-9CPP");
	TRACEW(L"TRACEW����,�����������������-9CPP");
	return 0;
}

