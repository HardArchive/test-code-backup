#include <windows.h>
#include <winuser.h>
#include <stdio.h>
#include "HookDllEx.h"
#include "KillProcess.h"
#include "HookDll.h"
#include "HookFun.h"

//#define _WIN32_WINNT 0x0500 // for KBDLLHOOKSTRUCT


// Í¨¹ýdumpbin -Headers ²é½Ú

#pragma data_seg("mydata")

HWND hgetwnd = NULL;   // 
HHOOK hgetmouse = NULL;  // 
HHOOK hgetkeyboard = NULL;
HHOOK hhkLowLevelKybd = NULL;  // ½Úµã

#pragma data_seg()

#pragma comment(linker,"/section:mydata,RWS")


BOOL key_del=FALSE,key_alt=FALSE,key_ctrl=FALSE;

LRESULT CALLBACK LowLevelKeyboardProc(
									   int nCode,   
									   WPARAM wParam, 
									   LPARAM lParam)   
{ 
	PKBDLLHOOKSTRUCT p = (PKBDLLHOOKSTRUCT)lParam;
	BOOL fEatKeystroke = FALSE; 

	if   (nCode == HC_ACTION)
	{ 

		switch(wParam)
		{ 
			case   WM_KEYDOWN:    
			case   WM_SYSKEYDOWN:
			case   WM_KEYUP: 
			case   WM_SYSKEYUP:  
            
			// ½ûÓÃF1--F12
		    if (Dis_F1ToF12(nCode,wParam,lParam))
		    {
				return 1;
		    }
		   
			//////////////////////////////////////////////////////////////////////////
		    // Ctrl+A,B,C,D,E,F,V,X,Z
			if ((((GetKeyState(VK_CONTROL)&0x8000)!=0) && ((p->vkCode==MY_VK_A)||(p->vkCode==MY_VK_a)))||
				(((GetKeyState(VK_CONTROL)&0x8000)!=0) && ((p->vkCode==MY_VK_B)||(p->vkCode==MY_VK_b)))||
				(((GetKeyState(VK_CONTROL)&0x8000)!=0) && ((p->vkCode==MY_VK_C)||(p->vkCode==MY_VK_c)))||
				(((GetKeyState(VK_CONTROL)&0x8000)!=0) && ((p->vkCode==MY_VK_D)||(p->vkCode==MY_VK_d)))||
				(((GetKeyState(VK_CONTROL)&0x8000)!=0) && ((p->vkCode==MY_VK_E)||(p->vkCode==MY_VK_e)))||
				(((GetKeyState(VK_CONTROL)&0x8000)!=0) && ((p->vkCode==MY_VK_F)||(p->vkCode==MY_VK_f)))||
				(((GetKeyState(VK_CONTROL)&0x8000)!=0) && ((p->vkCode==MY_VK_V)||(p->vkCode==MY_VK_v)))||
				(((GetKeyState(VK_CONTROL)&0x8000)!=0) && ((p->vkCode==MY_VK_X)||(p->vkCode==MY_VK_x)))||
				(((GetKeyState(VK_CONTROL)&0x8000)!=0) && ((p->vkCode==MY_VK_Z)||(p->vkCode==MY_VK_z)))
				)
			{
				return 1;
			}
            
	         
			fEatKeystroke = 
				
                    ((p->vkCode==VK_ESCAPE)&&(p->vkCode==VK_SHIFT)&&((GetKeyState(VK_CONTROL)&0x8000)!=0))|| // // ÆÁ±ÎCtrl + Alt+ ESC
					
					((p->vkCode==VK_TAB)&&((p->flags&LLKHF_ALTDOWN) != 0))||          // ÆÁ±ÎAlt+Tab

					((p->vkCode==VK_ESCAPE)&&((p->flags & LLKHF_ALTDOWN)!=0))||       // ÆÁ±ÎAlt+¿Õ¸ñ

					((p->vkCode==VK_ESCAPE)&&((GetKeyState(VK_CONTROL)&0x8000)!=0))|| // ÆÁ±ÎAlt+ ESC

					((p->vkCode==VK_SHIFT)&&((p->flags & LLKHF_ALTDOWN)!=0))||         //    Alt+ shift

					((GetKeyState(MY_VK_CONTROL)&0x8000)!=0)&&(p->vkCode==MY_VK_PAUSE)||
                        
					(p->vkCode==MY_VK_F4)&&((p->flags & LLKHF_ALTDOWN)!=0)||

					(p->vkCode==VK_LWIN)||(p->vkCode==VK_RWIN)||                      // ÆÁ±Î×óÓÒÁ½±ßµÄ Win ¼ü

					((p->flags&LLKHF_ALTDOWN) != 0)||     //ÆÁ±Î ALT

	// 			    ((GetKeyState(VK_CONTROL)&0x8000)!=0)||(p->vkCode==VK_PRINT)||    // ÆÁ±Î´òÓ¡ÈÈ¼ü

 					(p->vkCode==MY_VK_PAUSE)||

					(p->vkCode==MY_VK_SNAPSHOT)||   // ½ØÍ¼

					(p->vkCode==MY_VK_ESCAPE)||

					(p->vkCode==MY_VK_MENU)||

					(p->vkCode==MY_VK_TAB) ||
					(p->vkCode==MY_VK_DELETE) 
	
					;
			
				break; 
 			default:
 				break;
		} 
	} 
	return(fEatKeystroke?1:CallNextHookEx(hhkLowLevelKybd,nCode,wParam,lParam)); 
} 
			
LRESULT CALLBACK MouseProc(
						    int nCode,
						    WPARAM wParam,
						    LPARAM lParam)
{
	return 1;
}

LRESULT CALLBACK KeyboardProc(
							   int code,
							   WPARAM wParam,
							   LPARAM lParam
							  )
{	PKBDLLHOOKSTRUCT p = (PKBDLLHOOKSTRUCT) lParam;

// 	if(	(wParam == VK_CONTROL))
// 	{
// 
// 	    SendMessage(hgetwnd,WM_CLOSE,0,0);
// //		UnhookWindowsHookEx(hgetmouse);
// 		UnhookWindowsHookEx(hgetkeyboard);
// 		UnhookWindowsHookEx(hhkLowLevelKybd);
// 	}
//     else ;

      return CallNextHookEx(hgetkeyboard,code,wParam,lParam); 

}
				
						
void setHook(HWND hwnd)
{
	hgetwnd = hwnd;

    //  hgetmouse = SetWindowsHookEx(WH_MOUSE,MouseProc,GetModuleHandle("HookDll.dll"),0);
	hgetkeyboard = SetWindowsHookEx(WH_KEYBOARD,(HOOKPROC)KeyboardProc,GetModuleHandle("HookDll.dll"),0);
	// µÍ¼¶¼üÅÌ¹³×Ó
    hhkLowLevelKybd = SetWindowsHookEx(WH_KEYBOARD_LL,(HOOKPROC)LowLevelKeyboardProc,GetModuleHandle("HookDll.dll"),0); 
    
}

void unHook(HWND hwnd)
{	
	hgetwnd = hwnd;
	SendMessage(hgetwnd,WM_CLOSE,0,0);
//	UnhookWindowsHookEx(hgetmouse);
	UnhookWindowsHookEx(hgetkeyboard);
	UnhookWindowsHookEx(hhkLowLevelKybd);
}