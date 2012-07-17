#pragma once
#include "WinHttpApi.h"
#include "iseqstream.h"

//回调：下载状态的枚举
enum enumKHttpStatus
{
	enumHttpStatus_Progress			= 1, //下载进度
	enumHttpStatus_ErrorCode		= 2, //下载过程中遇到的错误，一般为GetLastError的值
	enumHttpStatus_StatusCode		= 3, //下载过程中遇到的非正常状况，如网络无法连接等
	enumHttpStatus_CurDownSize		= 4, //当前下载的大小。当前下载的总大小 - 上次下载的总大小的值
	enumHttpStatus_DownSpeed		= 5, //下载速度
	enumHttpStatus_DownedSize		= 6, //已经下载的总的大小
	enumHttpStatus_ExitDown			= 7, //退出下载
};
//回调函数类
class IStatusCallback
{
public:
	/*
	* @funcn							--- OnStatusCallback
	* @brief							--- 下载过程之中的回调函数
	* @param uStatus					--- 下载过程中说遇到的状态
	* @param nValue						--- 对应于uStatus状态的值
	* @return							--- 参看enumBackValue中的枚举
	*/
	virtual DWORD OnStatusCallback(enumKHttpStatus uStatus, int nValue) = 0;
};

class WinHttpWrapper
{
public:
	WinHttpWrapper(void);
	~WinHttpWrapper(void);
public:
	BOOL Initialize();
	BOOL Uninitialize();
public:
	long SendHttpRequest(LPCWSTR lpszUrl,
						 IKOutStream * _pISeqOutStream,
						 IStatusCallback * _pIStatusCallback = NULL);

	long SendHttpRequestEx(LPCWSTR lpszUrl,
							DWORD dwFileSize,
							IKOutStream * _pISeqOutStream,
							IStatusCallback * _pIStatusCallback = NULL,
							int nDownSpeed = 1);

	long GetHttpFileSize(LPCWSTR lpszUrl,
						 DWORD& dwFileSize);

private:
	long _SendHttpRequest(LPCWSTR lpszUrl,
							DWORD dwFileSize,
							int nDownSpeed,
							IKOutStream * _pIOutStream,
							IStatusCallback * _pIStatusCallback = NULL);

protected:
	WinHttpApi WinHttp_;
};

//////////////////////////////////////////////////////////////////////////
// WinHttpHandle
class WinHttpHandle
{
public:
	WinHttpHandle(IN WinHttpApi& _WinHttp);
	~WinHttpHandle();
public:
	BOOL Attach(HINTERNET handle);
	
	HINTERNET Detach();
	
	void Close();

	HRESULT SetOption(DWORD option, const void* value, DWORD length);

	HRESULT QueryOption(DWORD option, void* value, DWORD& length) const;
protected:
	HINTERNET m_handle;
protected:
	WinHttpApi& WinHttp_;
};

//////////////////////////////////////////////////////////////////////////
// WinHttpSession
class WinHttpSession 
	: public WinHttpHandle
{
public:
	WinHttpSession(IN WinHttpApi& _WinHttp);
public:
	HRESULT Initialize();
};
