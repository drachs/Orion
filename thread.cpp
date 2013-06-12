#include "thread.h"

/** @file thread.cpp
 * Implementations of classes defined in thread.h
 */

/**
 * Threads class
 */
using namespace std;

threads::threads()
    :numberThreads(0), running(false)
    {
    }

threads::~threads()
    {
    this->stopThreads();
    }

bool threads::isRunning(void)
    {
    return(running);
    }

int threads::numThreads(void)
	{
	return(numberThreads);
	}

void threads::stopThreads(void)
    {
    myMutex.lock();
    if (running)
		{
		running = false;

		list<pthread_t>::iterator current = threadids.begin();
		while (current != threadids.end())
			{
			pthread_join(*current, NULL);
			current++;
			numberThreads--;
			}
		}
    myMutex.unlock();
    }

int threads::startThread(void)
    {
	int ret = false;
	pthread_t threadid;

    myMutex.lock();
    running = true;

	if ((ret = !pthread_create(&threadid, NULL, &(threads::entryPoint), this)))
		{
		numberThreads++;
		threadids.push_back(threadid);
		}

	myMutex.unlock();
	return(ret);

    }

void *threads::entryPoint(void *pthis)
    {
    threads *ptr = (threads *) pthis;

    ptr->task();
    return(0);
    }

/*
 *
 * mutex class
 *
 */

void mutex::lock(void)
    {
    pthread_mutex_lock(&internal_mutex);
    }

void mutex::unlock(void)
    {
    pthread_mutex_unlock(&internal_mutex);
    }

mutex::mutex()
    {
    pthread_mutex_init(&internal_mutex, NULL);
    }

mutex::~mutex()
    {
    pthread_mutex_destroy(&internal_mutex);
    }

/*
 *
 * recursiveMutex class
 *
 */

void recursiveMutex::lock(void)
    {
    pthread_mutex_lock(&internal_mutex);
    }

void recursiveMutex::unlock(void)
    {
    pthread_mutex_unlock(&internal_mutex);
    }

recursiveMutex::recursiveMutex()
    {
	pthread_mutexattr_t attr;

	pthread_mutexattr_init(&attr);
	pthread_mutexattr_settype(&attr, PTHREAD_MUTEX_RECURSIVE);

    pthread_mutex_init(&internal_mutex, &attr);

    pthread_mutexattr_destroy(&attr);
    }

recursiveMutex::~recursiveMutex()
    {
    pthread_mutex_destroy(&internal_mutex);
    }
/*
 * Conditional Variable Class
 */

conditional_variable::conditional_variable()
	{
	pthread_cond_init(&internal_conditional_variable, NULL);
	}

conditional_variable::~conditional_variable()
	{
	pthread_cond_destroy(&internal_conditional_variable);
	}

void conditional_variable::wait(void)
	{
	pthread_cond_wait(&internal_conditional_variable, &internal_mutex);
	}

void conditional_variable::signal(void)
	{
	pthread_cond_signal(&internal_conditional_variable);
	}

void conditional_variable::broadcast(void)
	{
	pthread_cond_broadcast(&internal_conditional_variable);
	}
