/////////////////////////////////////////////////////////////////////
// KernelObject.h
// Rao Changzheng, 1998-08-13
#ifndef __KERNELOBJECT_H__
#define __KERNELOBJECT_H__

/////////////////////////////////////////////////////////////////////
// Includes
#include <assert.h>
#include <string.h>
#include <iostream.h>
#include <process.h>
#include <windows.h>
#include <winbase.h>



/////////////////////////////////////////////////////////////////////
// Constants
#define constDEFAULTOBJECTNAME "KERNELOBJECT"
const long constMAXIMUMWAITCOUNT =128;



/////////////////////////////////////////////////////////////////////
//
// CTCriticalSection
//
// CTCounter
//
// CTSingleObject
//     |
//     +-----> CTThread
//     |
//     +-----> CTMutex -----> CTInterMutex <---------------+
//     |                                                   |
//     +-----> CTSemaphore -> CTInterSemaphore <-----------+
//     |                                                   |
//     +-----> CTEvent -----> CTInterEvent <---------------+
//                                                         |
// CTInterObject -----------------------------------------+
//                                                        |
// CTMultiObject                                          |
//     |                                                  |
//     +-----> CTMultiMutex -----> CTInterMultiMutex <----+
//     |                                                  |
//     +-----> CTMultiSemaphore -> CTInterMultiSemaphore <-+
//     |                                                 |
//     +-----> CTMultiEvent -----> CTInterMultiEvent <-----+
//



///////////////////////////////////////////////////////////////////////////////
// CTCriticalSection Class
//
class CTCriticalSection
{
	private:
		CRITICAL_SECTION CriticalSection;

	public:
		CTCriticalSection() { ::InitializeCriticalSection( &CriticalSection ); }
		~CTCriticalSection() { ::DeleteCriticalSection( &CriticalSection ); }

		void Enter() { ::EnterCriticalSection( &CriticalSection ); }
		void Leave() { ::LeaveCriticalSection( &CriticalSection ); }
		void Lock() { ::EnterCriticalSection( &CriticalSection ); }
		void Unlock() { ::LeaveCriticalSection( &CriticalSection ); }

};



///////////////////////////////////////////////////////////////////////////////
// CTCounter Class
//
class CTCounter
{
	private:
		long m_lCounter;

	public:
		CTCounter( long lCounter = 0 ) : m_lCounter( lCounter ) { }

		long Get() { return m_lCounter;	}
		long Set( long lCounter ) {	return ::InterlockedExchange( &m_lCounter, lCounter ); }

		long Inc() { return ::InterlockedIncrement( &m_lCounter ); }
		long Dec() { return ::InterlockedDecrement( &m_lCounter ); }
		long operator = ( int rval ) { Set( rval ); return Get(); }
};



///////////////////////////////////////////////////////////////////////////////
// CTSingleObject Class
//
class CTSingleObject
{
	protected:
		HANDLE Handle;

	public:
		CTSingleObject() : Handle( 0 ) {	}
		~CTSingleObject() { ::CloseHandle( Handle ); }

		HANDLE GetHandle() { return Handle; }
};



///////////////////////////////////////////////////////////////////////////////
// CTThread Class
//
// 1. The specified thread function prototype should be like this:
// e.g. "DWORD WINAPI YourThreadFunc( LPVOID lpvThreadParm );"
// 2. NOT ALL Win32 APIs are implemented in Class TThread.
//

typedef unsigned ( __stdcall * THREADFUNC )( void * ) ;

class CTThread : public CTSingleObject
{
	private:
		DWORD                  m_dwThreadId ;
		LPTHREAD_START_ROUTINE m_lpStartAddress ;
		LPVOID                 m_lpParameter ;

	public:
		CTThread(
			LPTHREAD_START_ROUTINE lpStartAddress,
			LPVOID                 lpParameter,
			DWORD                  dwCreationFlags = 0,//Run immediately
			DWORD                  dwStackSize = 0 )
			: m_lpStartAddress( lpStartAddress ),m_lpParameter( lpParameter )
		{
			Handle = (HANDLE)_beginthreadex( 0, dwStackSize, (THREADFUNC)lpStartAddress,
								lpParameter, dwCreationFlags, (unsigned *)& m_dwThreadId );
			if ( !Handle )
			{
				cout << "[TThread()] _beginthreadex() last error: "
					 << ::GetLastError() << endl;
				exit( -1 );
			}
		}

		//Should only apply to current thread
		void Exit( DWORD dwExitCode )
		{
			if ( GetCurrentThreadId()==m_dwThreadId )
			{
				_endthreadex( (unsigned)dwExitCode );
			}
			else
			{
				Terminate( (unsigned)dwExitCode );
			}
		}

		BOOL GetExitCode( DWORD & dwExitCode )
		{
			return ::GetExitCodeThread( Handle, &dwExitCode );
		}

		DWORD Resume( )
		{
			return ::ResumeThread( Handle );
		}

		DWORD Continue( )
		{
			return ::ResumeThread( Handle );
		}

		DWORD Suspend( )
		{
			return ::SuspendThread( Handle );
		}

