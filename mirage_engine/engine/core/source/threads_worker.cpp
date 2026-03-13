#include "threads_worker.h"
#include "threads_manager.h"
#include <mutex>
#include <functional>

MirageEngine::Core::ThreadWorker::ThreadWorker(uint64_t threadId, ThreadPool* threadpool)
	:m_threadId(threadId), m_threadPool(threadpool)
{
	m_thread = std::make_unique<std::thread>(&ThreadWorker::Work, this);
}

MirageEngine::Core::ThreadWorker::~ThreadWorker()
{
	if (m_thread && m_thread->joinable()) 
	{
		m_thread->join(); 
	}
}

void MirageEngine::Core::ThreadWorker::Work()
{
    while (true)
    {
        std::shared_ptr<Job> job;
        {
            std::unique_lock<std::mutex> lock(m_threadPool->m_mutex);

            m_threadPool->m_condition_variable.wait(lock, [this] {
                return m_threadPool->m_shutdownRequested ||
                    !m_threadPool->m_queuesHigh.empty() ||
                    !m_threadPool->m_queuesNormal.empty() ||
                    !m_threadPool->m_queuesLow.empty();
                });

            if (m_threadPool->m_shutdownRequested) 
            {
                if (m_threadPool->m_queuesHigh.empty() &&
                    m_threadPool->m_queuesNormal.empty() &&
                    m_threadPool->m_queuesLow.empty()) 
                {
                    return;
                }
            }

            job = GetNextJob();
        }

        if (job) {
            try {
                job->m_func();
            }
            catch (const std::exception& e) 
            {
                LOG(logError, THREAD, "Exception in thread ", m_threadId, ": ", e.what());
            }
            catch (...) {
                LOG(logError, THREAD, "Unknown exception in thread ", m_threadId);
            }
        }
    }
}

void MirageEngine::Core::ThreadWorker::Join()
{
    if (m_thread->joinable()) m_thread->join();
}

std::shared_ptr<MirageEngine::Core::Job> MirageEngine::Core::ThreadWorker::GetNextJob()
{
    if (!m_threadPool->m_queuesHigh.empty()) {
        std::shared_ptr< MirageEngine::Core::Job> job = m_threadPool->m_queuesHigh.front();
        m_threadPool->m_queuesHigh.pop_front();
        return job;
    }
    if (!m_threadPool->m_queuesNormal.empty()) {
        std::shared_ptr< MirageEngine::Core::Job> job = m_threadPool->m_queuesNormal.front();
        m_threadPool->m_queuesNormal.pop_front();
        return job;
    }
    if (!m_threadPool->m_queuesLow.empty()) {
        std::shared_ptr< MirageEngine::Core::Job> job = m_threadPool->m_queuesLow.front();
        m_threadPool->m_queuesLow.pop_front();
        return job;
    }
    return nullptr;
}