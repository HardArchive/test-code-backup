// test.cpp : �������̨Ӧ�ó������ڵ㡣
//
//
//#include "stdafx.h"
//
//
//int _tmain(int argc, _TCHAR* argv[])
//{
//	return 0;
//}

// ͷ�ļ�����
#include "stdafx.h"
#include "SecurityTool.h"
#include "GetHostIP.h"


void outputuser();
// �������
int _tmain(int argc, _TCHAR* argv[])
{
	outputuser();

	printf("\n����1#####\r\n");
	GetHostIP_01();
	printf("\n����2#####\r\n");
	GetHostIP_02();
	printf("\n����3#####\r\n");
	GetHostIP_03();

	getchar();
	return 0;
}



void outputuser()
{
	CString strUser = "";
	CSecurityTool::GetCurrProcessUser(strUser);
	//cout<<"��ǰ�����û���   "<<strUser.<<endl;
	CSecurityTool::GetLogUserXP(strUser);
	//cout<<"XP��½�û���     "<<strUser<<endl;
	CSecurityTool::GetLogUser2K(strUser);
	//cout<<"win2000��½�û���"<<strUser<<endl;
}