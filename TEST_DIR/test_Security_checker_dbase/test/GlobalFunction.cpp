#include "StdAfx.h"
#include "GlobalFunction.h"

#include <IPHlpApi.h>
#pragma comment(lib, "Iphlpapi.lib")

#include <Tlhelp32.h>
#include <Wtsapi32.h>
#pragma comment(lib, "Wtsapi32.lib")

BOOL GetNetInfo(LPTSTR ptszMacBuf, LPTSTR ptszIpBuf)
//BOOL GetNetInfo(TCHAR* ptszMacBuf, TCHAR* ptszIpBuf)
{
	//PIP_ADAPTER_INFO结构体指针存储本机网卡信息
	PIP_ADAPTER_INFO pIpAdapterInfo = new IP_ADAPTER_INFO();
	//得到结构体大小,用于GetAdaptersInfo参数
	unsigned long stSize = sizeof(IP_ADAPTER_INFO);
	//调用GetAdaptersInfo函数,填充pIpAdapterInfo指针变量;其中stSize参数既是一个输入量也是一个输出量
	int dwRet = GetAdaptersInfo(pIpAdapterInfo,&stSize);
	if (ERROR_BUFFER_OVERFLOW == dwRet)
	{
		//如果函数返回的是ERROR_BUFFER_OVERFLOW
		//则说明GetAdaptersInfo参数传递的内存空间不够,同时其传出stSize,表示需要的空间大小
		//这也是说明为什么stSize既是一个输入量也是一个输出量
		//释放原来的内存空间
		delete pIpAdapterInfo;
		//重新申请内存空间用来存储所有网卡信息
		pIpAdapterInfo = (PIP_ADAPTER_INFO)new BYTE[stSize];
		//再次调用GetAdaptersInfo函数,填充pIpAdapterInfo指针变量
		dwRet = GetAdaptersInfo(pIpAdapterInfo,&stSize);     
	}
	if (ERROR_SUCCESS == dwRet)
	{
		char szTemp[8] = {0};
		char szMac[32] = {0};
		char szIP[32] = {0};
		////可能有多网卡,因此通过循环去判断, 但这里只获取和一个网上MAC和IP
		//while (pIpAdapterInfo)
		//{
		//	for (UINT i = 0; i < pIpAdapterInfo->AddressLength; i++)
		//	{
		//		if (i==pIpAdapterInfo->AddressLength-1)
		//		{
		//			sprintf_s(szTemp, "%02x\n", pIpAdapterInfo->Address[i]);
		//			strcat_s(szMac, szTemp);
		//		}
		//		else
		//		{
		//			sprintf_s(szTemp, "%%02x-", pIpAdapterInfo->Address[i]);
		//			strcat_s(szMac, szTemp);
		//		}
		//	}

		//	//可能网卡有多IP,因此通过循环去判断
		//	IP_ADDR_STRING *pIpAddrString =&(pIpAdapterInfo->IpAddressList);
		//	do 
		//	{
		//		strcpy_s(szIP, pIpAddrString->IpAddress.String);
		//		pIpAddrString=pIpAddrString->Next;
		//	} while (pIpAddrString);
		//	pIpAdapterInfo = pIpAdapterInfo->Next;
		//}

		for (UINT i = 0; i < pIpAdapterInfo->AddressLength; i++)
		{
			if (i==pIpAdapterInfo->AddressLength-1)
			{
				sprintf_s(szTemp, "%02x", pIpAdapterInfo->Address[i]);
				strcat_s(szMac, szTemp);
			}
			else
			{
				sprintf_s(szTemp, "%02x-", pIpAdapterInfo->Address[i]);
				strcat_s(szMac, szTemp);
			}
		}		
		strcpy_s(szIP, pIpAdapterInfo->IpAddressList.IpAddress.String); //IP

		//输出网卡信息
#ifdef UNICODE
		USES_CONVERSION;
		if (ptszIpBuf)
		{
			_tcscpy(ptszIpBuf, A2W(szIP));
		}
		if (ptszMacBuf)
		{
			_tcscpy(ptszMacBuf, A2W(szMac));
		}
#else
		if (ptszIpBuf)
		{
			_tcscpy(ptszIpBuf, szIP);
		}
		if (ptszMacBuf)
		{
			_tcscpy(ptszMacBuf, szMac);
		}
#endif
	}
	//释放内存空间
	if (pIpAdapterInfo)
	{
		delete pIpAdapterInfo;
	}

	return TRUE;
}



//获取进程的用户名
BOOL GetProcessUser(DWORD dwProcessID, TCHAR *szUserName, DWORD nNameLen)
{
	BOOL fResult  = FALSE;
	HANDLE hProc  = NULL;
	HANDLE hToken = NULL;
	TOKEN_USER *pTokenUser = NULL;

	__try
	{
		// Open the process with PROCESS_QUERY_INFORMATION access
		hProc = OpenProcess(PROCESS_QUERY_INFORMATION, FALSE, dwProcessID);
		if (hProc == NULL)
		{
			__leave;
		}
		fResult = OpenProcessToken(hProc, TOKEN_QUERY, &hToken);
		if(!fResult)  
		{
			__leave;
		}

		DWORD dwNeedLen = 0;		
		fResult = GetTokenInformation(hToken,TokenUser, NULL, 0, &dwNeedLen);
		if (dwNeedLen > 0)
		{
			pTokenUser = (TOKEN_USER*)new BYTE[dwNeedLen];
			fResult = GetTokenInformation(hToken,TokenUser, pTokenUser, dwNeedLen, &dwNeedLen);
			if (!fResult)
			{
				__leave;
			}
		}
		else
		{
			__leave;
		}

		SID_NAME_USE sn;
		TCHAR szDomainName[MAX_PATH];
		DWORD dwDmLen = MAX_PATH;
		fResult = LookupAccountSid(NULL, pTokenUser->User.Sid, szUserName, &nNameLen,
			szDomainName, &dwDmLen, &sn);
	}
	__finally
	{
		if (hProc)
			::CloseHandle(hProc);
		if (hToken)
			::CloseHandle(hToken);
		if (pTokenUser)
			delete[] (char*)pTokenUser;		
	}
	return fResult;
}

//获取当前用户名  获取进程的用户名
BOOL GetUser(LPTSTR pUser)
{
	BOOL bRet = FALSE;
	HANDLE hSnapshot = NULL;

	if (!pUser)
	{
		return false;
	}

	__try
	{
		// Get a snapshot of the processes in the system
		hSnapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
		if (hSnapshot == NULL)
		{            
			__leave;
		}

		PROCESSENTRY32 pe32;
		pe32.dwSize = sizeof(pe32);

		// Find the "System" process
		BOOL fProcess = Process32First(hSnapshot, &pe32);
		while (fProcess)
		{
			if (lstrcmpi(pe32.szExeFile, TEXT("explorer.exe")) == 0)
			{	
				TCHAR szUserName[MAX_PATH] = {0};
				if (GetProcessUser(pe32.th32ProcessID, szUserName, MAX_PATH))
				{
					bRet = TRUE;
					//wcscpy(pUser, szUserName);
				}

				break;
			}
			fProcess = Process32Next(hSnapshot, &pe32);
		}
		if (!fProcess)
		{			
			__leave;    // Didn't find "System" process
		}
	}
	__finally
	{
		// Cleanup the snapshot
		if (hSnapshot != NULL)
			CloseHandle(hSnapshot);
	}

	return bRet;	

}