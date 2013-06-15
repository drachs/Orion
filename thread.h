#ifndef THREAD_H
#define THREAD_H

#include <pthread.h>
#include <list>

/** @file
 * Contains definitions for various threading classes.
 */

/**
 * Encapsulates a pthread_mutex_t.
 */

class mutex
    {
public:

	/// Lock the mutex.
    void lock(void);

	/// Unlock the mutex.
    void unlock(void);

    /// Try to lock the mutex.  Returns true if successfull.
    bool trylock(void);

    mutex();
    ~mutex();

protected:
    pthread_mutex_t internal_mutex;
    };

/**
 * Encapsulates a recursive pthread_mutex_t.  Virtual constructos would have
 * made this more elegant.
 */

class recursiveMutex
    {
public:

	/// Lock the mutex.
    void lock(void);

	/// Unlock the mutex.
    void unlock(void);

    recursiveMutex();
    ~recursiveMutex();

protected:
    pthread_mutex_t internal_mutex;
    };

/**
 * Encapsulates a conditional variable
 */

class conditional_variable : public mutex
	{
public:
	/// Wait for this cv to receive a signal.  Lock before calling this function.  Unlocks, waits, then locks before returning.
	void wait(void);
	/**
	 * Wake up a single waiting thread. In general, you want to lock modifying the data this CV protects, call this function, and then unlock so that
	 * one of the threads monitoring the data can do it's thing.
	 */
	void signal(void);
	/**
	 * Wake up all waiting threads in turn.  In general, you want to lock modifying the data this CV protects, call this function, and then unlock so that
	 * one of the threads monitoring the data can do it's thing.
	 */
	void broadcast(void);

	conditional_variable();
	~conditional_variable();

private:
	pthread_cond_t internal_conditional_variable;
	};

/**
 * Derive from this class to create multiple %threads.
 * Override the pure virtual function task(), this will be launched in a new thread.
 * Call startThread to create each new thread.   Call stopThreads to cause isRunning to return false.
 * Monitor isRunning from task, when it starts returning false finish up and return.   stopThreads does not return until all threads stop.
 * Destructor calls stopThreads.
 */

class threads
	{
public:
	threads();
	virtual ~threads();

	/**
	 * Starts a new thread running task
	 */
	int startThread(void);
	/// Causes isRunning() to start returning false.  Blocks until all threads running task() return.
	void stopThreads(void);
	/// Override this function with your code to be executed in separate %threads
	virtual void task(void)=0;
	/// Returns the number of threads currently created
	int numThreads(void);
	/**
	 *  Returns false if threads should stop, true if threads should continue.  Does not indicate if threads are running, indicates if threads
	 *  should be running.
	*/
	bool isRunning(void);

private:
	std::list<pthread_t> threadids;
	int numberThreads;
	bool running;
	mutex myMutex;
	static void *entryPoint(void * pthis);
	};

#endif
