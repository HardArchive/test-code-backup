// KStreamWrapper.h
// 各种流的操作包装。

#pragma once
#ifndef __UPLIVE_KSTREAM_WRAPPER_H__
#define __UPLIVE_KSTREAM_WRAPPER_H__
#include "iseqstream.h"

class CFileOutStream
	: public IKOutStream
{
public:
	CFileOutStream(IN LPCWSTR lpszFile, IN BOOL bAlwaysCreate = TRUE);
	~CFileOutStream();
public:
	/*
	@ FuncName	: Write
	@ Brief		: 输出接口。
	*/
	virtual HRESULT Write(IN c_byte * pbyBuf, IN u_int uSize, OUT u_int * pWrittenSize);
	/*
	@ FuncName	: Seek
	@ Brief		: Seek接口，FILE_BEGIN、FILE_CURRENT、FILE_END 
	*/
	virtual HRESULT Seek(IN u_long ulMove, IN u_int uMoveMethod);
	/*
	@ FuncName	: GetSize
	@ Brief		: GetSize接口，获取流的大小。 
	*/
	virtual HRESULT GetSize(OUT u_int& uStreamSize);
protected:
	HANDLE hFile_;
};

class CMemOutStream
	: public IKOutStream
{
public:
	CMemOutStream(std::string& _byBuffer);
	~CMemOutStream();
public:
	/*
	@ FuncName	: Write
	@ Brief		: 输出接口。
	*/
	virtual HRESULT Write(IN c_byte * pbyBuf, IN u_int uSize, OUT u_int * pWrittenSize);
	/*
	@ FuncName	: Seek
	@ Brief		: Seek接口，FILE_BEGIN、FILE_CURRENT、FILE_END 
	*/
	virtual HRESULT Seek(IN u_long ulMove, IN u_int uMoveMethod);
	/*
	@ FuncName	: GetSize
	@ Brief		: GetSize接口，获取流的大小。 
	*/
	virtual HRESULT GetSize(OUT u_int& uStreamSize);
protected:
	std::string& byBuffer_;
};

class CFileInStream
	: public IKInStream
{
public:
	CFileInStream(IN LPCWSTR lpszFile, IN BOOL bAlwaysCreate = TRUE);
	~CFileInStream();
public:
	/*
	@ FuncName	: Read
	@ Brief		: 输入接口。
	*/
	virtual HRESULT Read(IN byte * pbyBuf, IN u_int uSize, OUT u_int * pReadedSize);
	/*
	@ FuncName	: Seek
	@ Brief		: Seek接口，FILE_BEGIN、FILE_CURRENT、FILE_END 
	*/
	virtual HRESULT Seek(IN u_long ulMove, IN u_int uMoveMethod);
	/*
	@ FuncName	: GetSize
	@ Brief		: GetSize接口，获取流的大小。 
	*/
	virtual HRESULT GetSize(OUT u_int& uStreamSize);
protected:
	HANDLE hFile_;
};

#endif //__UPLIVE_KSTREAM_WRAPPER_H__