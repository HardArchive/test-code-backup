// test.cpp : �������̨Ӧ�ó������ڵ㡣
//

#include "stdafx.h"

//#include "SHDocVw_OCX.h"
#include <comutil.h>
#import "c:\program files\common files\system\ado\msado15.dll" no_namespace raw_interfaces_only
#import "C:\WINDOWS\system32\cdosys.dll" no_namespace raw_interfaces_only

//#include "cdoex.h"
//#include "msado15.h"
//
//using namespace CDO;
// ������ccrun(����)����������ϸĵ�,��BCB 6.0 �³ɹ����벢����ͨ��.
// ����в����������е�,������Ҫ�ڻ����ϰ�װ��outlook.
void __fastcall SaveWholePage(BSTR bstrUrl, BSTR bstrFileName)
{
	CoInitialize(NULL);
	IMessage *pMsg;
	IConfiguration *pConfig;
	HRESULT hr = CoCreateInstance(__uuidof(Message), NULL,
		CLSCTX_INPROC_SERVER, __uuidof(IMessage), (void**)&pMsg);
	hr = CoCreateInstance(
		__uuidof(Configuration),
		NULL,
		CLSCTX_INPROC_SERVER,
		__uuidof(IConfiguration),
		(void**)&pConfig);
	pMsg->put_Configuration(pConfig);
	try
	{
		pMsg->CreateMHTMLBody(
			bstrUrl,
			cdoSuppressNone,
			_bstr_t(""),
			_bstr_t(""));
	}
	catch(...)
	{
		//ShowMessage("�д�����!");
		return;
	}


	_Stream*   pStm    = NULL;
	pMsg->GetStream(&pStm);
	pStm->SaveToFile(bstrFileName,
		adSaveCreateOverWrite);

	pMsg->Release();
	pStm->Release();
	CoUninitialize();
}


/********************************************���Ƿ���1******************************************************************
//#import  no_namespace raw_interfaces_only
#import "c:\program files\common files\system\ado\msado15.dll" no_namespace raw_interfaces_only
#import "C:\WINDOWS\system32\cdosys.dll" no_namespace raw_interfaces_only
#include "C:\Program Files\Microsoft Visual Studio .NET 2003\Vc7\PlatformSDK\Include\CDOSysStr.h"
#include "C:\Program Files\Microsoft Visual Studio .NET 2003\Vc7\PlatformSDK\Include\CDOSysErr.h"
//#include <code.h>
//#include <msado15.h>
//using namespace CDO;
//#include <cdosysstr.h>
//#include <cdosyserr.h>

#include <string>
using namespace std;

int SaveWholePage(string szPageURL, string szFileName)
{
	CoInitialize(NULL);
	BSTR bstr = _com_util::ConvertStringToBSTR(szPageURL.c_str());
	string szUserName = "domain\\username";
	BSTR bstrUserName = _com_util::ConvertStringToBSTR(szUserName.c_str());
	string szPass = "domain\\username";
	BSTR bstrPass = _com_util::ConvertStringToBSTR(szPass.c_str());

	IMessage *pMsg=NULL;
	IConfiguration* pConfig = NULL;
	_Stream*        pStm    = NULL;

	HRESULT hr=CoCreateInstance(
		__uuidof(Message),
		NULL,
		CLSCTX_INPROC_SERVER,
		__uuidof(IMessage),
		(void**)&pMsg);

	hr=CoCreateInstance(
		__uuidof(Configuration),
		NULL,
		CLSCTX_INPROC_SERVER,
		__uuidof(IConfiguration),
		(void**)&pConfig);

	//pMsg->Configuration = pConfig;
	pMsg->put_Configuration (pConfig);
	try
	{
		pMsg->CreateMHTMLBody(
			bstr, 
			cdoSuppressNone,
			bstrUserName,
			bstrPass );
	}
	catch(_com_error err)
	{
		// handle exception
		//AfxMessageBox("Exception");
		MessageBox(NULL, "Exception", "", MB_OK);
		return 0;
	}   

	_StreamPtr pStream;

	pMsg->GetStream(&pStm);

	pStm->SaveToFile(_com_util::ConvertStringToBSTR(szFileName.c_str()),
		adSaveCreateOverWrite);

	pMsg->Release();
	pStm->Release();
	CoUninitialize();

	return 1;
}

********************************************���Ƿ���1****************************************************************/
int _tmain(int argc, _TCHAR* argv[])
{

	/***************���Ƿ���1*****************
	if (1 == SaveWholePage("http://www.baidu.com", "D:\\WinPath\\desktop\\9cpp.mht"))
	{
		MessageBox(NULL, "Download Success!!!!!", "TIPS", MB_OK);
	}
	else
	{
		MessageBox(NULL, "Download Fail!!!!!", "TIPS", MB_OK);
	}

	*******************************************/

	//���Ƿ���2
	// ���þ���:
	SaveWholePage(_bstr_t("http://www.ccrun.com/article.asp?i=615&d=l77p38"),
		_bstr_t("D:\\WinPath\\desktop\\9cpp.mht"));
	getchar();
	return 0;
}

