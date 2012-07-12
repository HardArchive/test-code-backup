// test.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"


#include <Shlwapi.h>
#pragma comment(lib, "Shlwapi.lib")  


void DeleteDir(char* pDirPath)
{
	//可以用的
	//SHFILEOPSTRUCT FileOp;
	//ZeroMemory((void*)&FileOp,sizeof(SHFILEOPSTRUCT));

	////如果加了FOF_SILENT标志，那么在函数调用的时候，即使出错，也不会弹框提示，，如果这个目录是共享的，依然还是会弹框提示
	//FileOp.fFlags = FOF_SILENT|FOF_NOCONFIRMATION;
	//FileOp.hNameMappings = NULL;
	//FileOp.hwnd = NULL;
	//FileOp.lpszProgressTitle = NULL;
	//FileOp.pFrom = pDirPath;
	//FileOp.pTo = NULL;
	//FileOp.wFunc = FO_DELETE;
	//SHFileOperation(&FileOp);
	// 
	 
	char str[MAX_PATH] ="RMDIR /S/Q "; //加上 /Q  删除时不要求确认
	strcat(str, pDirPath);
	system(str);

}
int _tmain(int argc, _TCHAR* argv[])
{
	DeleteDir("D:\\WinPath\\desktop\\新建文件夹");
	return 0;
}

