#include "JobManager.h"

#include <mutex>

namespace Savanna::Concurrency
{
    static std::mutex g_JobMutex;

    JobManager::JobManager()
        : m_JobHandleCounter(0)
        , m_JobThreads()
        , m_HighPriorityJobs()
        , m_NormalPriorityJobs()
        , m_LowPriorityJobs()
    {}

    JobManager::~JobManager()
    {
        for (auto& jobThread : m_JobThreads)
        {
            jobThread.Stop();
        }

        for (auto& job : jobThread.GetJobs())
        {
            m_JobPool.Deallocate(job);
        }
    }

    JobHandle JobManager::AcquireJobHandle(const IJobInterface* pIJobInterface, JobPriority priority, JobHandle dependency)
    {
        if (pIJobInterface == nullptr)
        {
            return k_InvalidJobHandle;
        }

        JobHandle outHandle = k_InvalidJobHandle;

        if (dependency != k_InvalidJobHandle)
        {
            if (GetJobState(dependency) != JobState::Completed)
            {
                outHandle = m_JobPool.Allocate(DependentJob(pIJobInterface, dependency));
            }
            else
            {
                goto NO_ACTIVE_DEPENDENCY;
            }
        }
        else
        {
            // If there is no dependency, we can just allocate the job directly.
            NO_ACTIVE_DEPENDENCY:
            outHandle = m_JobPool.Allocate(BasicJob(pIJobInterface));
        }

        return outHandle;
    }

    void JobManager::ReleaseJobHandle(JobHandle jobHandle)
    {
        if (jobHandle != k_InvalidJobHandle)
        {
            m_JobPool.Deallocate(jobHandle);
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
        ScheduleJobBatch(&handle, 1, priority);
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

