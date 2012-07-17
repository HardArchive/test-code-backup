// KStreamWrapper.h
// �������Ĳ�����װ��

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
	@ Brief		: ����ӿڡ�
	*/
	virtual HRESULT Write(IN c_byte * pbyBuf, IN u_int uSize, OUT u_int * pWrittenSize);
	/*
	@ FuncName	: Seek
	@ Brief		: Seek�ӿڣ�FILE_BEGIN��FILE_CURRENT��FILE_END 
	*/
	virtual HRESULT Seek(IN u_long ulMove, IN u_int uMoveMethod);
	/*
	@ FuncName	: GetSize
	@ Brief		: GetSize�ӿڣ���ȡ���Ĵ�С�� 
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
	@ Brief		: ����ӿڡ�
	*/
	virtual HRESULT Write(IN c_byte * pbyBuf, IN u_int uSize, OUT u_int * pWrittenSize);
	/*
	@ FuncName	: Seek
	@ Brief		: Seek�ӿڣ�FILE_BEGIN��FILE_CURRENT��FILE_END 
	*/
	virtual HRESULT Seek(IN u_long ulMove, IN u_int uMoveMethod);
	/*
	@ FuncName	: GetSize
	@ Brief		: GetSize�ӿڣ���ȡ���Ĵ�С�� 
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
	@ Brief		: ����ӿڡ�
	*/
	virtual HRESULT Read(IN byte * pbyBuf, IN u_int uSize, OUT u_int * pReadedSize);
	/*
	@ FuncName	: Seek
	@ Brief		: Seek�ӿڣ�FILE_BEGIN��FILE_CURRENT��FILE_END 
	*/
	virtual HRESULT Seek(IN u_long ulMove, IN u_int uMoveMethod);
	/*
	@ FuncName	: GetSize
	@ Brief		: GetSize�ӿڣ���ȡ���Ĵ�С�� 
	*/
	virtual HRESULT GetSize(OUT u_int& uStreamSize);
protected:
	HANDLE hFile_;
};

#endif //__UPLIVE_KSTREAM_WRAPPER_H__