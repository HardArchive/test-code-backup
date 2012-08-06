//#include <windows.h>
//#include <afxinet.h>

#include "stdafx.h"
#include <Windows.h>
#include <iostream>
#include <WinInet.h>

#pragma comment(lib,"Wininet")
//int GetInternetFile(char *szFileUrl,char *szFileName);

int InternetGetFile(char *szFileUrl,char *szFileName);

int main(int argc,char **argv)
{

	DWORD dwTimeBegin = GetTickCount();
	InternetGetFile("http://blog.csdn.net/primer_programer/article/details/1968244", "D:\\WinPath\\desktop\\检查网络是否连接正常.html");
//	GetInternetFile(argv[1],argv[2]);
	printf("use time %u ms\n",GetTickCount()-dwTimeBegin);
	return 0;
}
//
//int GetInternetFile(char *szFileUrl,char *szFileName)
//{
//	CInternetSession mySessin("test");
//	mySessin.SetOption(INTERNET_OPTION_CONNECT_TIMEOUT,30);
//	CHttpFile *pHttpFile=NULL;
//	char szData[10*1024];
//	pHttpFile = (CHttpFile *)mySessin.OpenURL(szFileUrl);
//
//	DWORD dwBytesToRead=0;
//	DWORD dwBytesLeft=0;
//	DWORD dwBytesRead = 0;
//	DWORD dwBufLen=sizeof(DWORD);
//	if( !(pHttpFile->QueryInfo(HTTP_QUERY_CONTENT_LENGTH|HTTP_QUERY_FLAG_NUMBER,
//		&dwBytesToRead,&dwBufLen)) )
//	{
//		printf("query length failed:%d\n",GetLastError());
//		pHttpFile->Close();
//		mySessin.Close();
//		return -1;
//	}
//	printf("the internetfile length=%u bytes\n",dwBytesToRead);
//
//	FILE *fp=NULL;
//	fp = fopen(szFileName,"w");
//	if(!fp)
//	{
//		printf("create file %s failed\n",szFileName);
//		pHttpFile->Close();
//		mySessin.Close();
//		return -1;
//	}
//
//	dwBytesLeft = dwBytesToRead;
//	while( dwBytesLeft!=0 ){
//		dwBytesRead = pHttpFile->Read(szData,dwBytesLeft);
//		if( dwBytesRead==0 )
//			break;
//		else
//		{
//			fwrite(szData,1,dwBytesRead,fp);
//		}
//		dwBytesLeft = dwBytesToRead - dwBytesRead;
//	}
//	fclose(fp);
//	pHttpFile->Close();
//	mySessin.Close();
//
//	printf("download file success\n");
//	return 0;
//}

int InternetGetFile(char *szFileUrl,char *szFileName)
{
/*
	//获取Internet连接状态
	DWORD dwFlags=0;
	if( !InternetGetConnectedState(&dwFlags,0) )
	{
		printf("InternetGetConnectedState() failed:%d\n",GetLastError());
		return -1;
	}

	char strAgent[64]={0};
	sprintf(strAgent,"Agent%ld",GetTickCount());

	//打开Internet连接
	HINTERNET hOpen=NULL;
	if( !(dwFlags & INTERNET_CONNECTION_PROXY))
	{
		hOpen = InternetOpen(strAgent,INTERNET_OPEN_TYPE_PRECONFIG_WITH_NO_AUTOPROXY,
			NULL,NULL,0);
	}
	else
	{
		hOpen = InternetOpen(strAgent,INTERNET_OPEN_TYPE_PRECONFIG,NULL,NULL,0);
	}
*/

	HINTERNET hOpen=NULL;
	hOpen = InternetOpen("myAgent",INTERNET_OPEN_TYPE_PRECONFIG,NULL,NULL,0);
	if(!hOpen)
	{
		printf("Internet connect error:%d\n",GetLastError());
		return -1;
	}

	DWORD dwTimeOut = 30;
	if( !InternetSetOption(hOpen,INTERNET_OPTION_CONNECT_TIMEOUT,&dwTimeOut,sizeof(dwTimeOut)) )
	{
		printf("set timeout failed:%d\n",GetLastError());
		InternetCloseHandle(hOpen);
		return -1;
	}
/*
	DWORD dwValue=0;
	DWORD dwBufLen=sizeof(dwValue);
	if(!InternetQueryOption(hOpen,INTERNET_OPTION_CONNECT_TIMEOUT,&dwValue,&dwBufLen))
	{
		printf("InternetQueryOption failed:%d\n",GetLastError());
		InternetCloseHandle(hOpen);
		return -1;
	}
	printf("query option result, dwValue = %u,dwBufLen=%u\n",dwValue,dwBufLen);


*/
	char szHead[] = "Accept:*/*\r\n\r\n";
	char  szTemp[1024];
	HINTERNET hConnect=NULL;

	//打开Internet文件的URL
	if( !( hConnect = InternetOpenUrl(hOpen, szFileUrl, NULL, 0,
		INTERNET_FLAG_DONT_CACHE|INTERNET_FLAG_NO_AUTH|INTERNET_FLAG_NO_COOKIES
		,0) ) )
	{
		printf("Invalid url\n");
		InternetCloseHandle(hOpen);
		return -1;
	}

	FILE *fp=NULL;
	fp = fopen(szFileName,"w");
	if(!fp)
	{
		printf("create file %s failed\n",szFileName);
		InternetCloseHandle(hOpen);
		return -1;
	}

	DWORD dwBytesToRead =0;
	DWORD dwSizeOfRq = sizeof(dwBytesToRead);
	DWORD dwIndex=0;

	//查询Internet文件的长度信息
	if( !( HttpQueryInfo(hConnect,HTTP_QUERY_CONTENT_LENGTH|HTTP_QUERY_FLAG_NUMBER,
		(LPVOID)&dwBytesToRead,&dwSizeOfRq,&dwIndex) ) )
	{
		printf("HttpQueryInfo failed:%d\n",GetLastError());
		dwBytesToRead = 0;
		fclose(fp);
		InternetCloseHandle(hOpen);
		return -1;
	}
	printf("Avaliable data:%u bytes\n",dwBytesToRead);
	
	DWORD dwReadBytes=0;
	do
	{
		//读取文件数据
		if( !InternetReadFile(hConnect,szTemp,1024,&dwReadBytes) )
		{
			printf("read file error:%d\n",GetLastError());
			fclose(fp);
			InternetCloseHandle(hOpen);
			return -1;
		}

		//下载完毕
		if(dwReadBytes==0)
			break;
		else
			fwrite(szTemp,dwReadBytes,1,fp);
		
		if( (dwBytesToRead -= dwReadBytes)==0 ) break;

	}while(	TRUE);
	
	fclose(fp);
	InternetCloseHandle(hOpen);
	printf("download file success\n");
	return 0;
	
}