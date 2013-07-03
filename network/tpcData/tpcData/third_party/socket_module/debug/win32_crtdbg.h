#pragma once

#if defined _DEBUG && defined _DETECT_MEMORY_LEAK

#ifdef new
	#undef new
#endif

#ifdef delete
	#undef delete
#endif

#ifndef _CRTDBG_MAP_ALLOC
	#define _CRTDBG_MAP_ALLOC
#endif

#include <crtdbg.h>

namespace __dbg_impl
{
	/*
	class CCriticalSection
	{
	public:
		CCriticalSection()	{ ::InitializeCriticalSection( &m_cs ); }
		~CCriticalSection()	{ ::DeleteCriticalSection( &m_cs ); }

		void Enter()		{ ::EnterCriticalSection( &m_cs ); }
		void Leave()		{ ::LeaveCriticalSection( &m_cs ); }

	private:
		CRITICAL_SECTION m_cs;
	};

	class CDumbCS
	{
	public:
		void Enter() { }
		void Leave() { }
	};

	class CGuard
	{
	public:
#ifdef _MT
		typedef CCriticalSection lock_type;
#else
		typedef CDumbCS lock_type;
#endif
		lock_type lock;

	public:
		CGuard()	{ lock.Enter(); }
		~CGuard()	{ lock.Leave(); }
	};
	*/

	class CDebugEnv
	{
	public:
		CDebugEnv()
		{
			::_CrtSetDbgFlag( _CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF );
			::_CrtMemCheckpoint(&s1);
		}

		~CDebugEnv()
		{
			::_CrtMemCheckpoint(&s2);

			if (::_CrtMemDifference( &s3, &s1, &s2))
			{
				TRACE0("!! Memory stats !!\n");
				TRACE0("----------------------------------------\n");
				::_CrtMemDumpStatistics(&s3);
				TRACE0("----------------------------------------\n");
			}
		}

	private:
		_CrtMemState s1, s2, s3;
	};

	static __dbg_impl::CDebugEnv __dbgEnv;
}

inline void* __cdecl operator new(size_t nSize, const char* lpszFileName, int nLine)
{
	// __dbg_impl::CGuard guard;
	return ::_malloc_dbg(nSize, _NORMAL_BLOCK, lpszFileName, nLine);
}

inline void* __cdecl operator new[](size_t nSize, const char* lpszFileName, int nLine)
{
	return operator new(nSize, lpszFileName, nLine);
}

inline void* __cdecl operator new(size_t nSize)
{
	return operator new(nSize, __FILE__, __LINE__);
}

inline void* __cdecl operator new[](size_t nSize)
{
	return operator new(nSize, __FILE__, __LINE__);
}

inline void* __cdecl operator new(size_t nSize, const std::nothrow_t&)
{
	return operator new(nSize, __FILE__, __LINE__);
}

inline void* __cdecl operator new[](size_t nSize, const std::nothrow_t&)
{
	return operator new(nSize, __FILE__, __LINE__);
}

inline void __cdecl operator delete(void* p)
{
	// __dbg_impl::CGuard guard;
	::_free_dbg(p, _NORMAL_BLOCK);
}

inline void __cdecl operator delete[](void* p)
{
	operator delete(p);
}

inline void __cdecl operator delete(void* p, const char* lpszFileName, int nLine)
{
	operator delete(p);
}

inline void __cdecl operator delete[](void* p, const char* lpszFileName, int nLine)
{
	operator delete(p);
}

inline void __cdecl operator delete(void *p, const std::nothrow_t&)
{
	operator delete(p);
}

inline void __cdecl operator delete[](void *p, const std::nothrow_t&)
{
	operator delete(p);
}

#define new new(__FILE__, __LINE__)

#endif // _DEBUG && defined _DETECT_MEMORY_LEAK
