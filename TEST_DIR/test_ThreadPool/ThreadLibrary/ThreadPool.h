/**
 *	@file
 */
#pragma once
#include "ThreadRequest.h"
#include "ManualEvent.h"
#include "CriticalSection.h"
#include "Semaphore.h"
#include <vector>
#include <string>
#include <queue>
#include <exception>

namespace
{
const int THREAD_COUNT = 20;				///< Number of threads to initialize to start the requests
const int MAX_QUEUE_SIZE = 100;				///< Maximum size of queue before submitJob blocks
};

/**	
 *	Creates a pool of threads, which will service a queue of funtors.
 * 
 *	The pool of threads is determined by the MAX_THREADS define, or passed in through the constructor.   
 *  To have a thread  execute a request, the user submits a ThreadRequest derived command 
 *  object (functor) to the thread queue.  (I was going to call it threadJob but that's rude).
 *  The request handler will pull this out of the queue, and run the functors operator() method.
 *  If the queue is full, determined by the MAX_QUEUE_SIZE define, or the constructor parameter, 
 *  then the submitRequest method will block preventing new items being added to the queue.  
 *  These are implemented via a ManualEvent object.
 *
 *	@warning Exceptions WILL NOT propogate out of the accept handler.  You MUST catch 
 *           your exceptions in the ThreadRequest derived functor.
 *
 *	@author Peter Hancock
 */
class ThreadPool
{
public:
	ThreadPool(int threadSize=THREAD_COUNT, int queueSize=MAX_QUEUE_SIZE);
	virtual ~ThreadPool();

	int accept();														 ///< Start pool for acceptance
	void submitRequest(ThreadRequest *request);							 ///< Submits a job to the pool for later attachment
	void shutdown();													 ///< Shuts down the thread pool

protected:
	virtual void onThreadStart(int threadId) throw();					///< Runs on EACH thread initialization.  
	virtual void onThreadFinish(int threadId) throw();					///< Runs on EACH thread termination.

private:
	bool alive;														 ///< Contains the suicide pill to close down threads
	std::vector<void *> pool;											///< thread handle pool
	int threads;														///< Number of threads in the pool

	static unsigned int __stdcall internalThreadProc(void* lpParam);	 ///< internal thread proc to run acceptance handlers
	void acceptHandler(unsigned int threadId) throw();					 ///< Internal acceptance handler run the the internalThreadProc
	void *threadData;											///< Pointer to thread data
	std::queue<ThreadRequest*> jobQueue;						///< Pointer to request objects
	Semaphore queueAccess;										///< Ensure that only queue number or max threads will access queue at once
	CriticalSection queueGuard;									///< Ensure that the job Queue is threadsafe
	ManualEvent notFull;										///< Event signalled when queue is not full

	ThreadPool(const ThreadPool&);								// Disable copy and assignment
	ThreadPool& operator=(const ThreadPool&);

};

/**
 *	Thrown when the thread pool is shutting down.
 *	The submitRequest throws this once the threadPool has been requested to terminate.  Prevents clients
 *	from adding requests to the thread pool queue that won't be handled.
 *
 *	@author Peter Hancock
 */
class ThreadPoolShutdownException : public std::exception
{
public:
	ThreadPoolShutdownException(){}
	ThreadPoolShutdownException(const char* mesg) : std::exception(mesg){}
	~ThreadPoolShutdownException(){}
};