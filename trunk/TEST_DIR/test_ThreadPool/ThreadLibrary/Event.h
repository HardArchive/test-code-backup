/**
 *	@file
 */
#pragma once
#include <windows.h>
#include <string>
#include <exception>

/** 
 *	Provides an abstract base class for windows signal events
 *  Event class objects should create override the constuctor, setting a valid handle
 *  value for the base class.  The base class provides will close the handle on desruction
 *  @note Do not close the handle in the derived classes.
 */
class Event
{
public:
	/**	
	 *	Initializes the handle to nothing
	 */
	Event() : handle(NULL)
	{
	};
	
	/**
	 *	Closes the event handle
	 */
	virtual ~Event()
	{
		if(handle)						// Just in case somebody DOES close the handle in the derived class
			::CloseHandle(handle);
	}
	
	/**
	 *	Signals an event.
	 *	Must be overridden in derived classes.
	 */
	virtual void signal() = 0;

	/** 
	 *	Allows events to be treated as native WIN32 objects.
	 */
	operator void*() const
	{
		return handle;
	}

protected:
	void *handle;
};

/**
 *	EventException thrown when there are problems creating events.
 *
 *	@author Peter Hancock
 */
class EventException : public std::exception
{
public:
	/**
	 *	Creates an exception with a message.
	 *	@param mesg	Message to pass back with the exception.
	 */
	EventException(const char* mesg) : std::exception(mesg)
	{
	}
};
