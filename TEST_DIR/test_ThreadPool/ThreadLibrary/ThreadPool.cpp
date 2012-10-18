/**
 * @file
 */
#include <process.h>	
#include <windows.h>
#include "ThreadPool.h"
#include "ThreadRequest.h"

#include <cassert>

using std::for_each;

namespace
{
// Any other data that we wish to transfer to the thread for performance monitoring or whatever can
// be added to this structure later
struct ThreadData							
{
	ThreadPool *threadPool;		///< Pointer to this object
	unsigned int threadId;		///< Id of the thread
};

/** 
 *	SuicideException used by the suicide pill to break out of the accept loop
 */
class SuicideException
{
};
/**
 * SuicidePill is a do nothing request that forces the thread to loop through once, checking
 * the alive flag, before shutting itself down cleanly.
 * It performs this by throwing a ThreadPoolShutdownException which actually terminates 
 */
class SuicidePill : public ThreadRequest
{
public:
	void operator()(int)
	{
		throw SuicideException();		// Terminate this thread cleanly
	}
};
}

/**
 *	Initialize the thread pool.
 *	@param threadSize Number of threads to initialize the pool to.
 *	@param queueSize Size of the queue before pool will stop entries.
 */
ThreadPool::ThreadPool(int threadSize, int queueSize) :		queueGuard(), 
															notFull(), 
															queueAccess(queueSize, 0), 
															alive(false), 
															threads(threadSize)
{
	// Initialize the queue
	notFull.signal();				// Queue is not full
	pool.reserve(threadSize);
}
/// Static function to fire off all of the accept handlers for each thread
unsigned int __stdcall ThreadPool::internalThreadProc(void* lpParam)
{
	// This is ALL running in a new thread...
	ThreadData *data = static_cast<ThreadData*>(lpParam);		// convert the lpParam back to the data structure
	try
	{
		data->threadPool->acceptHandler(data->threadId);			// And start this threads accept handler.
	}
	catch(...)
	{
	}
	delete data;				
	_endthreadex(0);
	return 0; 
}

/**
 *	Internal function to handle the thread requests
 *	@param threadId Id of the thread - debugging purposes only really
 *	@warning This will NOT propagate exceptions out of the function.  Exceptions must be caught
 *           by the command object placed in the queue.
 */
void ThreadPool::acceptHandler(unsigned int threadId) throw()
{
	onThreadStart(threadId);
	for(;;)
	{
		// Ensure that only one thread is actually waiting on the notEmpty signal at a time.
		// Eliminates race conditions to get at the queue
		::WaitForSingleObject(queueAccess, INFINITE);
		try
		{
			queueGuard.enter();
			if(!jobQueue.empty())
			{
				std::auto_ptr<ThreadRequest> request(jobQueue.front());		// Autopointer takes ownership of request
				jobQueue.pop();
				notFull.signal();						// Ensure that it's not full, coz we just took something out!
				queueGuard.leave();
				try
				{
					request->operator()(threadId);			// Now do your stuff!!!
				}
				catch(SuicideException&)				// Occurs if we've swallowed the suicide pill
				{
					break;								// exit the acceptHandler loop to terminate thread
				}
				// The following exception should NEVER occur because the request->operator()() should NOT
				// be propogating requests.
				catch(...)
				{
					// Just swallow the exception and continue on.
				}
			}
			else
			{
				queueGuard.leave();
			}
		}
		catch(...)
		{
			queueGuard.leave();		// Release the accessor and continue on.
		}
	} // for(;;)
	onThreadFinish(threadId);
}

/**	
 *	@name Thread Start Finish functions
 *	ThreadSpecific Thread initialization and termination functions.
 *	Provide thread specific initialization for EACH thread.
 *	Override this function to provide thread specific initialization.  For instance, to initialize a COM component
 *	from each thread you would derive a new ThreadPool, and override onThreadStart() with a ::CoInitialize(NULL) and
 *	override onThreadFinish() with a ::CoUninitialize()
 */

