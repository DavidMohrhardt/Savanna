#include "JobManager.h"

#include "DependentJob.h"

#include <mutex>

namespace Savanna::Concurrency
{
    static std::mutex g_JobMutex;

    void JobManager::ProcessJobs()
    {
        SAVANNA_ASSERT(!ThreadManager::IsMainThread(), "JobManager::ProcessJobs() must not be called from the main thread.");

        while (m_ProcessingJobs.load())
        {
            JobHandle jobHandle = k_InvalidJobHandle;

            {
                std::lock_guard<std::mutex> lock(g_JobMutex);

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

                m_JobHandles[jobHandle] = k_SavannaJobStateRunning;
            }

            if (jobHandle != k_InvalidJobHandle)
            {
                IJob* pJob = reinterpret_cast<IJob*>(jobHandle.GetJobHandle());
                JobResult result = pJob->Execute();

                JobState state = k_SavannaJobStateCompleted;
                switch (result)
                {
                case JobResult::k_Success:
                    pJob->OnJobSucceeded();
                    break;
                case JobResult::k_Failure:
                    pJob->OnJobFailed();
                    break;
                case JobResult::k_Cancelled:
                    pJob->OnJobCancelled();
                    state = k_SavannaJobStateCancelled;
                    break;
                }

                {
                    std::lock_guard<std::mutex> lock(g_JobMutex);
                    m_JobHandles[jobHandle] = state;
                }

                pJob->OnJobFinished(result);
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

    JobHandle JobManager::AcquireJobHandle(const IJobInterface* pIJobInterface, JobPriority priority, JobHandle dependency)
    {
        if (pIJobInterface != nullptr)
        {
            return k_InvalidJobHandle;
        }
        else if (dependency != k_InvalidJobHandle && GetJobState(dependency) != JobState::Completed)
        {
            DependentJob<BasicJob>* pJob = MemoryManager::Get()->Allocate<DependentJob<BasicJob>>(1);
            pJob->DependentJob<BasicJob>(dependency, BasicJob(pIJobInterface));
            return reinterpret_cast<JobHandle>(pJob);
        }
        else
        {
            BasicJob* pJob = MemoryManager::Get()->Allocate<BasicJob>(1);
            pJob->BasicJob(pIJobInterface);
            return reinterpret_cast<JobHandle>(pJob);
        }
    }

    void JobManager::ReleaseJobHandle(JobHandle jobHandle)
    {
        if (jobHandle != k_InvalidJobHandle)
        {
            MemoryManager::Get()->Deallocate(jobHandle);
        }
    }

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
        std::lock_guard<std::mutex> lock(g_JobMutex);
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

    void JobManager::ScheduleJobBatch(JobHandle *handles, size_t jobCount, JobPriority priority)
    {
        std::lock_guard<std::mutex> lock(g_JobMutex);

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
        std::lock_guard<std::mutex> lock(g_JobMutex);
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
            case k_SavannaJobStateCompleted:
            case k_SavannaJobStateCancelled:
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
        std::lock_guard<std::mutex> lock(g_JobMutex);

        auto it = m_JobHandles.find(jobHandle);
        if (it != m_JobHandles.end())
        {
            return it->second;
        }
        return JobState::Invalid;
    }
} // namespace Savanna::Concurrency

