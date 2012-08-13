// test.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"

#include <winsock2.h>
#include <ws2tcpip.h>
#include <stdio.h>
#pragma comment (lib, "wsock32.lib")
//#pragma comment(lib, "Ws2_32.lib")

int main2(int argc, char **argv)
{

	//-----------------------------------------
	// Declare and initialize variables
	
	char szDomain[64] = {"baidu.com"};

	WSADATA wsaData;
	int iResult;

	DWORD dwError;

	struct hostent *remoteHost;
	char *host_name;
	struct in_addr addr;

	// Initialize Winsock
	iResult = WSAStartup(MAKEWORD(2, 2), &wsaData);
	if (iResult != 0) 
	{
		printf("WSAStartup failed: %d\n", iResult);
		return 1;
	}

	host_name = szDomain;

	// If the user input is an alpha name for the host, use gethostbyname()
	// If not, get host by addr (assume IPv4)
	if (isalpha(host_name[0])) 
	{        /* host address is a name */
		printf("Calling gethostbyname with %s\n", host_name);
		remoteHost = gethostbyname(host_name);
	}
	else 
	{
		printf("Calling gethostbyaddr with %s\n", host_name);
		addr.s_addr = inet_addr(host_name);
		if (addr.s_addr == INADDR_NONE)
		{
			printf("The IPv4 address entered must be a legal address\n");
			return 1;
		} 
		else
			remoteHost = gethostbyaddr((char *) &addr, 4, AF_INET);
	}

	if (remoteHost == NULL) 
	{
		dwError = WSAGetLastError();
		if (dwError != 0) 
		{
			if (dwError == WSAHOST_NOT_FOUND) 
			{
				printf("Host not found\n");
				return 1;
			} 
			else if (dwError == WSANO_DATA) 
			{
				printf("No data record found\n");
				return 1;
			} 
			else 
			{
				printf("Function failed with error: %ld\n", dwError);
				return 1;
			}
		}
	} 
	else 
	{
		printf("Function returned:\n");
		printf("\tOfficial name: %s\n", remoteHost->h_name);
		printf("\tAlternate names: %s\n", remoteHost->h_aliases);
		printf("\tAddress type: ");
		switch (remoteHost->h_addrtype)
		{
		case AF_INET:
			printf("AF_INET\n");
			break;
		case AF_INET6:
			printf("AF_INET\n");
			break;
		case AF_NETBIOS:
			printf("AF_NETBIOS\n");
			break;
		default:
			printf(" %d\n", remoteHost->h_addrtype);
			break;
		}
		printf("\tAddress length: %d\n", remoteHost->h_length);
		addr.s_addr = *(u_long *) remoteHost->h_addr_list[0];
		printf("\tFirst IP Address: %s\n", inet_ntoa(addr));
	}

	getchar();
	return 0;
}

