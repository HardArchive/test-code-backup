/**
 *	@file
 */
#pragma once
#include "event.h"
#include <cassert>

/** 
 *	Provides a wrapper around the WIN32 event objects
 *	
 *	@author Peter Hancock
 */
class ManualEvent :
	public Event
{
public:
	/**
	 *	Create an event.
	 *	@param eventName	Name of the event for external processes
	 *	@throw EventException	Thrown when the event can't be created
	 *	@note If using named events, the name is GLOBAL.  ie - you will not be able to run more
	 *	than one process with the same name.  An event is a kernel object.
	 */
	ManualEvent(const char* eventName=NULL)
	{
		// Note that the handle is closed in the base Event class.
		handle = ::CreateEvent(NULL, true, false, eventName);
		if(handle == NULL)
			throw EventException(eventName);
		assert(handle);
	}
	/** 
	 *	Signal the event.
	 */
	void signal() throw()
	{
		assert(handle);
		::SetEvent(handle);
	}
	/**
	 *	Reset the event to the non-signaled state
	 */
	void reset() throw()
	{
		assert(handle);
		::ResetEvent(handle);
	}
};
