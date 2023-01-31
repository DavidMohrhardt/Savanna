/**
 * @file JobSystem.cpp
 * @author David Mohrhardt (https://github.com/DavidMohrhardt/Savanna)
 * @brief
 * @version 0.1
 * @date 2023-01-19
 *
 * @copyright Copyright (c) 2023
 *
 */
#include "JobSystem.h"

#include <mutex>

namespace Savanna::Concurrency
{
    std::thread::id JobSystem::s_MainThreadID;

    JobSystem::JobSystem()
        : m_JobThreads(k_MaxJobThreadCount)
        , m_JobHandleCounter(0)
        , m_Started(false)
    {
        const auto _ = []() -> int { s_MainThreadID = std::this_thread::get_id(); return 0; }();
    }

    JobSystem::JobSystem(size_t threadCount)
        : JobSystem()
        , m_JobThreads(threadCount)
        , m_JobHandleCounter(0)
        , m_Started(false)
    {
        assert(s_MainThreadID == std::this_thread::get_id());
        assert(threadCount <= k_MaxJobThreadCount);
        assert(threadCount > 0);
    }

    JobSystem::~JobSystem()
    {
        Stop();
    }

    void JobSystem::Start()
    {
        assert(s_MainThreadID == std::this_thread::get_id());
        if (!m_Started.compare_exchange_strong(false, true))
        {
            for (size_t i = 0; i < m_JobThreads.size(); ++i)
            {
                m_JobThreads[i] = JobThread(AwaitJobs);
                m_JobThreads[i].Start();
            }
        }
    }

    void JobSystem::Stop()
    {
        assert(s_MainThreadID == std::this_thread::get_id());
        if (m_Started.compare_exchange_strong(true, false))
        {
            for (size_t i = 0; i < m_JobThreads.size(); ++i)
            {
                m_JobThreads[i].SignalStopAndDetach();
            }
        }
    }

    void JobSystem::RegisterJob(std::shared_ptr<IJob> job, JobPriority priority = JobPriority::Normal)
    {
        JobHandle handle = JobHandle(m_JobHandleCounter.fetch_add(1));
        job->SetJobHandle(handle);

        std::lock_guard<std::mutex> lock(m_JobMapMutex);
        m_JobMap[handle] = job;

        switch (priority)
        {
        case JobPriority::Low:
            m_LowPriorityJobs.push(handle);
            break;
        case JobPriority::High:
            m_HighPriorityJobs.push(handle);
            break;
        case JobPriority::Normal:
        default:
            m_NormalPriorityJobs.push(handle);
            break;
        }
    }

    std::shared_ptr<IJob> JobSystem::PollJobs()
    {
        std::lock_guard<std::mutex> lock(m_JobMapMutex);
        if (!m_HighPriorityJobs.empty())
        {
            JobHandle handle = m_HighPriorityJobs.front();
            m_HighPriorityJobs.pop();
            return m_JobMap[handle];
        }
        else if (!m_NormalPriorityJobs.empty())
        {
            JobHandle handle = m_NormalPriorityJobs.front();
            m_NormalPriorityJobs.pop();
            return m_JobMap[handle];
        }
        else if (!m_LowPriorityJobs.empty())
        {
            JobHandle handle = m_LowPriorityJobs.front();
            m_LowPriorityJobs.pop();
            return m_JobMap[handle];
        }
        else
        {
            return std::shared_ptr<IJob>();
        }
    }
} // namespace Savanna::Concurrency
