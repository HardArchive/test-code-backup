#pragma once
#include "WinHttpApi.h"
#include "iseqstream.h"

//�ص�������״̬��ö��
enum enumKHttpStatus
{
	enumHttpStatus_Progress			= 1, //���ؽ���
	enumHttpStatus_ErrorCode		= 2, //���ع����������Ĵ���һ��ΪGetLastError��ֵ
	enumHttpStatus_StatusCode		= 3, //���ع����������ķ�����״�����������޷����ӵ�
	enumHttpStatus_CurDownSize		= 4, //��ǰ���صĴ�С����ǰ���ص��ܴ�С - �ϴ����ص��ܴ�С��ֵ
	enumHttpStatus_DownSpeed		= 5, //�����ٶ�
	enumHttpStatus_DownedSize		= 6, //�Ѿ����ص��ܵĴ�С
	enumHttpStatus_ExitDown			= 7, //�˳�����
};
//�ص�������
class IStatusCallback
{
public:
	/*
	* @funcn							--- OnStatusCallback
	* @brief							--- ���ع���֮�еĻص�����
	* @param uStatus					--- ���ع�����˵������״̬
	* @param nValue						--- ��Ӧ��uStatus״̬��ֵ
	* @return							--- �ο�enumBackValue�е�ö��
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
