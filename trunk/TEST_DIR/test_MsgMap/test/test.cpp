// test.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include <Windows.h>

//
//int _tmain(int argc, _TCHAR* argv[])
//{
//	return 0;
//}

//msg
#define OWN_MSG_1 0x1
#define OWN_MSG_2 0x2
#define OWN_MSG_3 0x3
//cmd
#define OWN_CMD_1 0x4
#define OWN_CMD_2 0x5
#define OWN_CMD_3 0x6

class COwnMessageMap//
{
public:
	virtual int DefaultWinProc(unsigned int uMsg, char* wParam, char* lParam)=0;//关键的纯虚函数

	void _Run()
	{
		int i=0;
		char str[3]={0};
		while(1)//模拟GetMsg
		{
			_sleep(15);
			sprintf(str,"%d",i);
			DefaultWinProc(i,str,str);
			i++;
			if(i>10)
				break;
		}
	}
}; 


#define BEGIN_OWN_MAP(theClass)                                            \
	int DefaultWinProc(unsigned int uMsg, char* wParam, char* lParam)      \
{                                                                          \

#define MSG_HANDLER(msg, function)                                         \
	if(uMsg==msg)                                                          \
{                                                                          \
	return function(wParam, lParam);                                       \
}                                                                          \

#define CMD_HANDLER(msg, function)                                         \
	if(uMsg==msg)                                                          \
{                                                                          \
	return function(uMsg, wParam, lParam);                                 \
}                                                                          \

#define END_OWN_MAP()                                                      \
	return 1;                                                              \
}                                                                          \

class COwnWnd:public COwnMessageMap
{
	BEGIN_OWN_MAP(COwnWnd)
		MSG_HANDLER(OWN_MSG_1,OnMsg1)
		MSG_HANDLER(OWN_MSG_3,OnMsg3)
		CMD_HANDLER(OWN_CMD_1,OnCmd1)
		CMD_HANDLER(OWN_CMD_2,OnCmd)
		CMD_HANDLER(OWN_CMD_3,OnCmd)
	END_OWN_MAP()

	int OnMsg1(char* wp,char* lp)
	{
		printf("Msg\t");
		printf(wp);
		printf("\n");
		return 1;
	}
	int OnMsg3(char* wp,char* lp)
	{
		printf("MSG\t");
		printf(wp);
		printf("\n");
		return 1;
	}
	int OnCmd1(unsigned int id,char* wp,char* lp)
	{
		printf("command id:%x\t",id);
		printf("CMD\t");
		printf(wp);
		printf("\n");
		return 1;
	}
	int OnCmd(unsigned int id,char* wp,char* lp)
	{
		printf("command id:%d\t",id);
		printf("CMD\t");
		printf(wp);
		printf("\n");
		return 1;
	}
};
int _tmain(int argc, _TCHAR* argv[])
{
	COwnWnd Wnd;
	Wnd._Run();
	getchar();
	return 0;
}