#pragma once

/*
#if !defined _CE_DETECT_MEMORY_
	#define _CE_DETECT_MEMORY_
#endif
*/

#if defined _DEBUG && defined _CE_DETECT_MEMORY_

#ifdef new
	#undef new
#endif

#include "../CriticalSection.h"

struct __CrtFileName
{
	wchar_t*		_CrtName;
	__CrtFileName*	_CrtNext;
};

struct __CrtMem
{
	__CrtFileName*	_CrtFileName;
	int				_CrtLine;
	unsigned int	_CrtMemLen;
	void*			_CrtMemAddr;
	__CrtMem*		_CrtNext;
};

class __GarbageCollector
{
public:
	static void* _impl_new(unsigned int s, wchar_t* name, int line);
	static void _impl_delete(void *pvMem);

//private:
	static int exception_handle(unsigned int code, struct _EXCEPTION_POINTERS *ep);

private:
	__GarbageCollector() {}
	~__GarbageCollector();

private:
	static __CrtMem*			_CrtMemRoot;
	static __CrtFileName*		_CrtFileNameRoot;

	static CCriSec				_cs;

	static __GarbageCollector	_gc;
};

/*
operator new(size_t, const char*, int)
operator new[](size_t, const char*, int)
operator new(size_t)
operator new[](size_t)
operator new(size_t, const std::nothrow_t&)
operator new[](size_t, const std::nothrow_t&)
operator delete(void*)
operator delete[](void*)
operator delete(void*, const char*, int)
operator delete[](void*, const char*, int)
operator delete(void*, const std::nothrow_t&)
operator delete[](void*, const std::nothrow_t&)
*/

inline void* __cdecl operator new(unsigned int s)
{
	return __GarbageCollector::_impl_new(s, NULL, 0);
}

inline void* __cdecl operator new[](unsigned int s)
{
	return __GarbageCollector::_impl_new(s, NULL, 0);
}

inline void* __cdecl operator new(unsigned int s, wchar_t* name, int line)
{
	return __GarbageCollector::_impl_new(s, name, line);
}

inline void* __cdecl operator new[](unsigned int s, wchar_t* name, int line)
{
	return __GarbageCollector::_impl_new(s, name, line);
}

inline void* __cdecl operator new(unsigned int s, const std::nothrow_t&)
{
	return __GarbageCollector::_impl_new(s, NULL, 0);
}

inline void* __cdecl operator new[](unsigned int s, const std::nothrow_t&)
{
	return __GarbageCollector::_impl_new(s, NULL, 0);
}

inline void __cdecl operator delete(void *pvMem)
{
	__GarbageCollector::_impl_delete(pvMem);
}

inline void __cdecl operator delete[](void *pvMem)
{
	__GarbageCollector::_impl_delete(pvMem);
}

inline void __cdecl operator delete(void* pvMem, wchar_t* /*lpszFileName*/, int /*nLine*/ )
{
	__GarbageCollector::_impl_delete(pvMem);
}

inline void __cdecl operator delete[](void* pvMem, wchar_t* /*lpszFileName*/, int /*nLine*/ )
{
	__GarbageCollector::_impl_delete(pvMem);
}

inline void __cdecl operator delete(void* pvMem, char* /*lpszFileName*/, int /*nLine*/ )
{
	__GarbageCollector::_impl_delete(pvMem);
}

inline void __cdecl operator delete[](void* pvMem, char* /*lpszFileName*/, int /*nLine*/ )
{
	__GarbageCollector::_impl_delete(pvMem);
}

inline void __cdecl operator delete(void *pvMem, const std::nothrow_t&)
{
	__GarbageCollector::_impl_delete(pvMem);
}

inline void __cdecl operator delete[](void *pvMem, const std::nothrow_t&)
{
	__GarbageCollector::_impl_delete(pvMem);
}

#define new new(_T(__FILE__), __LINE__)

#endif // defined _DEBUG && defined _CE_DETECT_MEMORY_
