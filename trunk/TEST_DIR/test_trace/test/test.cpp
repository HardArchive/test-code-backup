// test.cpp : �������̨Ӧ�ó������ڵ㡣
//

#include "stdafx.h"
#include <Windows.h>
#include "DebugTrace.h"
#include "MessageboxEx.h"
using namespace QNA;

int _tmain(int argc, _TCHAR* argv[])
{
	TRACE(TEXT("TRACE���У�����ʼ����������-9CPP"));
	//Sleep(5000);
	TRACE(TEXT("TRACE����,�����������������-9CPP"));

	TRACEA("TRACEA����,�����������������-9CPP");
	TRACEW(L"TRACEW����,�����������������-9CPP");
	MessageBoxEx(NULL, MB_OK, L"����", L"����%s", L"\r\n�������");
	MessageBoxEx(NULL, MB_YESNO, L"����", L"����%s", L"\r\n�������???");
	return 0;
}

