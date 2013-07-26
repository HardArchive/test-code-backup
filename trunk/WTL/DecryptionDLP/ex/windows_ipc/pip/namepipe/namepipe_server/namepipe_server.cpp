// namepipe_server.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <windows.h>

#define BUFSIZE 3500
#define PIPE_TIMEOUT 20

VOID InstanceThread(LPVOID);
VOID GetAnswerToRequest(LPTSTR,LPTSTR,LPDWORD);

void LogPrintfHex(char *buf, unsigned short len);

 
int xx = 0;
void MyErrExit( char * errStr )
{
	printf("%s \n",errStr);
	exit(-1);
}

int main(int argc, char* argv[])
{
	BOOL fConnected;
	DWORD dwThreadId;
	HANDLE hPipe,hThread;

	LPTSTR lpszPipename = "\\\\.\\pipe\\mynamedpipe";
	//LPTSTR lpszPipename = "\\\\.\\pipe\\scs";

	for( ; ; )
	{
		hPipe = CreateNamedPipe(lpszPipename,PIPE_ACCESS_DUPLEX,PIPE_TYPE_MESSAGE|PIPE_READMODE_MESSAGE|PIPE_WAIT,
			PIPE_UNLIMITED_INSTANCES,
			BUFSIZE,BUFSIZE,PIPE_TIMEOUT,NULL);
		if( hPipe == INVALID_HANDLE_VALUE )
			MyErrExit("CreatePipe");

		fConnected = ConnectNamedPipe(hPipe,NULL) ?TRUE:( GetLastError() == ERROR_PIPE_CONNECTED);
		if( fConnected )
		{
			printf("Client pipe connected. \n");
			hThread = CreateThread(NULL,0,(LPTHREAD_START_ROUTINE)InstanceThread,hPipe,0,&dwThreadId);
			if( hThread == INVALID_HANDLE_VALUE)
				MyErrExit("CreateThread");
		}
		else
		{
			printf("Client pipe error!\n");
			CloseHandle(hPipe);
		}


	}


	return 0;
}

VOID InstanceThread( LPVOID lpvParam)
{
	char chRequest[BUFSIZE];
	char chReply[BUFSIZE];

	DWORD cbBytesRead,cbReplyBytes,cbWritten;
	BOOL fSuccess;
	HANDLE hPipe;

	hPipe = (HANDLE)lpvParam;
	while(1)
	{
		fSuccess = ReadFile(hPipe,chRequest,BUFSIZE,&cbBytesRead,NULL);
		if( !fSuccess || cbBytesRead == 0 )
			break;


		//GetAnswerToRequest(chRequest,chReply,&cbReplyBytes);
/*		strcpy(chReply,"Hello World");
		cbReplyBytes = strlen(chReply);
		fSuccess = WriteFile(hPipe,chReply,cbReplyBytes,&cbWritten,NULL);
		if( !fSuccess || cbReplyBytes != cbWritten )
			break;
*/
		if( cbBytesRead > 0 )
			LogPrintfHex(chRequest, cbBytesRead);

	}

	printf("Client have not msg.");
	FlushFileBuffers(hPipe);
	DisconnectNamedPipe(hPipe);
	CloseHandle(hPipe);

}


	void LogPrintfHex(char *buf, unsigned short len)
	{
		char printBuf[1024];
		memset(printBuf,0,sizeof(printBuf));

		if( len >= 256 )
			len = 256;

		int j = 0;
		printBuf[0] = '\n';
		j = 1;
		unsigned char t;
		for( int i = 0; i < len; i ++ )
		{
			t = buf[i];
			//sprintf(&printBuf[j],"%02x ",(unsigned char)t);
			printf("%02x ",(unsigned char)t);
			j = j + 3;
			if( i % 16 == 15 )
			{
				printf("\n");
				printBuf[j] = '\n';
				j ++;
			}
			
		}

		printf("\n");

		//LogPrintf(LOG_DEBUG,"%s",printBuf);


	}
