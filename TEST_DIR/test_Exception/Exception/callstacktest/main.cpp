#include <iostream>

using namespace std;

#include "debug_tool.hpp"

#include <windows.h>
#include <DbgHelp.h>

#pragma comment( lib, "Dbghelp.lib" )

inline std::string  GetLastErrorDescA( unsigned uErrorCode = ::GetLastError() )
{
	LPVOID lpMsg = NULL;
	::FormatMessageA(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM,
		NULL, uErrorCode, MAKELANGID(LANG_NEUTRAL,SUBLANG_DEFAULT),
		(LPSTR)&lpMsg,
		0,
		NULL
		);

	if( lpMsg == NULL )
	{
		return "δ֪";
	}

	std::string strReason = (const char*)lpMsg;

	::LocalFree( lpMsg );

	return strReason;
}

int SEHHanlerTest( LPEXCEPTION_POINTERS pep )
{
	dbsoft::callstack::callstack_ptr pCall = dbsoft::callstack::generate( pep->ContextRecord );

	cout<<" Error:: " << GetLastErrorDescA( pep->ExceptionRecord->ExceptionCode&0x0fffffff ) << endl;

	if( pCall )
	{
		for( dbsoft::callstack::const_iterator it = pCall->begin();
			it != pCall->end();
			++it )
		{
			cout<< *it << endl;
		}
	}

	return 1;
}

void test_main()
{
	int* p = NULL;

	__try
	{
		*p = 100;
	}
	__except( SEHHanlerTest( GetExceptionInformation() ) )
	{

	}
}

int main()
{
	dbsoft::callstack::callstack_ptr call = dbsoft::callstack::generate();

	if( call )
	{
		for( dbsoft::callstack::const_iterator it = call->begin();
			it != call->end();
			++it )
		{
			cout<< *it << endl;
		}
	}

	printf_s( "........................................................\n" );

	test_main();

	return 0;
}