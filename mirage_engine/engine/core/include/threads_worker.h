#include <cstdint>
#include <memory>
#include <thread>

namespace MirageEngine
{
	namespace Core
	{
		class ThreadPool;
		struct Job;
		class ThreadWorker
		{
		public:
			ThreadWorker(uint64_t threadId, ThreadPool* threadpool);
			~ThreadWorker();
			void Work();
			void Join();
		private:
			uint64_t m_threadId;
			ThreadPool* m_threadPool;

			std::unique_ptr<std::thread> m_thread;

			std::shared_ptr<Job> GetNextJob();
		};
	}
}
