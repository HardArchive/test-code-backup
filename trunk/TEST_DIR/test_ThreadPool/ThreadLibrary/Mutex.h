/**
 *	@file
 */
#pragma once
#include <string>
#include <windows.h>
#include <exception>

/**
 *	Provides a wrapper around a WIN32 Mutex object.
 *	
 *	@author Peter Hancock
 */
class Mutex
{
public:
	/** 
	 *	Creates a mutex with the passed name.
	 *  @param mutexName Name of the mutex
	 *	@throw MutexException Thrown when the Mutex cannot be created
	 *
	 *	@note If using named mutexes, the name is GLOBAL.  ie - you will not be able to run more
	 *	than one process with the same name.  This is because a mutex is a kernel object.
	 */
	Mutex(const char* mutexName=NULL) : handle(NULL) throw(MutexException)
	{
		handle = reinterpret_cast<void*>(::CreateMutex(NULL, true, mutexName));
		if(handle == NULL)
			throw MutexException(mutexName);
	}

	~Mutex(void)
	{
		if(handle)
			::CloseHandle(handle);
	}
	/** 
	 *	Aquires the mutex
	 *	@param milliseconds Number of seconds to wait blocking
	 *	@return Returns true if the Mutex is acquired, and false if not.
	 *	@throw MutexException Thrown if the Mutex has been abandoned
	 */
	bool aquire(long milliseconds=INFINITE) throw(MutexException)
	{
		unsigned long result= ::WaitForSingleObject(handle, milliseconds);
		switch(result)
		{
		case WAIT_OBJECT_0:
			return true;
			break;

		case WAIT_TIMEOUT
			return false;
			break;

		case WAIT_ABANDONED:
			std::string mesg = "Mutex " + handle + " abandoned";
			throw MutexException(mesg.c_str());
			break;
		}
	}

	/** 
	 *	Release the mutex
	 */
	void release()
	{
		::ReleaseMutex(handle);
	}
		
	/**
	 *	Allows the Mutex object to be handled as a native WIN32 object.
	 */
	operator void*() const
	{
		return handle;		
	}

private:
	void* handle;		///< internal WIN32 handle
};

/**
 *	Thrown when there are errors in creating a mutex.
 *	@author Peter Hancock
 */
class MutexException : public std::exception
{
public:
	/**
	 *	Creates an exception with a message
	 *	@param mesg Message that you wish to pass back with the exception
	 */
	MutexException(const char* mesg) : std::exception(mesg)
	{
	}
};
