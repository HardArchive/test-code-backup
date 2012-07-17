#include "StdAfx.h"
#include "KStreamWrapper.h"

//////////////////////////////////////////////////////////////////////////
// CFileOutStream
CFileOutStream::CFileOutStream(IN LPCWSTR lpszFile, IN BOOL bAlwaysCreate/* = TRUE*/)
	:hFile_(NULL)
{
	hFile_ = ::CreateFileW(lpszFile, 
						   GENERIC_WRITE, 
						   NULL, 
						   NULL, 
						   bAlwaysCreate ? CREATE_ALWAYS : OPEN_ALWAYS, 
						   FILE_ATTRIBUTE_NORMAL, 
						   NULL);
}

CFileOutStream::~CFileOutStream()
{
	if (hFile_ && INVALID_HANDLE_VALUE != hFile_)
	{
		::CloseHandle(hFile_);
		hFile_ = NULL;
	}
}

/*
@ FuncName	: Write
@ Brief		: 输出接口。
*/
HRESULT CFileOutStream::Write(IN c_byte * pbyBuf, IN u_int uSize, OUT u_int * pWrittenSize)
{
	HRESULT hResult = E_FAIL;

	if (hFile_ && INVALID_HANDLE_VALUE != hFile_)
	{
		DWORD dwWrittenSize = 0;
		::WriteFile(hFile_, pbyBuf, uSize, &dwWrittenSize, NULL);

		if (pWrittenSize)
			*pWrittenSize = dwWrittenSize;
		
		if (dwWrittenSize == uSize)
			hResult = S_OK;
		else
			hResult = HRESULT_FROM_WIN32(::GetLastError());
	}

	return hResult;
}

/*
@ FuncName	: Seek
@ Brief		: Seek接口，FILE_BEGIN、FILE_CURRENT、FILE_END 
*/
HRESULT CFileOutStream::Seek(IN u_long ulMove, IN u_int uMoveMethod)
{
	HRESULT hResult = E_FAIL;

	if (hFile_ && INVALID_HANDLE_VALUE != hFile_)
	{
		if (INVALID_SET_FILE_POINTER != ::SetFilePointer(hFile_, ulMove, NULL, uMoveMethod))
			hResult = S_OK;
		else
			hResult = HRESULT_FROM_WIN32(::GetLastError());
	}

	return hResult;
}

/*
@ FuncName	: GetSize
@ Brief		: GetSize接口，获取流的大小。 
*/
HRESULT CFileOutStream::GetSize(OUT u_int& uStreamSize)
{
	HRESULT hResult = E_FAIL;

	if (hFile_ && INVALID_HANDLE_VALUE != hFile_)
	{
		uStreamSize = ::GetFileSize(hFile_, NULL);
		hResult = S_OK;
	}

	return hResult;
}

//////////////////////////////////////////////////////////////////////////
// CMemOutStream
CMemOutStream::CMemOutStream(std::string& _byBuffer)
	:byBuffer_(_byBuffer)
{
}

CMemOutStream::~CMemOutStream()
{
}

/*
@ FuncName	: Write
@ Brief		: 输出接口。
*/
HRESULT CMemOutStream::Write(IN c_byte * pbyBuf, IN u_int uSize, OUT u_int * pWrittenSize)
{
	u_int uWrittenSize = 0;

	if (pbyBuf && uSize > 0)
	{
		this->byBuffer_.append((char*)pbyBuf, uSize);
		
		uWrittenSize = uSize;
	}

	if (pWrittenSize)
		*pWrittenSize = uWrittenSize;
	
	return S_OK;
}

/*
@ FuncName	: Seek
@ Brief		: Seek接口，FILE_BEGIN、FILE_CURRENT、FILE_END 
*/
HRESULT CMemOutStream::Seek(IN u_long ulMove, IN u_int uMoveMethod)
{
	return E_NOTIMPL;
}

/*
@ FuncName	: GetSize
@ Brief		: GetSize接口，获取流的大小。 
*/
HRESULT CMemOutStream::GetSize(OUT u_int& uStreamSize)
{
	uStreamSize = (u_int)this->byBuffer_.size();

	return S_OK;
}
//////////////////////////////////////////////////////////////////////////
// CFileInStream
CFileInStream::CFileInStream(IN LPCWSTR lpszFile, IN BOOL bAlwaysCreate/* = TRUE*/)
	: hFile_(NULL)
{
	hFile_ = ::CreateFileW(lpszFile, 
						   GENERIC_READ, 
						   NULL, 
						   NULL, 
						   bAlwaysCreate ? CREATE_ALWAYS : OPEN_ALWAYS, 
						   FILE_ATTRIBUTE_NORMAL, 
						   NULL);
}

CFileInStream::~CFileInStream()
{
	if (hFile_ && INVALID_HANDLE_VALUE != hFile_)
	{
		::CloseHandle(hFile_);
		hFile_ = NULL;
	}
}

/*
@ FuncName	: Read
@ Brief		: 输入接口。
*/
HRESULT CFileInStream::Read(IN byte * pbyBuf, IN u_int uSize, OUT u_int * pReadedSize)
{
	HRESULT hResult = E_FAIL;

	if (hFile_ && INVALID_HANDLE_VALUE != hFile_)
	{
		DWORD dwReadedSize = 0;
		::ReadFile(hFile_, pbyBuf, uSize, &dwReadedSize, NULL);

		if (pReadedSize)
			*pReadedSize = dwReadedSize;

		if (dwReadedSize == uSize)
			hResult = S_OK;
		else
			hResult = HRESULT_FROM_WIN32(::GetLastError());
	}

	return hResult;
}

/*
@ FuncName	: Seek
@ Brief		: Seek接口，FILE_BEGIN、FILE_CURRENT、FILE_END 
*/
HRESULT CFileInStream::Seek(IN u_long ulMove, IN u_int uMoveMethod)
{
	HRESULT hResult = E_FAIL;

	if (hFile_ && INVALID_HANDLE_VALUE != hFile_)
	{
		if (INVALID_SET_FILE_POINTER != ::SetFilePointer(hFile_, ulMove, NULL, uMoveMethod))
			hResult = S_OK;
		else
			hResult = HRESULT_FROM_WIN32(::GetLastError());
	}

	return hResult;
}

/*
@ FuncName	: GetSize
@ Brief		: GetSize接口，获取流的大小。 
*/
HRESULT CFileInStream::GetSize(OUT u_int& uStreamSize)
{
	HRESULT hResult = E_FAIL;

	if (hFile_ && INVALID_HANDLE_VALUE != hFile_)
	{
		uStreamSize = ::GetFileSize(hFile_, NULL);
		hResult = S_OK;
	}

	return hResult;
}