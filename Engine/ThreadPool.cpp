#include "pch.h"
#include "ThreadPool.h"

using namespace SteffEngine::Threading;

ThreadPool* ThreadPool::m_pInstance{ nullptr };

ThreadPool::ThreadPool()
	: m_IsShuttingDown(false)
{
	CreateWorkers();
}

PLUGIN_API
ThreadPool* ThreadPool::GetInstance()
{
	if (m_pInstance == nullptr)
	{
		m_pInstance = new ThreadPool{};
	}

	return m_pInstance;
}

void ThreadPool::Destroy()
{
	SAFE_DELETE(m_pInstance);
}

ThreadPool::~ThreadPool()
{
	m_IsShuttingDown = true;
	std::for_each(m_Workers.begin(), m_Workers.end(), [](std::thread& thread) { thread.join(); });
}

void ThreadPool::CreateWorkers()
{
	m_Workers.reserve(m_WorkerThreadsCount);
	for (size_t i{ 0 }; i < m_WorkerThreadsCount; ++i)
	{
		m_Workers.emplace_back([this]()
			{
				while (true)
				{
					Work work;

					{
						std::unique_lock<std::mutex> lock{ m_Mutex };
						m_ConditionalVariable.wait(lock, [this]() { return m_IsShuttingDown || !m_Work.empty(); });

						if (m_IsShuttingDown && m_Work.empty())
							break;

						if (!m_Work.empty())
						{
							work = std::move(m_Work.front());
							m_Work.pop();
						}
					}

					work();
				}
			});
	}
}

PLUGIN_API
void ThreadPool::AddWork(Work work)
{
	Logging::Log(LogType::INFORMATION, "Assigned new work for the threadpool");

	{
		std::unique_lock<std::mutex> lock{ m_Mutex };
		m_Work.push(std::move(work));
	}

	m_ConditionalVariable.notify_one();
}