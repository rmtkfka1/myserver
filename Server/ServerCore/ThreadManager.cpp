#include "pch.h"
#include "ThreadManager.h"
#include "CoreTLS.h"
#include "CoreGlobal.h"

Atomic<uint32> ThreadManager::_SThreadId = 0;
/*------------------
	ThreadManager
-------------------*/

ThreadManager::ThreadManager()
{
	// Main Thread
	InitTLS();
}

ThreadManager::~ThreadManager()
{
	Join();
}

void ThreadManager::Launch(function<void(void)> callback)
{
	LockGuard guard(_lock);

	_threads.push_back(thread([=]()
		{
			InitTLS();
			callback();
			DestroyTLS();
		}));
}

void ThreadManager::Join()
{
	for (thread& t : _threads)
	{
		if (t.joinable())
			t.join();
	}

	_threads.clear();
}

void ThreadManager::InitTLS()
{
	LThreadId = _SThreadId.fetch_add(1);
}

void ThreadManager::DestroyTLS()
{
	_SThreadId--;
}