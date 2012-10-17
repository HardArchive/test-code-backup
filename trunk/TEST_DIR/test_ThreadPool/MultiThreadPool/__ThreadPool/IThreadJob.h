#pragma once
#include "MultiThreadPool.h"
#include <Urlmon.h>
#pragma comment(lib, "Urlmon.lib")

struct tagMisson
{
	char chURL[260];        //������ַ
	char chFileName[260];   //�����ļ���
};

//�����߳���
class IThreadJob : public ThreadJob 
{
public:
	virtual void DoJob(void *p)//�Զ�����麯��
	{
		tagMisson *pMi = (tagMisson*)p;
		HRESULT hrRet = URLDownloadToFile(0, pMi->chURL, pMi->chFileName, 0, NULL);
		if (hrRet == S_OK)
		{
			printf("�������\n");
		}else{
			printf("����ʧ��\n");
		}
	}
};