//@{
/**
 *	Thread Start function.
 *	Derived classes should override this to perform initialization for EACH thread.
 *	@param threadId ThreadID for this threadstart routine.  Useful for debugging.
 *	@throw NOTHING  Note that this function is declared as throwing no exceptions.  You MUST handle the derived
 *	function exceptions yourself.
 */
void ThreadPool::onThreadStart(int) throw()
{
}
/**
 *	Thread Finish function.
 *	Derived classes should override this to cleanup any thread stuff that may have been initialized in onThreadStart()
 *	@param threadId ThreadID for this threadstart routine.  Useful for debugging.
 *	@throw NOTHING  Note that this function is declared as throwing no exceptions.  You MUST handle the derived
 *	function exceptions yourself.
 */
void ThreadPool::onThreadFinish(int) throw()
{
}
//@}

/**
 *	Initialize all of the threads and then set them into action...
 *	@return number of threads started
 */
int ThreadPool::accept()
{
	if(!alive)						// Cannot accept twice!
	{
		for(int i = 0 ; i < threads ; i++)
		{
			try
			{
				void *handle = NULL;
				ThreadData *data = new ThreadData;			// Deleted on thread destruction in internalThreadProc
				assert(data);								// ensure we've allocated it.
				data->threadPool = this;
				handle = reinterpret_cast<void*>(_beginthreadex(NULL, 0, internalThreadProc, data, CREATE_SUSPENDED, &(data->threadId)));
				if(handle != NULL)				// Ignore threads that aren't created
					pool.push_back(handle);		// and push the others onto the pool.
			}
			catch(...)
			{
				assert(false);		// code shouldn't reach here!
				// I think we can ignore this.  If the thread doesn't get created, we go on to the next.
				// this should be a pretty damn rare experience, and one that needs to be tested
				// and investigated further in case it does happen.
			}
		}
		for_each(pool.begin(), pool.end(), ::ResumeThread);
		alive=true;		
	}
	return static_cast<int>(pool.size());
}
/**
 *	Adds a request onto the queue for servicing by the thread pool. 
 *  @param request Pointer to an object derived from thread request, overriding operator()
 *	@exception ThreadPoolShutdownException Thrown if the threadpool is closing down
 */
void ThreadPool::submitRequest(ThreadRequest *request)
{
	if(!alive)
		throw ThreadPoolShutdownException();

	::WaitForSingleObject(notFull, INFINITE);
	try
	{
		queueGuard.enter();						// Need to ensure that the queue is locked for this - STL queue isn't threadsafe
		jobQueue.push(request);
		if(jobQueue.size() >= MAX_QUEUE_SIZE)
			notFull.reset();						// Clear the notfull signal to block the thread for future insertions
		queueAccess.release();						// Release 1 slot to allow a thread in to service the queue
		queueGuard.leave();
	}
	catch(std::exception&)
	{
		queueGuard.leave();		// Must release the exception first...
		throw;					// Then we can propogate it out.
	}
}
/**
 *	Shutsdown all of the threads.
 *	Forces each thread to swallow a suicide pill to terminate itself.  Once the queue has 
 *	enough suicide pills for the number of active threads, the queue is marked as
 *  no longer servicing requests.
 *	Upon completion of the suicide pill, shutdown blocks for 10 seconds to ensure that all 
 *	threads have been terminated, and then goes and closes the handles.
 */
void ThreadPool::shutdown()
{
	for(std::vector<void*>::const_iterator j = pool.begin() ; j != pool.end() ; ++j)
		this->submitRequest(new SuicidePill());			// Feed thread a suicide pill
	alive = false;			// Stop the queue receiving

	for(std::vector<void*>::const_iterator i = pool.begin() ; i != pool.end() ; ++i)
	{
		switch(::WaitForSingleObject(*i, 10000))
		{
		case WAIT_OBJECT_0:
			// This is what we WANT to happen.
			break;

		case WAIT_TIMEOUT:			// Something bad happened - force the termination
			::TerminateThread(*i, 1);
			break;

		case WAIT_ABANDONED:	    // The thread has been terminated already!?  maybe...
			break;
		}
		::CloseHandle(*i);
	}
} 


ThreadPool::~ThreadPool(void)
{
}
