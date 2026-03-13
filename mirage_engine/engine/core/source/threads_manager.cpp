#include "threads_manager.h"
#include "threads_worker.h"

DEFINE_LOG_CATEGORY(THREAD)

Bwt::Core::ThreadPool::ThreadPool(size_t num_threads)
{
	if (num_threads > 1000)
		num_threads = 1000;
	if (num_threads == 0)
		num_threads = 1;
	for (int i = 0; i < num_threads; ++i)
	{
		m_workers.push_back(std::make_unique<ThreadWorker>(i, this));
		LOG(logDebug, THREAD, "Thread ", i, " Initialized.");
	}
	LOG(logDebug, THREAD, "Initializing ", num_threads, " threads.");
}

Bwt::Core::ThreadPool::~ThreadPool()
{	
	ShutDown();
}

void Bwt::Core::ThreadPool::ShutDown()
{
	if (m_shutdownRequested.exchange(true))
		return;
	LOG(logDebug, THREAD, "ThreadPool asked shutdown");
	m_condition_variable.notify_all();
	for (std::unique_ptr<ThreadWorker>& worker : m_workers)
	{
		if (worker) 
		{
			worker->Join();
		}
	}
	m_queuesHigh.clear();
	m_queuesNormal.clear();
	m_queuesLow.clear();
}
