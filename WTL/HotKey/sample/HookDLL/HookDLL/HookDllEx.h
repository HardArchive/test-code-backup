

#ifndef _HOOKDLLEX_H
#define _HOOKDLLEX_H
#include <windows.h>
#include <winuser.h>
#include <stdio.h>

/************************************************************************/
//#define LLKHF_EXTENDED       0x00000001
//#define LLKHF_INJECTED       0x00000010
//#define LLKHF_ALTDOWN        0x00000020
//#define LLKHF_UP             0x00000080
//#define LLMHF_INJECTED       0x00000001  
//#define WH_KEYBOARD_LL       13
/************************************************************************/
extern  DWORD MY_VK_LBUTTON        ;
extern  DWORD MY_VK_RBUTTON        ;
extern  DWORD MY_VK_CANCEL         ;
extern  DWORD MY_VK_MBUTTON        ;

extern  DWORD MY_VK_BACK           ;
extern  DWORD MY_VK_TAB            ;

extern  DWORD MY_VK_CLEAR          ;
extern  DWORD MY_VK_RETURN         ;

extern  DWORD MY_VK_SHIFT          ;
extern  DWORD MY_VK_CONTROL        ;
extern  DWORD MY_VK_MENU           ;
extern  DWORD MY_VK_PAUSE          ;
extern  DWORD MY_VK_CAPITAL        ;

extern  DWORD MY_VK_KANA           ;
extern  DWORD MY_VK_HANGEUL        ;
extern  DWORD MY_VK_HANGUL         ;
extern  DWORD MY_VK_JUNJA          ;
extern  DWORD MY_VK_FINAL          ;
extern  DWORD MY_VK_HANJA          ;
extern  DWORD MY_VK_KANJI          ;

extern  DWORD MY_VK_ESCAPE         ;

extern  DWORD MY_VK_CONVERT        ;
extern  DWORD MY_VK_NONCONVERT     ;
extern  DWORD MY_VK_ACCEPT         ;
extern  DWORD MY_VK_MODECHANGE     ;

extern  DWORD MY_VK_SPACE          ;
extern  DWORD MY_VK_PRIOR          ;
extern  DWORD MY_VK_NEXT           ;
extern  DWORD MY_VK_END            ;
extern  DWORD MY_VK_HOME           ;
extern  DWORD MY_VK_LEFT           ;
extern  DWORD MY_VK_UP             ;
extern  DWORD MY_VK_RIGHT          ;
extern  DWORD MY_VK_DOWN           ;
extern  DWORD MY_VK_SELECT         ;
extern  DWORD MY_VK_PRDWORD          ;
extern  DWORD MY_VK_EXECUTE        ;
extern  DWORD MY_VK_SNAPSHOT       ;
extern  DWORD MY_VK_INSERT         ;
extern  DWORD MY_VK_DELETE         ;

//////////////////////////////////////////////////////////////////////////
extern  DWORD MY_VK_LWIN           ;
extern  DWORD MY_VK_RWIN           ;
extern  DWORD MY_VK_APPS           ;

extern  DWORD MY_VK_NUMPAD0        ;
extern  DWORD MY_VK_NUMPAD1        ;
extern  DWORD MY_VK_NUMPAD2        ;
extern  DWORD MY_VK_NUMPAD3        ;
extern  DWORD MY_VK_NUMPAD4        ;
extern  DWORD MY_VK_NUMPAD5        ;
extern  DWORD MY_VK_NUMPAD6        ;
extern  DWORD MY_VK_NUMPAD7        ;
extern  DWORD MY_VK_NUMPAD8        ;
extern  DWORD MY_VK_NUMPAD9        ;
extern  DWORD MY_VK_MULTIPLY       ;
extern  DWORD MY_VK_ADD            ;
extern  DWORD MY_VK_SEPARATOR      ;
extern  DWORD MY_VK_SUBTRACT       ;
extern  DWORD MY_VK_DECIMAL        ;
extern  DWORD MY_VK_DIVIDE         ;
extern  DWORD MY_VK_F1             ;
extern  DWORD MY_VK_F2             ;
extern  DWORD MY_VK_F3             ;
extern  DWORD MY_VK_F4             ;
extern  DWORD MY_VK_F5             ;
extern  DWORD MY_VK_F6             ;
extern  DWORD MY_VK_F7             ;
extern  DWORD MY_VK_F8             ;
extern  DWORD MY_VK_F9             ;
extern  DWORD MY_VK_F10            ;
extern  DWORD MY_VK_F11            ;
extern  DWORD MY_VK_F12            ;
extern  DWORD MY_VK_F13            ;
extern  DWORD MY_VK_F14            ;
extern  DWORD MY_VK_F15            ;
extern  DWORD MY_VK_F16            ;
extern  DWORD MY_VK_F17            ;
extern  DWORD MY_VK_F18            ;
extern  DWORD MY_VK_F19            ;
extern  DWORD MY_VK_F20            ;
extern  DWORD MY_VK_F21            ;
extern  DWORD MY_VK_F22            ;
extern  DWORD MY_VK_F23            ;
extern  DWORD MY_VK_F24            ;
//////////////////////////////////////////////////////////////////////////
// 0--9 ,a-z ,A-Z
extern  DWORD MY_VK_NUMLOCK        ;
extern  DWORD MY_VK_SCROLL         ;

extern  DWORD MY_VK_A              ;
extern  DWORD MY_VK_B              ;
extern  DWORD MY_VK_C              ;
extern  DWORD MY_VK_D              ;
extern  DWORD MY_VK_E              ;
extern  DWORD MY_VK_F              ;
extern  DWORD MY_VK_V              ;
extern  DWORD MY_VK_X              ;
extern  DWORD MY_VK_Z              ;

extern  DWORD MY_VK_a              ;
extern  DWORD MY_VK_b              ;
extern  DWORD MY_VK_c              ;
extern  DWORD MY_VK_d              ;
extern  DWORD MY_VK_e              ;
extern  DWORD MY_VK_f              ;
extern  DWORD MY_VK_v              ;
extern  DWORD MY_VK_x              ;
extern  DWORD MY_VK_z              ; 

//////////////////////////////////////////////////////////////////////////
extern   FILE       *fp_Taskmgr;  // 用于 打开关闭任务管理器

extern   const DWORD  ZEROKEY ;  // 按键清零

#endif