// test.cpp : �������̨Ӧ�ó������ڵ㡣
//

#include "stdafx.h"


#include <Shlwapi.h>
#pragma comment(lib, "Shlwapi.lib")  


void DeleteDir(char* pDirPath)
{
	//�����õ�
	//SHFILEOPSTRUCT FileOp;
	//ZeroMemory((void*)&FileOp,sizeof(SHFILEOPSTRUCT));

	////�������FOF_SILENT��־����ô�ں������õ�ʱ�򣬼�ʹ����Ҳ���ᵯ����ʾ����������Ŀ¼�ǹ���ģ���Ȼ���ǻᵯ����ʾ
	//FileOp.fFlags = FOF_SILENT|FOF_NOCONFIRMATION;
	//FileOp.hNameMappings = NULL;
	//FileOp.hwnd = NULL;
	//FileOp.lpszProgressTitle = NULL;
	//FileOp.pFrom = pDirPath;
	//FileOp.pTo = NULL;
	//FileOp.wFunc = FO_DELETE;
	//SHFileOperation(&FileOp);
	// 
	 
	char str[MAX_PATH] ="RMDIR /S/Q "; //���� /Q  ɾ��ʱ��Ҫ��ȷ��
	strcat(str, pDirPath);
	system(str);

}
int _tmain(int argc, _TCHAR* argv[])
{
	DeleteDir("D:\\WinPath\\desktop\\�½��ļ���");
	return 0;
}

