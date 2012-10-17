// __ThreadPool.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include "IThreadJob.h"

int _tmain(int argc, _TCHAR* argv[])
{
	IThreadJob mt[10];
	ThreadPool tp;
	tagMisson  mi[10];
	for(int i = 0; i < 10; i++)
	{
		sprintf(mi[i].chURL,"http://www.jxwmw.cn/pic/0/10/09/14/10091483_818430.jpg");
		sprintf(mi[i].chFileName,"%d.jpg",i);
		tp.Call(&mt[i],&mi[i]);
	}
	getchar();
	return 0;
}

