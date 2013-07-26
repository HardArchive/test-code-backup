// namepipe_client.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <stdlib.h>
#include <windows.h>

void MyErrExit( char *chStr )
{
	printf("Error: %s \r\n",chStr);
	exit(-1);
}

int main(int argc, char* argv[])
{
	HANDLE hPipe;
	LPVOID lpvMessage;
	CHAR cbBuf[512];
	BOOL fSuccess;
	DWORD cbRead,cbWritten,dwMode;
	LPTSTR lpszPipeName = "\\\\.\\pipe\\mynamedpipe";


	while( 1 )
	{
		fSuccess = WaitNamedPipe(lpszPipeName,NULL );
		if( !fSuccess )
		{
			Sleep(1000);
			continue;
		}


		hPipe = CreateFile( lpszPipeName,GENERIC_READ|GENERIC_WRITE,0,NULL,OPEN_EXISTING,0,NULL);
		if( hPipe != INVALID_HANDLE_VALUE )
			break;
		if( GetLastError() != ERROR_PIPE_BUSY )
		{
			printf("Could not open pipe ");
		}

		//if( WaitNamedPipe(lpszPipeName,20000) )
		//	break;
		Sleep(1000);
		printf("Timeout Could not open pipe  \r\n" );

	}

	printf("Connection setup successfully ! \r\n");
	dwMode = PIPE_READMODE_MESSAGE;
	fSuccess = SetNamedPipeHandleState(hPipe,&dwMode,NULL,NULL);
	if( !fSuccess )
		MyErrExit("SetNamedPipeHandleState");
	lpvMessage = ( argc > 1 ) ? argv[1]: "default messageabce测试数据";
	fSuccess = WriteFile(hPipe,(void *)lpvMessage,strlen((char *)lpvMessage) + 1,&cbWritten,NULL);
	if( !fSuccess )
		MyErrExit("WriteFile\r\n");

	fSuccess = WriteFile(hPipe,(void *)lpvMessage,500,&cbWritten,NULL);
	if( !fSuccess )
		MyErrExit("WriteFile");

	memset(cbBuf, 0, 512);

	do
	{
		memset(cbBuf, 0, 512);
		fSuccess = ReadFile(hPipe,cbBuf,512,&cbRead,NULL);
		printf("打印接收数据:%s\r\n", cbBuf);
		if( !fSuccess && GetLastError() != ERROR_MORE_DATA)
			break;
		//if( !WriteFile(GetStdHandle(STD_OUTPUT_HANDLE),cbBuf,cbRead,&cbWritten,NULL) )
		//	break;
	}while( fSuccess );
	CloseHandle(hPipe);

	printf("Close pipe ! \r\n");

	return 0;


}

