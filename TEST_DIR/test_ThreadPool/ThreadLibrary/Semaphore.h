/**
 *	@file
 */
#pragma once
#include <windows.h>
#include <process.h>
#include <string>
#include <exception>
#include <cassert>

/**
 *	SemaphoreException thrown when there are problems creating or using semaphores.
 *
 *	@author Peter Hancock
 */
class SemaphoreException : public std::exception
{
public:
	/**
	 *	Creates an exception with a message.
	 *	@param mesg	Message to pass back with the exception.
	 */
	SemaphoreException(const char* mesg) : std::exception(mesg)
	{
	}
};


/**
 *	Encapsulates a Win32 semaphore.
 *	Provides control over creation and clean destruction of native win32 semaphore handles.  Also
 *  allows the object to be used directly in place of a semaphore handle void the void* overload
 *	
 *	@author Peter Hancock
 * 
 */
class Semaphore
{
public:

	/**
	 *  Creates a semaphore object.
	 *  Semaphores are used to restrict access to a resource to a limited number of threads.  The
	 *	maximum number of threads that can access the resource is set by the maximumSlots argument
	 *  and the initialFree determines the initial number of threads that can access at startup.
	 *	@param maximumSlots Maximum number of threads that can access the protected object
	 *  @param initialFree  Initial number of threads that can access the object - default is 0
	 *	@param name Name of the semaphore.  Defaults to NULL
	 *
	 *	@note If using named semaphores, the name is GLOBAL.  ie - you will not be able to run more
	 *	than one process with the same name.
	 *
	 */
	Semaphore(int maximumSlots, int initialFree=0, const char* name=NULL) :	handle(NULL)
	{
		handle = ::CreateSemaphore(NULL, initialFree, maximumSlots, name);
		if(handle == NULL)
		{
			throw SemaphoreException(name);
		}
		assert(handle);
	}

	virtual ~Semaphore(void)
	{
		if(handle)
			::CloseHandle(handle);
	}

	/**
	 * @param count number of slots to release
	 */
	bool release(int count=1)
	{
		assert(handle);
		return ::ReleaseSemaphore(handle, count, NULL) != 0;
	}

	/**
	 *	Overload to act as a "native" WIN32 semaphore object.
	 *	The overload allows the actual object to be used in place of a true WIN32
	 *	HANDLE.
	 *	@return handle The raw WIN32 handle to the semaphore object.
	 */
	operator void*()
	{
		assert(handle);
		return handle;
	}

protected:
	void *handle;				///< Windows handle to the semaphore kernel object

};
