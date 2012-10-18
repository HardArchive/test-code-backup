/**
 *	@file
 */
#include <Windows.h>
#include <iostream>
#include <map>
#include "..\ThreadPool.h"
#include "..\CriticalSection.h"
#include <cstdlib>		// For the random number
#include <ctime>		// Random number seed


CriticalSection coutGuard;		///< Guard the output so that it's not mishmashed by thread preemption
CriticalSection statsGuard;		///< Ensure that the statistics are threadsafe!

/**
 *  Simple class to illustrate methods of population from within the thread pool...
 */
struct ThreadStats
{
	int commandExecutions;			///< Number of times the thread has executed a functor
	int totalWaitTime;				///< Total length of time that the functors have had the thread waiting
};

/**
 *	Test request functor to illustrate how you override the operator() function, to perform its own action
 */
class TestRequest : public ThreadRequest
{
public:
	/*
	 *	@param requestNumber	Number of the functor
	 *	@param stats			reference to the statistics structure that the functor will update
	 */
	TestRequest(int requestNumber, std::map<int, ThreadStats>& stats) : number(requestNumber), stats(stats)
	{
		srand(static_cast<unsigned int>(time(0)));
	}

	void operator()(int threadId) throw()
	{
		try
		{
			// Do a random number wait... 
			int waitTime=0;
			if(!(threadId % 10))
				waitTime = (rand() % 1000) + 3000;
			else
				waitTime = rand() % 1000;
			::Sleep(waitTime);				// Wait up to 1 second before completing and 4 seconds on multiples of 10
			
			updateStats(threadId, waitTime);
			Guard safeGuard(coutGuard);
			std::cout << threadId << ": Request: " << number << " waited for " << waitTime << " milliseconds" << std::endl;
		}	
		catch(...)
		{
			// Cannot allow the exception to propagate out
		}
	}

private:
	int number;								///< This functors unique number
	std::map<int, ThreadStats>& stats;		///< reference to the global stats structure

	/**
	 *	Helper method to update the statistics map, ensuring that the integrity of the 
	 *  map is maintained using the statsGuard.
	 *	@param threadId Id of the statistic structure to update
	 *	@param waitTime Length of time this functor waited for
	 */
	void updateStats(int threadId, int waitTime)
	{
		Guard safeGuard(statsGuard);
		if(stats.find(threadId)==stats.end())
		{
	        ThreadStats currentStat;
			currentStat.commandExecutions = 1;
			currentStat.totalWaitTime = waitTime;
			stats.insert(std::make_pair<int, ThreadStats>(threadId, currentStat));
		}
		else
		{
			std::map<int, ThreadStats>::iterator item = stats.find(threadId);
			(*item).second.commandExecutions++;
			(*item).second.totalWaitTime += waitTime;
		}
	}
	TestRequest(const TestRequest& copy);
	TestRequest& operator=(const TestRequest& rhs);
};

/**
 * Derived thread class showing example of thread specific initialization and termination.
 */
class COMPool : public ThreadPool
{
public:
	/**
	 *	Start by initializing COM
	 *	@param threadId Number of the thread
	 */
	void onThreadStart(int threadId)
	{
		Guard safeGuard(coutGuard);
		std::cout << "Starting COM for thread " << threadId << std::endl;
		::OleInitialize(NULL);	
	}

	/**
	 *	Shutdown COM when we are finished
	 *	@param threadId Number of the thread
	 */
	void onThreadFinish(int threadId)
	{
		Guard safeGuard(coutGuard);
		std::cout << "Stopping COM for thread " << threadId << std::endl;
		::OleUninitialize();
	}
};


int main(int, char*)
{
	COMPool myThread;
	std::map<int, ThreadStats> stats;
	myThread.accept();

	for(int j = 0 ; j < 200 ; ++j)
	{
		int waitTime = rand() % 100;
		::Sleep(waitTime);				// Wait up to 1 second before completing.
		myThread.submitRequest(new TestRequest(j, stats));
	}
	myThread.shutdown();
	
	// Now output the results 
	for(std::map<int, ThreadStats>::const_iterator i = stats.begin() ; i != stats.end() ; ++i)
		std::cout << "Thread Id: " << (*i).first << " executed " << (*i).second.commandExecutions << " times with total wait of " << (*i).second.totalWaitTime << std::endl;
	return 0;
}

