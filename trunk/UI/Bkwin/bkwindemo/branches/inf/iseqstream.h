
#pragma once

#ifndef __UPLIVE_ISEQSTREAM_H__
#define __UPLIVE_ISEQSTREAM_H__
#include "../../uplive/publish/common/common.h"

// ����
interface IKStream
{
	virtual ~IKStream(){};
	
	//virtual u_int GetSize() = 0;
};

// д��Ľӿڡ�
interface IKWrite
{
	/* Returns: result - the number of actually written bytes.
	(result < uSize) means error */
	virtual HRESULT Write(IN c_byte * pbyBuf, IN u_int uSize, OUT u_int * pWrittenSize) = 0;
};

// ��ȡ�Ľӿڡ�
interface IKRead
{
	/* if (input(*size) != 0 && output(*size) == 0) means end_of_stream.
	(output(*size) < input(*size)) is allowed */
	virtual HRESULT Read(IN byte * pbyBuf, IN u_int uSize, OUT u_int * pReadedSize) = 0;
};

// �������
interface IKOutStream
	: public IKStream,
	  public IKWrite
{
	/*
	@ FuncName	: Seek
	@ Brief		: Seek�ӿڣ�FILE_BEGIN��FILE_CURRENT��FILE_END 
	*/
	virtual HRESULT Seek(IN u_long ulMove, IN u_int uMoveMethod) = 0;
	/*
	@ FuncName	: GetSize
	@ Brief		: GetSize�ӿڣ���ȡ���Ĵ�С�� 
	*/
	virtual HRESULT GetSize(OUT u_int& uStreamSize) = 0;
};

// ��������
interface IKInStream
	: public IKStream,
	  public IKRead
{
	/*
	@ FuncName	: Seek
	@ Brief		: Seek�ӿڣ�FILE_BEGIN��FILE_CURRENT��FILE_END 
	*/
	virtual HRESULT Seek(IN u_long ulMove, IN u_int uMoveMethod) = 0;
	/*
	@ FuncName	: GetSize
	@ Brief		: GetSize�ӿڣ���ȡ���Ĵ�С�� 
	*/
	virtual HRESULT GetSize(OUT u_int& uStreamSize) = 0;
};

interface ISeqStream
	: public IKOutStream, 
	  public IKInStream
{
	
};


#endif	//__UPLIVE_ISEQSTREAM_H__