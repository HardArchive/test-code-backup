/**
 * @file
 */
#pragma once
#include <windows.h>

/**
 *	Interface to the threadpool.  
 *	This MUST be derived from with the execute method overridden to use your own custom
 *	handler.  
 * 
 *	@warning execute is declared as throwing no exceptions.  This means that you MUST deal with exceptions in the 
 *           command object.
 * 
 *	@author Peter Hancock
 *
 */
class ThreadRequest
{
public:
	ThreadRequest(){}
	virtual ~ThreadRequest(void){}
	virtual void operator()(int threadId) throw() = 0;			 ///< Must be overridden by your custom dispatch function.
};