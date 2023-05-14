#include "JobManager.h"

#include "DependentJob.h"

#include <mutex>

namespace Savanna::Concurrency
{
    static std::mutex s_JobQueueMutex;

    void JobManager::ProcessJobs()
    {
        SAVANNA_ASSERT(!ThreadManager::IsMainThread(), "JobManager::ProcessJobs() must not be called from the main thread.");

        while (m_ProcessingJobs.load())
        {
            JobHandle jobHandle = k_InvalidJobHandle;

            {
                std::lock_guard<std::mutex> lock(s_JobQueueMutex);

                if (!m_HighPriorityJobs.empty())
                {
                    jobHandle = m_HighPriorityJobs.front();
                    m_HighPriorityJobs.pop();
                }
                else if (!m_NormalPriorityJobs.empty())
                {
                    jobHandle = m_NormalPriorityJobs.front();
                    m_NormalPriorityJobs.pop();
                }
                else if (!m_LowPriorityJobs.empty())
                {
                    jobHandle = m_LowPriorityJobs.front();
                    m_LowPriorityJobs.pop();
                }

                // Check if the job has been removed from the queue
                if (jobHandle == k_InvalidJobHandle || m_JobHandles.find(jobHandle) == m_JobHandles.end())
                {
                    jobHandle = k_InvalidJobHandle;
                }
                else
                {
                    m_JobHandles[jobHandle] = k_SavannaJobStateRunning;
                }
            }

            if (jobHandle != k_InvalidJobHandle)
            {
                JobRunner runner = JobRunner(jobHandle);
                runner.Run();
            }

            std::this_thread::yield();
        }
    }

    JobManager::JobManager(uint8 threadPoolSize)
        : m_ThreadPoolSize(threadPoolSize), m_IsRunning(false), m_JobThreads(), m_HighPriorityJobs(), m_NormalPriorityJobs(), m_LowPriorityJobs()
    {
        SAVANNA_ASSERT(
            threadPoolSize > 0,
            "JobManager::JobManager: threadPoolSize must be greater than 0.");

        SAVANNA_ASSERT(
            threadPoolSize <= std::thread::hardware_concurrency - 1,
            "JobManager::JobManager: threadPoolSize must be less than the number of available cores - 1 for the main thread.");
    }

    JobManager::~JobManager()
    {
        for (auto& jobThread : m_JobThreads)
        {
            jobThread.Stop();
        }

        for (auto& job : jobThread.GetJobs())
        {
            MemoryManager::Get()->Deallocate(job);
        }
    }

    void JobManager::Start()
    {
        // If the atomic started bool is false, then we set it to true and spin up the threads.
        bool isNotStarted = m_ProcessingJobs.compare_exchange_weak(false, true, std::memory_order_release, std::memory_order_relaxed);
        if (isNotStarted)
        {
            m_JobThreads.resize(m_ThreadPoolSize);
            for (int i = 0; i < m_JobThreads.size(); ++i)
            {
                m_JobThreads[i] = std::thread(&ProcessJobs, &m_JobThreads[i]);
            }
        }
    }

    void JobManager::Stop(bool synchronized)
    {
        while (m_ProcessingJobs.compare_exchange_weak(true, false, std::memory_order_release, std::memory_order_relaxed))
        {
            for (const auto& thread : m_JobThreads)
            {
                if (synchronized)
                {
                    thread.join();
                }
                else
                {
                    thread.detach();
                }
            }
            m_JobThreads.clear();
        }
    }

    JobHandle JobManager::AcquireJobHandle(const IJobInterface* pIJobInterface)
    {
        if (pIJobInterface == nullptr) SAVANNA_BRANCH_UNLIKELY
        {
            return k_InvalidJobHandle;
        }

        JobHandle jobHandle = MemoryManager::Get()->Allocate<LowLevelJob>(pIJobInterface);
    }

    void JobManager::ReleaseJobHandle(JobHandle jobHandle)
    {
        if (jobHandle != k_InvalidJobHandle)
        {
            MemoryManager::Get()->Deallocate(jobHandle);
        }
    }

    /**
     * @brief Schedules a job to be executed by the job system.
     * The job pointer must remain valid until the job has finished
     * executing.
    */
    JobHandle JobManager::ScheduleJob(IJob *pJob, JobPriority priority, JobHandle dependency)
    {
        JobHandle handle = k_InvalidJobHandle;

        if (pJob != nullptr)
        {
            // since handles are opaque memory addresses, we can just use the job pointer as the handle.
            handle = reinterpret_cast<JobHandle>(pJob);
            ScheduleJob(handle, priority);
        }

        return handle;
    }

    void JobManager::ScheduleJob(JobHandle &handle, JobPriority priority)
    {
        std::lock_guard<std::mutex> lock(s_JobQueueMutex);
        switch (priority)
        {
        case JobPriority::High:
            m_HighPriorityJobs.push(handle);
            break;
        case JobPriority::Normal:
            m_NormalPriorityJobs.push(handle);
            break;
        case JobPriority::Low:
        default:
            m_LowPriorityJobs.push(handle);
            break;
        }
    }

    void JobManager::ScheduleJobBatch(JobHandle *handles, size_t jobCount, JobPriority priority)
    {
        std::lock_guard<std::mutex> lock(s_JobQueueMutex);

        switch (priority)
        {
        case JobPriority::High:
            for (size_t i = 0; i < jobCount; ++i)
            {
                m_HighPriorityJobs.push(handles[i]);
            }
            break;
        case JobPriority::Normal:
            for (size_t i = 0; i < jobCount; ++i)
            {
                m_NormalPriorityJobs.push(handles[i]);
            }
            break;
        case JobPriority::Low:
        default:
            for (size_t i = 0; i < jobCount; ++i)
            {
                m_LowPriorityJobs.push(handles[i]);
            }
            break;
        }
    }

    bool JobManager::TryCancelJob(JobHandle jobHandle)
    {
        std::lock_guard<std::mutex> lock(s_JobQueueMutex);
        auto it = m_JobHandles.find(jobHandle);
        if (it != m_JobHandles.end())
        {
            switch (it->second)
            {
            case k_SavannaJobStateReady:
                m_JobHandles.erase(it);
                return true;

            case k_SavannaJobStateRunning:
                if (static_cast<IJob*>(jobHandle)->Cancel())
                {
                    m_JobHandles.erase(it);
                    return true;
                }
                break;

            case k_SavannaJobStateCancelled:
                return true;

            case k_SavannaJobStateCompleted:
            default:
                break;
            }
        }
        return false;
    }

    void JobManager::AwaitCompletion(JobHandle jobHandle)
    {
        while (GetJobState(jobHandle) != JobState::Completed)
        {
            std::this_thread::yield();
        }
    }

    JobState JobManager::GetJobState(JobHandle jobHandle)
    {
        std::lock_guard<std::mutex> lock(s_JobQueueMutex);

        auto it = m_JobHandles.find(jobHandle);
        if (it != m_JobHandles.end())
        {
            return it->second;
        }
        return JobState::Invalid;
    }
} // namespace Savanna::Concurrency

