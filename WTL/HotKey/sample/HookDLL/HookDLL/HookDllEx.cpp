#include "HookDllEx.h"

DWORD MY_VK_LBUTTON        =  0x00;
DWORD MY_VK_RBUTTON        =  0x00;
DWORD MY_VK_CANCEL         =  0x00;
DWORD MY_VK_MBUTTON        =  0x00;

DWORD MY_VK_BACK           =  0x00;
DWORD MY_VK_TAB            =  0x00;

DWORD MY_VK_CLEAR          =  0x00;
DWORD MY_VK_RETURN         =  0x00;

DWORD MY_VK_SHIFT          =  0x00;
DWORD MY_VK_CONTROL        =  0x00;
DWORD MY_VK_MENU           =  0x00;
DWORD MY_VK_PAUSE          =  0x00;
DWORD MY_VK_CAPITAL        =  0x00;

DWORD MY_VK_KANA           =  0x00;
DWORD MY_VK_HANGEUL        =  0x00;
DWORD MY_VK_HANGUL         =  0x00;
DWORD MY_VK_JUNJA          =  0x00;
DWORD MY_VK_FINAL          =  0x00;
DWORD MY_VK_HANJA          =  0x00;
DWORD MY_VK_KANJI          =  0x00;

DWORD MY_VK_ESCAPE         =  0x00;

DWORD MY_VK_CONVERT        =  0x00;
DWORD MY_VK_NONCONVERT     =  0x00;
DWORD MY_VK_ACCEPT         =  0x00;
DWORD MY_VK_MODECHANGE     =  0x00;

DWORD MY_VK_SPACE          =  0x00;
DWORD MY_VK_PRIOR          =  0x00;
DWORD MY_VK_NEXT           =  0x00;
DWORD MY_VK_END            =  0x00;
DWORD MY_VK_HOME           =  0x00;
DWORD MY_VK_LEFT           =  0x00;
DWORD MY_VK_UP             =  0x00;
DWORD MY_VK_RIGHT          =  0x00;
DWORD MY_VK_DOWN           =  0x00;
DWORD MY_VK_SELECT         =  0x00;
DWORD MY_VK_PRDWORD          =  0x00;
DWORD MY_VK_EXECUTE        =  0x00;
DWORD MY_VK_SNAPSHOT       =  0x00;
DWORD MY_VK_INSERT         =  0x00;
DWORD MY_VK_DELETE         =  0x00;


//////////////////////////////////////////////////////////////////////////
// 功能键

DWORD MY_VK_LWIN           =  0x00;
DWORD MY_VK_RWIN           =  0x00;
DWORD MY_VK_APPS           =  0x00;

DWORD MY_VK_NUMPAD0        =  0x00;
DWORD MY_VK_NUMPAD1        =  0x00;
DWORD MY_VK_NUMPAD2        =  0x00;
DWORD MY_VK_NUMPAD3        =  0x00;
DWORD MY_VK_NUMPAD4        =  0x00;
DWORD MY_VK_NUMPAD5        =  0x00;
DWORD MY_VK_NUMPAD6        =  0x00;
DWORD MY_VK_NUMPAD7        =  0x00;
DWORD MY_VK_NUMPAD8        =  0x00;
DWORD MY_VK_NUMPAD9        =  0x00;
DWORD MY_VK_MULTIPLY       =  0x00;
DWORD MY_VK_ADD            =  0x00;
DWORD MY_VK_SEPARATOR      =  0x00;
DWORD MY_VK_SUBTRACT       =  0x00;
DWORD MY_VK_DECIMAL        =  0x00;
DWORD MY_VK_DIVIDE         =  0x00;
DWORD MY_VK_F1             =  0x00;
DWORD MY_VK_F2             =  0x00;
DWORD MY_VK_F3             =  0x00;
DWORD MY_VK_F4             =  0x00;
DWORD MY_VK_F5             =  0x00;
DWORD MY_VK_F6             =  0x00;
DWORD MY_VK_F7             =  0x00;
DWORD MY_VK_F8             =  0x00;
DWORD MY_VK_F9             =  0x00;
DWORD MY_VK_F10            =  0x00;
DWORD MY_VK_F11            =  0x00;
DWORD MY_VK_F12            =  0x00;
DWORD MY_VK_F13            =  0x00;
DWORD MY_VK_F14            =  0x00;
DWORD MY_VK_F15            =  0x00;
DWORD MY_VK_F16            =  0x00;
DWORD MY_VK_F17            =  0x00;
DWORD MY_VK_F18            =  0x00;
DWORD MY_VK_F19            =  0x00;
DWORD MY_VK_F20            =  0x00;
DWORD MY_VK_F21            =  0x00;
DWORD MY_VK_F22            =  0x00;
DWORD MY_VK_F23            =  0x00;
DWORD MY_VK_F24            =  0x00;

DWORD MY_VK_NUMLOCK        =  0x00;
DWORD MY_VK_SCROLL         =  0x00;
//////////////////////////////////////////////////////////////////////////
// 数字 字母
/* VK_0 thru VK_9 are the same as ASCII '0' thru '9' (0x30 - 0x39) */
/* VK_A thru VK_Z are the same as ASCII 'A' thru 'Z' (0x41 - 0x5A) */
DWORD MY_VK_0             =  0x00;
DWORD MY_VK_1             =  0x00;

DWORD MY_VK_A             =  0x00;
DWORD MY_VK_B             =  0x00;
DWORD MY_VK_C             =  0x00;
DWORD MY_VK_D             =  0x00;
DWORD MY_VK_E             =  0x00;
DWORD MY_VK_F             =  0x00;
DWORD MY_VK_V             =  0x00;
DWORD MY_VK_X             =  0x00;
DWORD MY_VK_Z             =  0x00;

DWORD MY_VK_a             =  0x00;
DWORD MY_VK_b             =  0x00;
DWORD MY_VK_c             =  0x00;
DWORD MY_VK_d             =  0x00;
DWORD MY_VK_e             =  0x00;
DWORD MY_VK_f             =  0x00;
DWORD MY_VK_v             =  0x00;
DWORD MY_VK_x             =  0x00;
DWORD MY_VK_z             =  0x00;

//////////////////////////////////////////////////////////////////////////

FILE          *fp_Taskmgr;     // 用于 打开关闭任务管理器

const DWORD  ZEROKEY = 0x00;     // 按键清零