		BOOL Terminate( DWORD dwExitCode )
		{
			return ::TerminateThread( Handle, dwExitCode );
		}

		BOOL Kill( DWORD dwExitCode )
		{
			return ::TerminateThread( Handle, dwExitCode );
		}

		DWORD GetRealId()
		{
			return m_dwThreadId;
		}

		DWORD GetId()
		{
			return ::GetCurrentThreadId();
		}

		BOOL SetPriority( int nPriority )
		{
			return ::SetThreadPriority( Handle, nPriority );
		}

		int GetPriority()
		{
			return ::GetThreadPriority( Handle );
		}
} ;



///////////////////////////////////////////////////////////////////////////////
// TMutex Class
//
class CTMutex : public CTSingleObject
{
	public:
		CTMutex( char* aName = 0, BOOL bInitialOwner = FALSE )
		{
			Handle = ::CreateMutex( 0, bInitialOwner, aName );
			if ( !Handle )
			{
				cout << "[TMutex()] CreateMutex() last error:" << GetLastError() << endl;
				exit( -1 );
			}
		}

		DWORD Lock( DWORD dwMilliseconds = INFINITE )
		{ return ::WaitForSingleObject( Handle, dwMilliseconds ); }

		BOOL Unlock()
		{ return ::ReleaseMutex( Handle ); }

		BOOL IsLocked()
		{ return WAIT_TIMEOUT==::WaitForSingleObject( Handle, 0 ); }
};

//typedef TMutex TMutexLock;



///////////////////////////////////////////////////////////////////////////////
// TSemaphore Class
//
class CTSemaphore : public CTSingleObject
{
	public:
		CTSemaphore( long  lInitialCount,
					long  lMaximumCount = constMAXIMUMWAITCOUNT,
					char* aName = 0 )
		{
			assert( lInitialCount <= lMaximumCount );
			Handle = ::CreateSemaphore( 0, lInitialCount, lMaximumCount, aName );
			if ( !Handle )
			{
				cout << "[CTSemaphore()] CreateSemaphore() last error: "
					 << ::GetLastError() << endl;
				exit( -1 );
			}
		}

		DWORD Wait( DWORD dwMilliseconds = INFINITE )
		{ return ::WaitForSingleObject( Handle, dwMilliseconds ); }


		BOOL Post( long lReleaseCount = 1 )
		{ return ::ReleaseSemaphore( Handle, lReleaseCount, 0 ); }

		DWORD Lock( DWORD dwMilliseconds = INFINITE )
		{ return Wait( dwMilliseconds ); }

		BOOL Unlock() {	return Post( 1 ); }

		BOOL IsLocked()
		{ return WAIT_TIMEOUT==::WaitForSingleObject( Handle, 0 ); }

		// -1 - error; 0~lMaximumCount - ok
		int GetCount()
		{
			int count;
			if ( TRUE==::ReleaseSemaphore( Handle, 0, (long*)&count ) )
			{
				return count;
			}
			return -1;
		}
};



///////////////////////////////////////////////////////////////////////////////
// TEvent Class
//
class CTEvent : public CTSingleObject
{
		BOOL   m_bManualReset ;

	public:
		CTEvent( BOOL  bManualReset = TRUE,
				BOOL  bInitialState = TRUE,
				char* aName = 0 )
			: m_bManualReset( bManualReset )
		{
			Handle = ::CreateEvent( 0, bManualReset, bInitialState, aName );
			if ( !Handle )
			{
				cout << "[TEvent()] CreateEvent() last error: "
					 << ::GetLastError() << endl;
				exit( -1 );
			}
		}

		BOOL Set() { return ::SetEvent( Handle ); }
		BOOL Reset() { return ::ResetEvent( Handle ); }

		BOOL Pulse() { return ::PulseEvent( Handle ); }
		BOOL IsManualReset() { return m_bManualReset; }

		DWORD Wait( DWORD dwMilliseconds = INFINITE )
		{ return ::WaitForSingleObject( Handle, dwMilliseconds ); }
};



///////////////////////////////////////////////////////////////////////////////
// TInterObject Class
//
class CTInterObject
{
	protected:
		char Name[MAX_PATH+1];
		int  AlreadyExist;

	public:
		CTInterObject( char* aName )
			: AlreadyExist( 0 )
		{
			if ( aName )
			{
				strncpy( Name, aName, MAX_PATH );
				Name[MAX_PATH] = 0;
			}
			else
			{
				#ifdef __KERNELOBJECT__
				cout << "[TInterObject()] A name must be specified!" << endl;
				#endif
				exit( -1 );
			}
		}

		char* GetName() { return Name; }
		int IsFirstOne() { return !AlreadyExist; }
};



///////////////////////////////////////////////////////////////////////////////
// TInterMutex Class
//
class CTInterMutex : public CTMutex, public  CTInterObject
{
	public:
		CTInterMutex( char* aName = 0 )
			: CTInterObject( aName ), CTMutex( aName )
		{
			// See if it is the first one
			if ( ERROR_ALREADY_EXISTS==GetLastError() )
			{
				#ifdef __KERNELOBJECT__
				cout << "[TInterMutex()] Already Exists!" << endl;
				#endif
				AlreadyExist = 1;
			}
			else
			{
				#ifdef __KERNELOBJECT__
				cout << "[TInterMutex()] Do NOT exist before!" << endl;
				#endif
				AlreadyExist = 0;
			}
		}
};



