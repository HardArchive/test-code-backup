// RGWinService.cpp : �������̨Ӧ�ó������ڵ㡣
//

#include "stdafx.h"


int _tmain(int argc, _TCHAR* argv[])
{
	//SetPriorityClass �в������ֱ��Ӧ 5 �ֽ�������Ȩ
	//	NORMAL_PRIORITY_CLASS    //��׼
	//	IDLE_PRIORITY_CLASS  //����(��)
	//	HIGH_PRIORITY_CLASS  //��
	//	REALTIME_PRIORITY_CLASS  //ʵʱ
	//	BELOW_NORMAL_PRIORITY_CLASS  //���ڱ�׼
	//	ABOVE_NORMAL_PRIORITY_CLASS  //���ڱ�׼
	SetPriorityClass(GetCurrentProcess(), REALTIME_PRIORITY_CLASS);


	Sleep(1000);
	return 0;
}

