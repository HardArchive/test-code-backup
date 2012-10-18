/**
 *	@file
 */
#pragma once
#include "event.h"

/**
 *	Wrapper around a WIN32 auto event.
 *	Provides an event or signal that automagically resets itself
 *	
 *	@author Peter Hancock
 *
 */
class AutoEvent :
	public Event
{
public:

	/**
	 *	Creates the auto event object.
	 *	The destructor does not close the handle, the base class Event looks after this.
	 *	@param eventName Name of the event can be used for interprocess communication
	 *	@throw EventException Thrown when the event cannot be created.
	 */
	AutoEvent(const char* eventName="anonymous") throw(EventException)
	{
		handle = reinterpret_cast<void*>(::CreateEvent(NULL, false, false, eventName));
		if(handle == INVALID_HANDLE_VALUE)
			throw EventException;
	}

	/**
	 *	Signal the event to all threads waiting, and then reset the signal once they've all been informed
	 */
	void signal()
	{
		::PulseEvent(handle);
	}
};
