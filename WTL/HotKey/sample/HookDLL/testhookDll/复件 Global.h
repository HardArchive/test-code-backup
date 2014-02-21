#ifndef  __GLOBAL_H
#define  __GLOBAL_H

#include "StdAfx.h"

extern  CString path ;
extern  CString pathName ;

extern  HINSTANCE g_hDLL;


typedef  bool (*P_CTR_A)(bool bl);
typedef  bool (*P_CTR_C)(bool bl);
typedef  bool (*P_CTR_D)(bool bl);
typedef  bool (*P_CTR_E)(bool bl);
typedef  bool (*P_CTR_F)(bool bl);
typedef  bool (*P_CTR_V)(bool bl);
typedef  bool (*P_CTR_X)(bool bl);
typedef  bool (*P_CTR_Z)(bool bl);

typedef  bool (*P_HookDLL)(HWND hwnd);
typedef  int  (*P_Taskmgr)(bool bl);
typedef  bool (*P_Snashot)(bool bl);
typedef  bool (*P_Delete)(bool bl);
typedef  bool (*P_Esc)(bool bl);
typedef  bool (*P_Table)(bool bl);



extern  P_HookDLL  hookdll;
extern  P_Taskmgr  setTaskMgr;
extern  P_Snashot  setSnashot;
extern  P_Table    setTab;
extern  P_Esc      setEsc;
extern  P_Delete   setDelete;



extern  P_CTR_A    set_Ctrl_A;
extern  P_CTR_C    set_Ctrl_C;
extern  P_CTR_D    set_Ctrl_D;
extern  P_CTR_E    set_Ctrl_E;
extern  P_CTR_F    set_Ctrl_F;
extern  P_CTR_V    set_Ctrl_V;
extern  P_CTR_X    set_Ctrl_X;
extern  P_CTR_Z    set_Ctrl_Z;

#endif