///////////////////////////////////////////////////////////////////////////////
// TInterSemaphore Class
//
class CTInterSemaphore : public CTSemaphore, public CTInterObject
{
	public:
		CTInterSemaphore( long  lInitialCount,
						 long  lMaximumCount = constMAXIMUMWAITCOUNT,
						 char* aName = 0 )
			: CTInterObject( aName ),
			CTSemaphore( lInitialCount, lMaximumCount, aName )
		{
			// See if it is the first one
			if ( ERROR_ALREADY_EXISTS==GetLastError() )
			{
				#ifdef __KERNELOBJECT__
				cout << "[TInterSemaphore()] Already Exists!" << endl;
				#endif
				AlreadyExist = 1;
			}
			else
			{
				#ifdef __KERNELOBJECT__
				cout << "[TInterSemaphore()] Do NOT exist before!" << endl;
				#endif
				AlreadyExist = 0;
			}
		}
};



///////////////////////////////////////////////////////////////////////////////
// TInterEvent Class
//
class CTInterEvent : public CTEvent, public CTInterObject
{
	public:
		CTInterEvent(BOOL  bManualReset = TRUE,
					BOOL  bInitialState = TRUE,
					char* aName = 0 )
			: CTInterObject( aName ),
			CTEvent( bManualReset, bInitialState, aName )
		{
			// See if it is the first one
			if ( ERROR_ALREADY_EXISTS==GetLastError() )
			{
				#ifdef __KERNELOBJECT__
				cout << "[TInterEvent()] Already Exists!" << endl;
				#endif
				AlreadyExist = 1;
			}
			else
			{
				#ifdef __KERNELOBJECT__
				cout << "[TInterEvent()] Do NOT exist before!" << endl;
				#endif
				AlreadyExist = 0;
			}
		}
};



///////////////////////////////////////////////////////////////////////////////
// TMultiObject Class
//
class CTMultiObject
{
	protected:
		HANDLE*  Handle;
		int      Size;
		CTCounter Counter;

	public:
		CTMultiObject( int aSize );
		~CTMultiObject();
};



///////////////////////////////////////////////////////////////////////////////
// CTMultiEvent Class
//
class CTMultiEvent
{
	private:
		HANDLE*  m_hMultiEvent;
		BOOL     m_bManualReset;
		int      m_iEventSize;
		CTCounter m_Counter;

	public:		
		CTMultiEvent(int iEventSize, BOOL bManualReset = TRUE, BOOL bInitialState = TRUE )
			: m_bManualReset(bManualReset)
		{
			assert( 0<iEventSize && iEventSize<=MAXIMUM_WAIT_OBJECTS );
			m_iEventSize  = iEventSize;
			m_hMultiEvent = new HANDLE[iEventSize];
			for( int i=0; i<m_iEventSize; i++ )
			{
				m_hMultiEvent[i] = ::CreateEvent( 0, bManualReset, bInitialState, 0 );
			}
			m_Counter.Set(0);
		}

		~CTMultiEvent()
		{
			for( int i=0; i<m_iEventSize; i++ )
			{
				::CloseHandle( m_hMultiEvent[i] );
				m_hMultiEvent[i] = 0;
			}
			delete [] m_hMultiEvent;
			m_hMultiEvent  = 0;
			m_bManualReset = FALSE;
		}

		DWORD WaitAll( DWORD dwTimeOut = INFINITE )
		{
			return WaitForMultipleObjects( m_iEventSize, m_hMultiEvent, TRUE, dwTimeOut );
		}

		DWORD WaitOne( DWORD dwTimeOut = INFINITE )
		{
			return WaitForMultipleObjects( m_iEventSize, m_hMultiEvent, FALSE, dwTimeOut );
		}

		BOOL Set( const int index )
		{
			assert( 0<=index && index<m_iEventSize );
			return ::SetEvent( m_hMultiEvent[index] ) ;
		}

		BOOL Reset( int& index )
		{
			if ( m_Counter.Get()<0 || m_Counter.Get()>=m_iEventSize )
			{
				return FALSE;
			}
			index = m_Counter.Get();
			m_Counter.Inc();
			return ::ResetEvent( m_hMultiEvent[index] );
		}

		BOOL Pulse( const int index )
		{
			assert( 0<=index && index<m_iEventSize );
			return ::PulseEvent( m_hMultiEvent[index] ) ;
		}
};



///////////////////////////////////////////////////////////////////////////////
// TMultiEvent Class
//
class CTShareMemory : public CTSingleObject, public CTInterObject
{
		int    Size;

	public:
		char*  Memory;

	public:
		CTShareMemory( char* aName, int aSize );
		~CTShareMemory() { }

		int GetSize() { return Size; }

};

#endif
