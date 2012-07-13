// test.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"

//1:必要的声明
#include <pdh.h>
#pragma comment ( lib , "Pdh.lib " )
//2:定义变量
PDH_STATUS pdhStatus;
LPTSTR lpsCounterListBuffer = NULL;
LPTSTR lpsthisCounter = NULL ;
DWORD   dwCounterListSize = 0;
LPTSTR lpsInstanceListBuffer = NULL ;
LPTSTR lpsthisInstance = NULL;
DWORD   dwInstanceListSize= 0;
int nCount = 0 ; // 硬盘个数（包含移动硬盘）

int _tmain(int argc, _TCHAR* argv[])
{
	//3:实现部分
	pdhStatus = PdhEnumObjectItems(
		NULL ,  
		NULL ,
		_TEXT ("PhysicalDisk ") ,
		lpsCounterListBuffer ,
		& dwCounterListSize ,
		lpsInstanceListBuffer ,
		& dwInstanceListSize ,
		PERF_DETAIL_WIZARD ,
		0 ) ;  

	lpsCounterListBuffer = (LPTSTR) malloc(dwCounterListSize * sizeof (TCHAR)) ;
	//ASSERT ( lpsCounterListBuffer != NULL ) ;

	lpsInstanceListBuffer= (LPTSTR) malloc(dwInstanceListSize * sizeof (TCHAR)) ;
	//ASSERT ( lpsInstanceListBuffer != NULL ) ;

	pdhStatus = PdhEnumObjectItems (
		NULL ,  
		NULL ,
		_TEXT ("PhysicalDisk ") ,
		lpsCounterListBuffer ,
		& dwCounterListSize ,
		lpsInstanceListBuffer ,
		& dwInstanceListSize ,
		PERF_DETAIL_WIZARD ,
		0 ) ;
	//ASSERT ( pdhStatus == ERROR_SUCCESS) ;

	lpsthisInstance = lpsInstanceListBuffer ;
	lpsthisCounter = lpsCounterListBuffer;

	for (;*lpsthisInstance> 0;lpsthisInstance += (lstrlen(lpsthisInstance)+1))
	{
		if (lstrcmp(lpsthisInstance,_TEXT("_Total ")) == 0) 
			continue ;
		nCount ++ ;
	}

	if ( lpsInstanceListBuffer != NULL )
	{
		free ( lpsInstanceListBuffer ) ;
		lpsInstanceListBuffer = NULL  ;
	}

	if ( lpsCounterListBuffer != NULL )
	{
		free ( lpsCounterListBuffer ) ;
		lpsCounterListBuffer = NULL  ;
	}

	dwCounterListSize = 0 ;
	dwInstanceListSize = 0 ;

	return 0;
}


// 你目前的硬盘个数为： nCount ;