 
#include "Global.h"

bool b_show = true;//决定主窗口是否隐藏

CString path = "D:\\Program Files\\PenMount Universal Driver\\hotKey.ini";
CString pathName = "D:\\Program Files\\PenMount Universal Driver";

//CString path = "hotKey.ini";
//CString pathName = "";
HINSTANCE g_hDLL=NULL;

P_HookDLL  hookdll;
P_Taskmgr  setTaskMgr;
P_Snashot  setSnashot;
P_Table    setTab;
P_Esc      setEsc;
P_Delete   setDelete;



P_CTR_A    set_Ctrl_A;
P_CTR_C    set_Ctrl_C;
P_CTR_D    set_Ctrl_D;
P_CTR_E    set_Ctrl_E;
P_CTR_F    set_Ctrl_F;
P_CTR_V    set_Ctrl_V;
P_CTR_X    set_Ctrl_X;
P_CTR_Z    set_Ctrl_Z;