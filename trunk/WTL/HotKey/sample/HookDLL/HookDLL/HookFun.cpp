#include "HookFun.h"
#include "HookDll.h"
#include "HookDllEx.h"
#include "KillProcess.h"
#include <windows.h>
#include <winuser.h>
#include <stdio.h>
 
//   1:true  , 0:false 
// 非0:true  , 0:false

//////////////////////////////////////////////////////////////////////////
// 开启bl为true ，关闭bl为false
int set_CTRL_SHIFT_ESC(bool bl)
{
	if (!bl)
	{
		MY_VK_CONTROL = (int)VK_CONTROL;
		MY_VK_SHIFT   = (int)VK_SHIFT;
		MY_VK_ESCAPE = (int)VK_ESCAPE;
		
	}
	else
	{
		MY_VK_CONTROL = ZEROKEY;
		MY_VK_SHIFT   = ZEROKEY;
		MY_VK_ESCAPE = ZEROKEY;
	}
	return 0;
	
}

//////////////////////////////////////////////////////////////////////////
// -1 失败  0成功开放任务管理器  1成功关禁用任务管理器   1
int  set_Taskmgr(bool bl)
{
	HANDLE handle; //定义CreateToolhelp32Snapshot系统快照句柄 
	handle=CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS,0);//获得系统快照句柄 	
	PROCESSENTRY32 info; //定义PROCESSENTRY32结构字指 	
	info.dwSize=sizeof(PROCESSENTRY32); 
	
//	PROCESS_INFORMATION pi;    
	STARTUPINFO si;    
	memset(&si,0,sizeof(si));   
	si.cb=sizeof(si);  
	
	si.wShowWindow=SW_SHOW; 
	si.dwFlags=STARTF_USESHOWWINDOW; 
	
	if (bl)
	{
		set_CTRL_SHIFT_ESC(true); // 开放此键
		if (fp_Taskmgr!=NULL)
		{
				fclose(fp_Taskmgr);   //  释放文件 
		}
	
		if (KKillByName(handle,info,"taskmgr.exe") )
		{
			return 0;  // 开放任务管理器
		}
		else
		{
			return -1;  // 失败
		}
	}
	else 
	{  
		set_CTRL_SHIFT_ESC(false); // 禁用组合键
		KKillByName(handle,info,"taskmgr.exe"); // 先删掉进程
		Sleep(800);
		fp_Taskmgr = fopen("C:\\WINDOWS\\system32\\taskmgr.exe","ab");
		//fp_Taskmgr = fopen_s("C:\\WINDOWS\\system32\\taskmgr.exe","ab");
//		fclose(fp);   // 不能够关闭
	}
	return 0;  // 禁止任务管理器
}

//////////////////////////////////////////////////////////////////////////
// 开关左右win键
// int set_WIN_LR(bool bl) 
// {
//     if (!bl)
//     {
// 		MY_VK_RWIN = (int)VK_RWIN;
// 	    MY_VK_LWIN = (int)VK_LWIN;
//     }
// 	else
// 	{
// 		MY_VK_LWIN = ZEROKEY;
// 		MY_VK_RWIN = ZEROKEY;
// 	}
// 	return 0;
// }

//////////////////////////////////////////////////////////////////////////
// 
// 开启bl为true ，关闭bl为false
bool set_Delete(bool bl)
{
	if (bl)
	{
		MY_VK_DELETE = ZEROKEY;
	}
	else
	{
		MY_VK_DELETE = VK_DELETE;
	}
	return true;
}

//////////////////////////////////////////////////////////////////////////
// 
// 开启bl为true ，关闭bl为false
bool set_Snashot(bool bl)
{
     if (bl)
     {
         MY_VK_SNAPSHOT = ZEROKEY;
     } 
     else
     {
		 MY_VK_SNAPSHOT = VK_SNAPSHOT;
     }
	 return true;
}

//////////////////////////////////////////////////////////////////////////
// 
// 开启bl为true ，关闭bl为false

bool set_Ctrl_A(bool bl)
{
	if (bl)
	{
		MY_VK_A = ZEROKEY;
        MY_VK_a = ZEROKEY;
	} 
	else
	{
		MY_VK_A = 'A';
 		MY_VK_a = 'a';
	}
	return true;
}

//////////////////////////////////////////////////////////////////////////
// 
// 开启bl为true ，关闭bl为false

