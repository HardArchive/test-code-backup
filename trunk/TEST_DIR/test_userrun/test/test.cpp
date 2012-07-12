// test.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include "wssrun.h"

//int _tmain(int argc, _TCHAR* argv[])
//{
//	return 0;
//}
//

void main( int argc, char** argv )
{
	//if ( argc < 2 )
	//{
	//  printf( "Usage: wssrun \n" );
	//  return ;
	//}

	//if ( CreateSystemProcess( argv[1] ) == FALSE )
	//{
	//  printf( "wssrun: CreateSystemProcess() to failed!\n" );
	//  return ;
	//}
	if ( CreateProcessEx(_T("1234.exe"), TRUE) == FALSE )
	{
		printf( "wssrun: CreateSystemProcess() to failed!\n" );
		return ;
	}

}


