#include "stdafx.h"
#include "ce_crtdbg.h"

#if defined _DEBUG && defined _CE_DETECT_MEMORY_

#define _MAX_MEMORY_DATA_SHOW_	256

__CrtMem*			__GarbageCollector::_CrtMemRoot			= 0;
__CrtFileName*		__GarbageCollector::_CrtFileNameRoot	= 0;
CCriSec				__GarbageCollector::_cs;
__GarbageCollector	__GarbageCollector::_gc;

int __GarbageCollector::exception_handle(unsigned int code, struct _EXCEPTION_POINTERS *ep)
{
	::NKDbgPrintfW(	_T("%s(%i) : Fatal : catch a run time exception at <0x%08X> (Exception Code: 0x%08X) !\n"), 
					_T(__FILE__), __LINE__, 
					ep->ExceptionRecord->ExceptionAddress, code);

	return EXCEPTION_EXECUTE_HANDLER;
}

void* __GarbageCollector::_impl_new(unsigned int s, wchar_t* name, int line)
{
	void * retPtr = 0;

	__try
	{
		retPtr = malloc(s);

		// set a conditional break point here (like: retPtr == 0x001D39A0)
		// can get more memory information, such as call stack etc.

		//CCriSecLock lock(_cs);
		_cs.Lock();

		if(retPtr)
		{
			__CrtMem*	_crtMemCell		= (struct __CrtMem*)malloc(sizeof(__CrtMem));
			_crtMemCell->_CrtLine		= line;
			_crtMemCell->_CrtMemLen		= s;
			_crtMemCell->_CrtMemAddr	= retPtr;
			_crtMemCell->_CrtNext		= 0;

			__CrtFileName* _tmpCrtFileName;

			if(name)
			{
				for(
						_tmpCrtFileName = _CrtFileNameRoot; 
						_tmpCrtFileName && wcscmp(name, _tmpCrtFileName->_CrtName);
						_tmpCrtFileName = _tmpCrtFileName->_CrtNext
					)
					;

				if(!_tmpCrtFileName)
				{
					wchar_t* _crtName = (wchar_t*)malloc((wcslen(name) + 1) * sizeof(wchar_t));
					wcscpy(_crtName, name);

					__CrtFileName* _crtFileName = (struct __CrtFileName*)malloc(sizeof (__CrtFileName));
					_crtFileName->_CrtName = _crtName;
					_crtFileName->_CrtNext = 0;

					if(!_CrtFileNameRoot)
						_CrtFileNameRoot = _crtFileName;
					else
					{
						for(
								_tmpCrtFileName = _CrtFileNameRoot;
								_tmpCrtFileName->_CrtNext;
								_tmpCrtFileName = _tmpCrtFileName->_CrtNext
							)
							;
						
						_tmpCrtFileName->_CrtNext = _crtFileName;
					}

					_tmpCrtFileName = _crtFileName;
				}
			}
			else
				_tmpCrtFileName		= 0;

			_crtMemCell->_CrtFileName = _tmpCrtFileName;

			if(!_CrtMemRoot)
			{
				_CrtMemRoot = _crtMemCell;
			}
			else
			{
				__CrtMem* _tmpMemPtr;
				for(_tmpMemPtr = _CrtMemRoot; _tmpMemPtr->_CrtNext; _tmpMemPtr = _tmpMemPtr->_CrtNext)
					;
				
				_tmpMemPtr->_CrtNext = _crtMemCell;
			}
		}

		_cs.Unlock();
	}
	__except(exception_handle(GetExceptionCode(), GetExceptionInformation()))
	{
		ASSERT(FALSE);
	}

	return retPtr;
}

