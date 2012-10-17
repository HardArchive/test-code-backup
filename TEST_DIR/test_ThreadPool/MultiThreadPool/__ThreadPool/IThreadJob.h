#pragma once
#include "MultiThreadPool.h"
#include <Urlmon.h>
#pragma comment(lib, "Urlmon.lib")

struct tagMisson
{
	char chURL[260];        //下载网址
	char chFileName[260];   //保存文件名
};

//工作线程类
class IThreadJob : public ThreadJob 
{
public:
	virtual void DoJob(void *p)//自定义的虚函数
	{
		tagMisson *pMi = (tagMisson*)p;
		HRESULT hrRet = URLDownloadToFile(0, pMi->chURL, pMi->chFileName, 0, NULL);
		if (hrRet == S_OK)
		{
			printf("下载完成\n");
		}else{
			printf("下载失败\n");
		}
	}
};