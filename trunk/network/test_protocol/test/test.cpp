// test.cpp : �������̨Ӧ�ó������ڵ㡣
//

#include "stdafx.h"
#include <Windows.h>
#include "ConfigXML.h"

int _tmain(int argc, _TCHAR* argv[])
{
	CConfigXML clsConfigXML;
	PSERVICESFINDINFO pstuServicesFindInfo = new SERVICESFINDINFO;
	pstuServicesFindInfo->Reset();	

	clsConfigXML.GetServicesFindInfo(pstuServicesFindInfo);
	

	pstuServicesFindInfo->Reset();
	delete pstuServicesFindInfo;
	pstuServicesFindInfo = NULL;
	return 0;
}

