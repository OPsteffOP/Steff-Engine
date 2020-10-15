#pragma once
#include <thread>
#include <queue>
#include <functional>
#include <condition_variable>

#define Work std::function<void()>

namespace SteffEngine
{
	namespace Threading
	{
		class ThreadPool
		{
		public:
			PLUGIN_API static ThreadPool* GetInstance();
			ThreadPool(const ThreadPool& threadPool) = delete;
			ThreadPool(ThreadPool&& threadPool) = delete;

			static void Destroy();

			ThreadPool& operator=(const ThreadPool& threadPool) = delete;
			ThreadPool& operator=(ThreadPool&& threadPool) = delete;

			PLUGIN_API void AddWork(Work work);

		private:
			explicit ThreadPool();
			~ThreadPool();

			void CreateWorkers();

			static ThreadPool* m_pInstance;
			static const size_t m_WorkerThreadsCount{ 5 };

			std::mutex m_Mutex;
			std::condition_variable m_ConditionalVariable;
			bool m_IsShuttingDown;

			std::vector<std::thread> m_Workers;
			std::queue<Work> m_Work;
		};
	}
}