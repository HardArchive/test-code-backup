// test.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include <Windows.h>
#include "DebugTrace.h"
#include "MessageboxEx.h"
using namespace QNA;

int _tmain(int argc, _TCHAR* argv[])
{
	TRACE(TEXT("TRACE运行，程序开始！！！！！-9CPP"));
	//Sleep(5000);
	TRACE(TEXT("TRACE运行,程序结束！！！！！-9CPP"));

	TRACEA("TRACEA运行,程序结束！！！！！-9CPP");
	TRACEW(L"TRACEW运行,程序结束！！！！！-9CPP");
	MessageBoxEx(NULL, MB_OK, L"标题", L"内容%s", L"\r\n添加内容");
	MessageBoxEx(NULL, MB_YESNO, L"标题", L"内容%s", L"\r\n添加内容???");
	return 0;
}

