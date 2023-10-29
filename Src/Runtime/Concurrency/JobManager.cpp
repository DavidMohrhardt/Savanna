#include "JobManager.h"
#include "SavannaConcurrency.h"

#include "DependencyJobs.h"

#include "Types/Containers/Arrays/dynamic_array.h"

namespace Savanna::Concurrency
{
    uint8 JobManager::s_ThreadPoolSize = std::thread::hardware_concurrency() - 1 < s_ThreadPoolSize
            ? std::thread::hardware_concurrency() - 1
            : s_ThreadPoolSize;

    inline JobResult JobManager::ExecuteJobInternal(JobHandle handle)
    {
        if (handle == k_InvalidJobHandle) SAVANNA_BRANCH_UNLIKELY
            return k_SavannaJobResultError;

        IJob* pJob = reinterpret_cast<IJob*>(handle);

        JobHandle dependency = pJob->GetDependency();
        if (dependency != k_InvalidJobHandle)
        {
            Get()->AwaitJobOrExecuteImmediateInternal(dependency);
        }

        JobResult result = pJob->Execute();
        switch (result)
        {
            case k_SavannaJobResultSuccess:
                pJob->OnComplete();
                break;
            case k_SavannaJobResultCancelled:
                pJob->OnCancel();
                break;
            case k_SavannaJobResultError:
            default:
                pJob->OnError();
                break;
        }
        pJob->m_JobState.store(k_SavannaJobStateFinished, std::memory_order_release);
        return result;
    }

    void JobManager::ProcessJobsInternal()
    {
        JobManager* pManager = Get();
        while (pManager->m_ProcessingJobs.load())
        {
            JobHandle jobHandle = k_InvalidJobHandle;

            uint32 expectedState = k_SavannaJobStateReady;
            uint32 desiredState = k_SavannaJobStateRunning;

                // In the current implementation of the lockless queue you can simply pop from the queue
            if (pManager->m_HighPriorityJobs.TryDequeue(jobHandle) ||
                pManager->m_NormalPriorityJobs.TryDequeue(jobHandle) ||
                pManager->m_LowPriorityJobs.TryDequeue(jobHandle))
            {
                if (reinterpret_cast<IJob*>(jobHandle)->m_JobState.compare_exchange_weak(expectedState, desiredState, std::memory_order_release, std::memory_order_relaxed))
                {
                    ExecuteJobInternal(jobHandle);
                }
            }

            std::this_thread::yield();
        }
    }

    JobManager::JobManager()
        : m_ProcessingJobs(false)
        , m_JobThreads()
        , m_HighPriorityJobs()
        , m_NormalPriorityJobs()
        , m_LowPriorityJobs()
    {}

    JobManager::~JobManager() {}

    bool JobManager::InitializeInternal()
    {
        SAVANNA_INSERT_SCOPED_PROFILER(JobManager::InitializeInternal());
        Info::Initialize();
        return true;
    }

    void JobManager::StartInternal()
    {
        SAVANNA_INSERT_SCOPED_PROFILER(JobManager::StartInternal());
        // If the atomic started bool is false, then we set it to true and spin up the threads.
        bool expected = false;
        if (m_ProcessingJobs.compare_exchange_weak(expected, true, std::memory_order_release))
        {
            m_JobThreads.clear();
            for (int i = 0; i < s_ThreadPoolSize; ++i)
            {
                m_JobThreads.push_back(std::move(std::thread(ProcessJobsInternal)));
            }
        }
    }

    void JobManager::StopInternal()
    {
        SAVANNA_INSERT_SCOPED_PROFILER(JobManager::StopInternal());
        while (m_ProcessingJobs.exchange(false, std::memory_order_release))
        {
            for (int i = 0; i < m_JobThreads.size(); ++i)
            {
                m_JobThreads[i].join();
            }
            m_JobThreads.clear();
        }
    }

    void JobManager::ShutdownInternal()
    {
        SAVANNA_INSERT_SCOPED_PROFILER(JobManager::ShutdownInternal());
        StopInternal();
        Info::Reset();
    }

    /**
     * @brief Schedules a job to be executed by the job system.
     * The job pointer must remain valid until the job has finished
     * executing.
    */
    JobHandle JobManager::ScheduleJob(IJob *pJob, JobPriority priority, JobHandle dependency)
    {
        SAVANNA_INSERT_SCOPED_PROFILER(JobManager::ScheduleJob(IJob *pJob, JobPriority priority, JobHandle dependency));
        JobHandle handle = k_InvalidJobHandle;

        if (pJob != nullptr) SAVANNA_BRANCH_LIKELY
        {
            if (dependency != k_InvalidJobHandle)
            {
                pJob->SetDependency(dependency);
            }

            // since handles are opaque memory addresses, we can just use the job pointer as the handle.
            handle = reinterpret_cast<JobHandle>(pJob);
            ScheduleJob(handle, priority);
        }

        return handle;
    }