void __GarbageCollector::_impl_delete(void *pvMem)
{
	__try
	{
		//CCriSecLock lock(_cs);
		_cs.Lock();

		if(pvMem)
		{
			if(_CrtMemRoot)
			{
				__CrtMem* _tmpMem;

				if(pvMem == _CrtMemRoot->_CrtMemAddr)
				{
					_tmpMem			= _CrtMemRoot;
					_CrtMemRoot		= _CrtMemRoot->_CrtNext;
					free(_tmpMem);
				}
				else
				{
					for(
							_tmpMem = _CrtMemRoot;
							_tmpMem->_CrtNext && (_tmpMem->_CrtNext->_CrtMemAddr != pvMem);
							_tmpMem = _tmpMem->_CrtNext
						)
						;

					if(_tmpMem->_CrtNext)
					{
						__CrtMem*			_tmpMem2;
						_tmpMem2			= _tmpMem->_CrtNext;
						_tmpMem->_CrtNext	= _tmpMem2->_CrtNext;

						free(_tmpMem2);
					}
					else
						::NKDbgPrintfW(_T("%s(%i) : Warning : deletes memory pointer not allocated with new!\n"), _T(__FILE__), __LINE__);
				}
			}
			else
				::NKDbgPrintfW(_T("%s(%i) : Warning : deletes memory pointer not allocated with new!\n"), _T(__FILE__), __LINE__);

			free (pvMem);
		}

		_cs.Unlock();
	}
	__except(exception_handle(GetExceptionCode(), GetExceptionInformation()))
	{
		ASSERT(FALSE);
	}
}

__GarbageCollector::~__GarbageCollector()
{
	bool hadConfirm = false;

	__try
	{
		::NKDbgPrintfW(_T("------------------------------ begin memory detect ------------------------------\n"));

		if(_CrtMemRoot)
		{
			__CrtMem* _tmpMem;

			for(_tmpMem = _CrtMemRoot; _tmpMem; _tmpMem = _tmpMem->_CrtNext)
			{
				if(_tmpMem->_CrtFileName)
				{
					::NKDbgPrintfW(
									_T("%s(%i) : normal block at 0x%08X, %i bytes long\n Data < "),
									_tmpMem->_CrtFileName->_CrtName,
									_tmpMem->_CrtLine,
									_tmpMem->_CrtMemAddr,
									_tmpMem->_CrtMemLen
								);

					if(!hadConfirm)
						hadConfirm = true;
				}
				else
				{
	#ifndef _ONLY_DETECT_CONFIRMED_MEMORY_LEAK_
					::NKDbgPrintfW(
										_T("[ Not Confirmed memory leak ] : normal block at 0x%08X, %i bytes long\n Data < "),
										_tmpMem->_CrtMemAddr,
										_tmpMem->_CrtMemLen
									);

					if(!hadConfirm)
						hadConfirm = true;
	#else
					continue;
	#endif
				}

				for(unsigned int i = 0; i < min(_tmpMem->_CrtMemLen, _MAX_MEMORY_DATA_SHOW_); i++)
					::NKDbgPrintfW(_T("%02X "), *(((unsigned char*)_tmpMem->_CrtMemAddr) + i));

				if(_tmpMem->_CrtMemLen > _MAX_MEMORY_DATA_SHOW_)
					::NKDbgPrintfW(_T("... ... "));

				::NKDbgPrintfW(_T(">\n"));
			}
		}

		if(hadConfirm)
			::NKDbgPrintfW(_T("<<-- Detected memory leaks -->>\n"));
		else
			::NKDbgPrintfW(_T("<<-- No memory leaks detected -->>\n"));

		::NKDbgPrintfW(_T("------------------------------- end memory detect -------------------------------\n"));

		__CrtFileName*	_tmpName = _CrtFileNameRoot;

		for(;_tmpName;)
		{
			_CrtFileNameRoot	= _tmpName->_CrtNext;

			free(_tmpName->_CrtName);
			free(_tmpName);

			_tmpName			= _CrtFileNameRoot;
		}
	}
	__except(exception_handle(GetExceptionCode(), GetExceptionInformation()))
	{
		ASSERT(FALSE);
	}
}

#endif	// defined _DEBUG && defined _CE_DETECT_MEMORY_