//kdownloadfile.h

#pragma once
#ifndef __UPLIVE_KREQUEST_HTTP_FILE_H__
#define __UPLIVE_KREQUEST_HTTP_FILE_H__
#include "../../uplive/publish/common/common.h"
#include "WinHttpWrapper.h"


BOOL WinHttp_Initialize();

BOOL WinHttp_Uninitialize();

BOOL WinHttp_RequestHttpFile(IN LPCWSTR lpszUrl);

BOOL WinHttp_RequestHttpFile(IN LPCWSTR lpszUrl, 
							 IN LPCWSTR lpszFile, 
							 IN IStatusCallback * _pIStatusCallback = NULL,
							 IN BOOL bIsNeedContinue = FALSE,//�Ƿ���Ҫ���öϵ㣬Ĭ��Ϊ�����öϵ㡣
							 IN int nDownSpeed = 1); //������1Ϊû���ޣ�2Ϊ����1�����ң�

BOOL WinHttp_GetHttpFileSize(IN LPCWSTR lpszUrl, IN OUT DWORD& dwFileSize);

#endif //__UPLIVE_KREQUEST_HTTP_FILE_H__