    inline void JobManager::ScheduleJob(JobHandle &handle, JobPriority priority)
    {
        if (handle == k_InvalidJobHandle) SAVANNA_BRANCH_UNLIKELY
            return;

        SAVANNA_INSERT_SCOPED_PROFILER(JobManager::ScheduleJob(JobHandle &handle, JobPriority priority));

        uint32 expectedState = k_SavannaJobStateInvalid;
        uint32 desiredState = k_SavannaJobStateReady;
        if (!reinterpret_cast<IJob*>(handle)->m_JobState.compare_exchange_weak(expectedState, desiredState, std::memory_order_release, std::memory_order_relaxed))
        {
            return;
        }

        auto& jobQueue = priority == k_SavannaJobPriorityHigh ? m_HighPriorityJobs
            : priority == k_SavannaJobPriorityNormal ? m_NormalPriorityJobs
            : m_LowPriorityJobs;

        jobQueue.Enqueue(std::move(handle));
    }

    // See TODO in JobManager.h
    // JobHandle JobManager::ScheduleJobBatch(IJob** pJobs, const size& jobCount, JobPriority priority, JobHandle dependency)
    // {
    //     if (pJobs == nullptr || jobCount == 0) SAVANNA_BRANCH_UNLIKELY
    //         return k_InvalidJobHandle;

    //     SAVANNA_INSERT_SCOPED_PROFILER(JobManager::ScheduleJobBatch(IJob* pJobs, const size& jobCount, JobPriority priority, JobHandle dependency));

    //     JobHandle outJobHandle = k_InvalidJobHandle;

    //     dynamic_array<JobHandle> handles(jobCount);
    //     for (size_t i = 0; i < jobCount; ++i)
    //     {
    //         handles.Append(ScheduleJob(pJobs[i], priority, dependency));
    //     }

    //     return ScheduleJob(SAVANNA_NEW(DependencyAwaiterJob, handles.data(), handles.Size()), priority);
    // }

    void JobManager::AwaitCompletion(JobHandle jobHandle)
    {
        if (jobHandle == k_InvalidJobHandle) SAVANNA_BRANCH_UNLIKELY
            return;

        SAVANNA_INSERT_SCOPED_PROFILER(JobManager::AwaitCompletion(JobHandle jobHandle));

        while (true)
        {
            auto state = reinterpret_cast<IJob*>(jobHandle)->m_JobState.load(std::memory_order_relaxed);
            if (state == k_SavannaJobStateFinished || state == k_SavannaJobStateInvalid)
            {
                return;
            }

            std::this_thread::yield();
        }
    }

    void JobManager::AwaitCompletion(JobHandle* pJobHandles, size_t jobCount)
    {
        JobHandle combinedJobHandles = CombineDependencies(pJobHandles, jobCount);
        AwaitCompletion(combinedJobHandles);
    }

    bool JobManager::TryCancelJob(JobHandle jobHandle) noexcept
    {
        if (jobHandle == k_InvalidJobHandle) SAVANNA_BRANCH_UNLIKELY
            return false;

        SAVANNA_INSERT_SCOPED_PROFILER(JobManager::TryCancelJob(JobHandle jobHandle));
        return reinterpret_cast<IJob*>(jobHandle)->TryCancel();
    }

    JobState JobManager::GetJobState(JobHandle jobHandle)
    {
        if (jobHandle == k_InvalidJobHandle) SAVANNA_BRANCH_UNLIKELY
            return k_SavannaJobStateInvalid;

        SAVANNA_INSERT_SCOPED_PROFILER(JobManager::GetJobState(JobHandle jobHandle));

        return reinterpret_cast<IJob*>(jobHandle)->GetState();
    }

    JobHandle JobManager::CombineDependencies(
        const JobHandle *handles,
        size_t jobCount)
    {
        SAVANNA_INSERT_SCOPED_PROFILER(JobManager::CombineDependencies(JobHandle *handles, size_t jobCount));
        if (jobCount == 0 || handles == nullptr)
        {
            return k_InvalidJobHandle;
        }

        return ScheduleJob(SAVANNA_NEW(k_SavannaMemoryLabelGeneral, DependencyAwaiterJob, handles, jobCount), k_SavannaJobPriorityHigh);
    }

    void JobManager::SetThreadPoolSize(uint8 threadPoolSize)
    {
        if (!IsInitialized())
        {
            s_ThreadPoolSize = threadPoolSize;
        }
    }

    JobResult JobManager::AwaitJobOrExecuteImmediateInternal(JobHandle dependency)
    {
        SAVANNA_INSERT_SCOPED_PROFILER(JobManager::AwaitJobOrExecuteImmediateInternal(JobHandle dependency));
        if (dependency == k_InvalidJobHandle) SAVANNA_BRANCH_UNLIKELY
            return k_SavannaJobResultError;

        JobHandle handle = k_InvalidJobHandle;

        uint32 expectedState = k_SavannaJobStateReady;
        uint32 desiredState = k_SavannaJobStateRunning;
        if (reinterpret_cast<IJob*>(dependency)->m_JobState.compare_exchange_weak(expectedState, desiredState, std::memory_order_release, std::memory_order_relaxed))
        {
            handle = dependency;
        }

        if (handle != k_InvalidJobHandle)
        {
            return ExecuteJobInternal(handle);
        }
        else
        {
            AwaitCompletion(dependency);
            return k_SavannaJobResultSuccess;
        }
    }
} // namespace Savanna::Concurrency