bool set_Ctrl_B(bool bl)
{
	if (bl)
	{
		MY_VK_B = ZEROKEY;
        MY_VK_b = ZEROKEY;
	} 
	else
	{
		MY_VK_B = 'B';
		MY_VK_b = 'b';
	}
	return true;
}

/////////////////////////////////////////////////////////////////////////
// 
// 开启bl为true ，关闭bl为false
bool set_Ctrl_C(bool bl)
{
	if (bl)
	{
		MY_VK_C = ZEROKEY;
        MY_VK_c = ZEROKEY;
	} 
	else
	{
		MY_VK_C = 'C';
		MY_VK_c = 'c';
	}
	return true;
}

/////////////////////////////////////////////////////////////////////////
// 开启bl为true ，关闭bl为false
bool set_Ctrl_D(bool bl)
{
	if (bl)
	{
		MY_VK_D = ZEROKEY;
        MY_VK_d = ZEROKEY;
	} 
	else
	{
		MY_VK_D = 'D';
		MY_VK_d = 'd';
	}
	return true;
}

/////////////////////////////////////////////////////////////////////////
// 开启bl为true ，关闭bl为false
bool set_Ctrl_E(bool bl)
{
	if (bl)
	{
		MY_VK_E = ZEROKEY;
        MY_VK_e = ZEROKEY;
	} 
	else
	{
		MY_VK_E = 'E';
		MY_VK_e = 'e';
	}
	return true;
}

/////////////////////////////////////////////////////////////////////////
// 开启bl为true ，关闭bl为false
bool set_Ctrl_F(bool bl)
{
	if (bl)
	{
		MY_VK_F = ZEROKEY;
        MY_VK_f = ZEROKEY;
	} 
	else
	{
		MY_VK_F = 'F';
		MY_VK_f = 'f';
	}
	return true;
}

/////////////////////////////////////////////////////////////////////////
// 开启bl为true ，关闭bl为false
bool set_Ctrl_V(bool bl)
{
	if (bl)
	{
		MY_VK_V = ZEROKEY;
        MY_VK_v = ZEROKEY;
	} 
	else
	{
		MY_VK_V = 'V';
		MY_VK_v = 'v';
	}
	return true;
}

/////////////////////////////////////////////////////////////////////////
// 开启bl为true ，关闭bl为false
bool set_Ctrl_X(bool bl)
{
	if (bl)
	{
		MY_VK_F = ZEROKEY;
        MY_VK_f = ZEROKEY;
	} 
	else
	{
		MY_VK_X = 'X';
		MY_VK_x = 'x';
	}
	return true;
}

/////////////////////////////////////////////////////////////////////////
// 开启bl为true ，关闭bl为false
bool set_Ctrl_Z(bool bl)
{
	if (bl)
	{
		MY_VK_Z = ZEROKEY;
        MY_VK_z = ZEROKEY;
	} 
	else
	{
		MY_VK_Z = 'Z';
		MY_VK_z = 'z';
	}
	return true;
}
//////////////////////////////////////////////////////////////////////////
// 
// 开启bl为true ，关闭bl为false

bool Dis_F1ToF12(int nCode,   WPARAM wParam, LPARAM lParam) 
{
    PKBDLLHOOKSTRUCT p = (PKBDLLHOOKSTRUCT)lParam;
	// F1-F12 :112--123  也可以用数值判断
 
	if ( (p->vkCode==VK_F1)||(p->vkCode==VK_F2)||(p->vkCode==VK_F3)||(p->vkCode==VK_F4)
		||(p->vkCode==VK_F5)||(p->vkCode==VK_F6)||(p->vkCode==VK_F7)||(p->vkCode==VK_F8)
		||(p->vkCode==VK_F9)||(p->vkCode==VK_F10)||(p->vkCode==VK_F11)||(p->vkCode==VK_F12)
		)   // 屏蔽Alt+Tab)
	{
			return true;
	} 
	return false;
}
/////////////////////////////////////////////////////////////////////////
// 开启bl为true ，关闭bl为false
bool set_Tab(bool bl)
{
	if (bl)
	{
		MY_VK_TAB = ZEROKEY;
 
	} 
	else
	{
		MY_VK_TAB = VK_TAB;
	}
	return true;
}
/////////////////////////////////////////////////////////////////////////
// 开启bl为true ，关闭bl为false
bool set_Esc(bool bl)
{
	if (bl)
	{
		MY_VK_ESCAPE = ZEROKEY;
 
	} 
	else
	{
		MY_VK_ESCAPE = VK_ESCAPE;
	}
	return true;
}


