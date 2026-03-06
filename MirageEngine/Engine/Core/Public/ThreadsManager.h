#pragma once
#include <atomic>
#include <condition_variable>
#include <functional>
#include <future>
#include <Logs/LogMacros.h>
#include <mutex>
#include <deque>
#include <thread>

DECLARE_LOG_CATEGORY_EXTERN(THREAD)


namespace Bwt
{
	namespace Core
	{
		enum class JobPriority
		{
			High,
			Normal,
			Low
		};
		struct Job 
		{
			JobPriority m_priority;
			std::function<void()> m_func;
		};


		class ThreadPool
		{
			friend class ThreadWorker;

		public:
			using ThreadID = uint64_t;

			ThreadPool(size_t num_threads = std::thread::hardware_concurrency());
			~ThreadPool();

			/// <summary>
			/// Takes a function and its arguments and enqueues it for execution
			/// </summary>
			/// <param name="func"> The function to execute (e.g. ClassName::Function). </param>
			/// <param name="args"> Arguments forwarded to the function. </param>
			/// <returns>A std::future holding the result of the task. </returns>
			template <typename Func, typename... Args>
			auto EnqueueTask(JobPriority prio, Func&& func, Args&&... args) -> std::pair< std::future<std::invoke_result_t<Func, Args...>>, std::shared_ptr<Job>>
			{
				LOG(logDebug, THREAD, "Enqueue new task");
				using ReturnType = std::invoke_result_t<Func, Args...>; // Declare an alias for the type returned

				auto task = std::make_shared<std::packaged_task<ReturnType()>>(std::bind(std::forward<Func>(func), std::forward<Args>(args)...));

				std::shared_ptr<Bwt::Core::Job> job = std::make_shared<Job>();
				job->m_func = [task]() { (*task)(); };
				job->m_priority = prio;

				// bind the function and integrate the args into parameter
				std::future<ReturnType> result = task->get_future();
				{
					std::lock_guard<std::mutex> lock(m_mutex); // Protect queue access
					if (m_shutdownRequested)
						throw std::runtime_error("ThreadPool is shutting down");

					// enqueue task
					switch (prio)
					{
					case Bwt::Core::JobPriority::High:
						m_queuesHigh.push_back(job);
						break;
					case Bwt::Core::JobPriority::Normal:
						m_queuesNormal.push_back(job);
						break;
					case Bwt::Core::JobPriority::Low:
						m_queuesLow.push_back(job);
						break;
					default:
						break;
					}
				}
				m_condition_variable.notify_one();
				return { std::move(result), job };
			}

			void ShutDown();
		private:
			std::mutex m_mutex; // Mutex for protecting the queue
			std::condition_variable m_condition_variable; // Condition variable for signaling finished threads

			std::vector<std::unique_ptr<ThreadWorker>> m_workers;

			std::deque<std::shared_ptr<Job>> m_queuesHigh;
			std::deque<std::shared_ptr<Job>> m_queuesNormal;
			std::deque<std::shared_ptr<Job>> m_queuesLow;
			std::atomic<bool> m_shutdownRequested{ false };

			// Prevent copying
			ThreadPool(const ThreadPool&) = delete;
			ThreadPool& operator=(const ThreadPool&) = delete;
		};
	}
}