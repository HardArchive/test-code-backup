// Hook.cpp : Defines the initialization routines for the DLL.
#include "stdafx.h"
#include "hook.h"
#include <windowsx.h>
#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif
#define MAX_KEY 100
#define CTRLBIT 0x04
#define ALTBIT  0x02
#define SHIFTBIT 0x01 
/////////////////////////////////////////////////////////////////
#pragma data_seg("shareddata")
HHOOK hHook							=NULL;
UINT nHookCount						=0;
static UCHAR HotKey[MAX_KEY]		= {0};		//hotkey
static UCHAR HotKeyMask[MAX_KEY]	= {0};		//flag for hotkey, value is VK_CONTRL or VK_NEMU or VK_SHIFT
static HWND hCallWnd[MAX_KEY]		= {0};		//window associated with hotkey
static int KeyCount					=0;
static UCHAR MaskBits				=0;		   //00000 Ctrl Alt Shift
#pragma data_seg()
////////////////////////////////////////////////////////////////
HINSTANCE hins;
void VerifyWindow();
/////////////////////////////////////////////////////////////////////////////
// CHookApp
BEGIN_MESSAGE_MAP(CHookApp, CWinApp)
	//{{AFX_MSG_MAP(CHookApp)
		// NOTE - the ClassWizard will add and remove mapping macros here.
		//    DO NOT EDIT what you see in these blocks of generated code!
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CHookApp construction

CHookApp::CHookApp()
{
	// TODO: add construction code here,
	// Place all significant initialization in InitInstance
}

/////////////////////////////////////////////////////////////////////////////
// The one and only CHookApp object

CHookApp theApp;
LRESULT CALLBACK KeyboardProc(int nCode,WPARAM wParam,LPARAM lParam)
{	
	BOOL bProcessed=FALSE;
	if(HC_ACTION==nCode)
	{
		if((lParam&0xc0000000)==0xc0000000){// Key up
			switch(wParam)
			{
			case VK_MENU:
				MaskBits&=~ALTBIT;
				break;
			case VK_CONTROL:
				MaskBits&=~CTRLBIT;
				break;
			case VK_SHIFT:
				MaskBits&=~SHIFTBIT;
				break;
			default:	//judge the key and send message
				break;
			}
			for(int index=0;index<MAX_KEY;index++){
				if(hCallWnd[index]==NULL)
					continue;
				if(IsWindow(hCallWnd[index])&&(HotKey[index]==wParam)&&(HotKeyMask[index]==MaskBits)){
						SendMessage(hCallWnd[index],WM_HOTKEY,wParam,WM_KEYUP);
						bProcessed=TRUE;
				}
			}
		}
		else if((lParam&0xc000ffff)==1){	//Key down
			switch(wParam)
			{
			case VK_MENU:
				MaskBits|=ALTBIT;
				break;
			case VK_CONTROL:
				MaskBits|=CTRLBIT;
				break;
			case VK_SHIFT:
				MaskBits|=SHIFTBIT;
				break;
			default:	//judge the key and send message
				break;
			}
			for(int index=0;index<MAX_KEY;index++){
				if(hCallWnd[index]==NULL)
					continue;
				if(IsWindow(hCallWnd[index])&&(HotKey[index]==wParam)&&(HotKeyMask[index]==MaskBits)){
						SendMessage(hCallWnd[index],WM_HOTKEY,wParam,WM_KEYDOWN);
						bProcessed=TRUE;
				}
			}
		}
		if(!bProcessed){
			for(int index=0;index<MAX_KEY;index++){
				if(hCallWnd[index]==NULL)
					continue;
				if(IsWindow(hCallWnd[index])&&(HotKey[index]==0)&&(HotKeyMask[index]==0))
					SendMessage(hCallWnd[index],WM_HOTKEY,WM_HOTKEY,lParam);
			}
		}
	}
	return CallNextHookEx( hHook, nCode, wParam, lParam );	
}

BOOL InitHotkey()
{
	if(hHook!=NULL){
		nHookCount++;
		return TRUE;
	}
	else
		hHook=SetWindowsHookEx(WH_KEYBOARD,(HOOKPROC)KeyboardProc,hins,0);
	if(hHook!=NULL)
		nHookCount++;
	return (hHook!=NULL);
}
BOOL UnInit()
{   
	if(nHookCount>1){
		nHookCount--;
		return TRUE;
	}
	BOOL unhooked = UnhookWindowsHookEx(hHook);
	if(unhooked==TRUE){
		nHookCount=0;
		hHook=NULL;
	}
	return unhooked;
} 

BOOL __declspec(dllexport) __stdcall AddHotkey(HWND hWnd,UCHAR cKey,UCHAR cMask)
{
	BOOL bAdded=FALSE;
	for(int index=0;index<MAX_KEY;index++){
		if(hCallWnd[index]==0){
			hCallWnd[index]=hWnd;
			HotKey[index]=cKey;
			HotKeyMask[index]=cMask;
			bAdded=TRUE;
			KeyCount++;
			break;
		}
	}
	return bAdded;
}

BOOL __declspec(dllexport) __stdcall DeleteHotkey(HWND hWnd,UCHAR cKey,UCHAR cMask)
{
	BOOL bRemoved=FALSE;
	for(int index=0;index<MAX_KEY;index++){
		if(hCallWnd[index]==hWnd){
			if(HotKey[index]==cKey&&HotKeyMask[index]==cMask){
				hCallWnd[index]=NULL;
				HotKey[index]=0;
				HotKeyMask[index]=0;
				bRemoved=TRUE;
				KeyCount--;
				break;
			}
		}
	}
	return bRemoved;
}

void VerifyWindow()
{
	for(int i=0;i<MAX_KEY;i++){
		if(hCallWnd[i]!=NULL){
			if(!IsWindow(hCallWnd[i])){
				hCallWnd[i]=NULL;
				HotKey[i]=0;
				HotKeyMask[i]=0;
				KeyCount--;
			}
		}
	}
}

BOOL CHookApp::InitInstance() 
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	hins=AfxGetInstanceHandle();
	InitHotkey();
	return CWinApp::InitInstance();
}

int CHookApp::ExitInstance() 
{
	VerifyWindow();
	UnInit();
	return CWinApp::ExitInstance();
}
