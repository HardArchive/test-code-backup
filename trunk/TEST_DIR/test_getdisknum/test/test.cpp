// test.cpp : �������̨Ӧ�ó������ڵ㡣
//

#include "stdafx.h"

//1:��Ҫ������
#include <pdh.h>
#pragma comment ( lib , "Pdh.lib " )
//2:�������
PDH_STATUS pdhStatus;
LPTSTR lpsCounterListBuffer = NULL;
LPTSTR lpsthisCounter = NULL ;
DWORD   dwCounterListSize = 0;
LPTSTR lpsInstanceListBuffer = NULL ;
LPTSTR lpsthisInstance = NULL;
DWORD   dwInstanceListSize= 0;
int nCount = 0 ; // Ӳ�̸����������ƶ�Ӳ�̣�

int _tmain(int argc, _TCHAR* argv[])
{
	//3:ʵ�ֲ���
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


// ��Ŀǰ��Ӳ�̸���Ϊ�� nCount ;