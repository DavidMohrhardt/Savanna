#include "JobSystem.h"
#include "SavannaConcurrency.h"

#include "DependencyJobs.h"
#include "EngineThread.h"
#include "ThreadManager.h"

#include "Types/Containers/Arrays/dynamic_array.h"

namespace savanna::concurrency
{
    inline JobResult JobSystem::ExecuteJobInternal(seJobHandle handle)
    {
        if (handle == k_InvalidJobHandle) SAVANNA_BRANCH_UNLIKELY
            return k_SavannaJobResultError;

        IJob* pJob = reinterpret_cast<IJob*>(handle);

        JobHandle dependency = pJob->GetDependency();
        if (dependency != k_InvalidJobHandle)
        {
            AwaitJobOrExecuteImmediateInternal(dependency);
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

    void JobSystem::ProcessJobsInternal(void* pArgs)
    {
        JobSystem* pJobSystem = reinterpret_cast<JobSystem*>(pArgs);
        while (pJobSystem->m_ProcessingJobs.load())
        {
            JobHandle jobHandle = k_InvalidJobHandle;

            uint32 expectedState = k_SavannaJobStateReady;
            uint32 desiredState = k_SavannaJobStateRunning;

                // In the current implementation of the lockless queue you can simply pop from the queue
            if (pJobSystem->m_HighPriorityJobs.TryDequeue(jobHandle) ||
                pJobSystem->m_NormalPriorityJobs.TryDequeue(jobHandle) ||
                pJobSystem->m_LowPriorityJobs.TryDequeue(jobHandle))
            {
                if (reinterpret_cast<IJob*>(jobHandle)->m_JobState.compare_exchange_weak(expectedState, desiredState, std::memory_order_release, std::memory_order_relaxed))
                {
                    pJobSystem->ExecuteJobInternal(jobHandle);
                }
            }

            std::this_thread::yield();
        }
    }

    void JobSystem::ProcessJobsInternalWithReturn(void* pArgs)
    {
        JobSystem* pJobSystem = reinterpret_cast<JobSystem*>(pArgs);
        if (pJobSystem->m_ProcessingJobs.load())
        {
            JobHandle jobHandle = k_InvalidJobHandle;

            uint32 expectedState = k_SavannaJobStateReady;
            uint32 desiredState = k_SavannaJobStateRunning;

                // In the current implementation of the lockless queue you can simply pop from the queue
            if (pJobSystem->m_HighPriorityJobs.TryDequeue(jobHandle) ||
                pJobSystem->m_NormalPriorityJobs.TryDequeue(jobHandle) ||
                pJobSystem->m_LowPriorityJobs.TryDequeue(jobHandle))
            {
                if (reinterpret_cast<IJob*>(jobHandle)->m_JobState.compare_exchange_weak(expectedState, desiredState, std::memory_order_release, std::memory_order_relaxed))
                {
                    pJobSystem->ExecuteJobInternal(jobHandle);
                }
            }

            std::this_thread::yield();
        }
    }

    JobSystem::JobSystem()
        : m_ProcessingJobs(false)
        , m_HighPriorityJobs()
        , m_NormalPriorityJobs()
        , m_LowPriorityJobs()
    {}

    JobSystem::~JobSystem() {}

    void JobSystem::Start(const seThreadHandle primaryThreadHandle)
    {
        SAVANNA_INSERT_SCOPED_PROFILER(JobSystem::StartInternal());

        // This is the interface for the primary job thread. It will loop until the atomic bool is set to false.
        static ThreadExecutionInterface k_PrimaryThreadJobProcessingInterface =
        {
            reinterpret_cast<pfn_seThreadTask>(ProcessJobsInternal),
            this
        };

        // This is the interface for the unreserved threads. Allows them to check if there is work to do
        // while they await reservation.
        static ThreadExecutionInterface k_UnreservedThreadJobProcessingInterface =
        {
            reinterpret_cast<pfn_seThreadTask>(ProcessJobsInternalWithReturn),
            this
        };

        // If the atomic started bool is false, then we set it to true and spin up the threads.
        bool expected = false;
        if (m_ProcessingJobs.compare_exchange_weak(expected, true, std::memory_order_release))
        {

            if (!ThreadManager::Get()->TryAcquireThreads(1, &m_PrimaryJobThreadHandle))
            {
                SAVANNA_WARNING_LOG("Failed to acquire primary job thread.");
                return;
            }

            // For the primary job thread we want to set the execution interface to a looping function with it's own yield.
            ThreadManager::Get()->SetThreadExecutionInterface(1, &m_PrimaryJobThreadHandle, &k_PrimaryThreadJobProcessingInterface);
            ThreadManager::Get()->StartThreads(1, &m_PrimaryJobThreadHandle);

            // Then set the unreserved threads to also process jobs while they await reservation.
            ThreadManager::Get()->SetUnreservedThreadDefaultExecution(&k_UnreservedThreadJobProcessingInterface);
        }
    }

    void JobSystem::Stop()
    {
        SAVANNA_INSERT_SCOPED_PROFILER(JobSystem::Stop());
        bool expected = true;
        if (m_ProcessingJobs.compare_exchange_strong(expected, false, std::memory_order_acq_rel))
        {
            ThreadManager::Get()->SetUnreservedThreadDefaultExecution(nullptr);
            ThreadManager::Get()->ReleaseThreads(1, &m_PrimaryJobThreadHandle);
        }
    }

    /**
     * @brief Schedules a job to be executed by the job system.
     * The job pointer must remain valid until the job has finished
     * executing.
    */
    JobHandle JobSystem::ScheduleJob(IJob *pJob, JobPriority priority, JobHandle dependency)
    {
        SAVANNA_INSERT_SCOPED_PROFILER(JobSystem::ScheduleJob(IJob *pJob, JobPriority priority, JobHandle dependency));
        JobHandle handle = k_InvalidJobHandle;

        if (pJob != nullptr) SAVANNA_BRANCH_LIKELY
        {
            if (dependency != k_InvalidJobHandle)
            {
                pJob->SetDependency(dependency);
            }

            // since handles are opaque memory addresses, we can just use the job pointer as the handle.
            handle = reinterpret_cast<JobHandle>(pJob);
            ScheduleJobInternal(handle, priority);
        }

        return handle;
    }

    seJobHandle JobSystem::ScheduleJob(
        const seJobDefinition &jobDefinition,
        JobPriority priority,
        JobHandle dependency)
    {
        SAVANNA_INSERT_SCOPED_PROFILER(JobSystem::ScheduleJob(const seJobDefinition&, JobPriority, JobHandle));
        JobHandle handle = reinterpret_cast<JobHandle>(
            SAVANNA_NEW(kSavannaAllocatorKindGeneral, AutoDisposePrimitiveJob, jobDefinition, kSavannaAllocatorKindGeneral));

        ScheduleJobInternal(handle, priority);
        return handle;
    }

    inline void JobSystem::ScheduleJobInternal(JobHandle& handle, JobPriority priority)
    {
        if (handle == k_InvalidJobHandle) SAVANNA_BRANCH_UNLIKELY
            return;

        SAVANNA_INSERT_SCOPED_PROFILER(JobSystem::ScheduleJob(seJobHandle &handle, JobPriority priority));

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

    // See TODO in JobSystem.h
    // JobHandle JobSystem::ScheduleJobBatch(IJob** pJobs, const size& jobCount, JobPriority priority, JobHandle dependency)
    // {
    //     if (pJobs == nullptr || jobCount == 0) SAVANNA_BRANCH_UNLIKELY
    //         return k_InvalidJobHandle;

    //     SAVANNA_INSERT_SCOPED_PROFILER(JobSystem::ScheduleJobBatch(IJob* pJobs, const size& jobCount, JobPriority priority, JobHandle dependency));

    //     JobHandle outJobHandle = k_InvalidJobHandle;

    //     dynamic_array<JobHandle> handles(jobCount);
    //     for (size_t i = 0; i < jobCount; ++i)
    //     {
    //         handles.Append(ScheduleJob(pJobs[i], priority, dependency));
    //     }

    //     return ScheduleJob(SAVANNA_NEW(DependencyAwaiterJob, handles.data(), handles.Size()), priority);
    // }

    void JobSystem::AwaitCompletion(seJobHandle jobHandle)
    {
        if (jobHandle == k_InvalidJobHandle) SAVANNA_BRANCH_UNLIKELY
            return;

        SAVANNA_INSERT_SCOPED_PROFILER(JobSystem::AwaitCompletion(seJobHandle jobHandle));

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

    void JobSystem::AwaitCompletion(seJobHandle* pJobHandles, size_t jobCount)
    {
        JobHandle combinedJobHandles = CombineDependencies(pJobHandles, jobCount);
        AwaitCompletion(combinedJobHandles);
    }

    bool JobSystem::TryCancelJob(seJobHandle jobHandle) noexcept
    {
        if (jobHandle == k_InvalidJobHandle) SAVANNA_BRANCH_UNLIKELY
            return false;

        SAVANNA_INSERT_SCOPED_PROFILER(JobSystem::TryCancelJob(seJobHandle jobHandle));
        return reinterpret_cast<IJob*>(jobHandle)->TryCancel();
    }

    JobState JobSystem::GetJobState(seJobHandle jobHandle)
    {
        if (jobHandle == k_InvalidJobHandle) SAVANNA_BRANCH_UNLIKELY
            return k_SavannaJobStateInvalid;

        SAVANNA_INSERT_SCOPED_PROFILER(JobSystem::GetJobState(seJobHandle jobHandle));

        return reinterpret_cast<IJob*>(jobHandle)->GetState();
    }

    JobHandle JobSystem::CombineDependencies(
        const JobHandle *handles,
        size_t jobCount)
    {
        SAVANNA_INSERT_SCOPED_PROFILER(JobSystem::CombineDependencies(seJobHandle *handles, size_t jobCount));
        if (jobCount == 0 || handles == nullptr)
        {
            return k_InvalidJobHandle;
        }

        return ScheduleJob(SAVANNA_NEW(kSavannaAllocatorKindGeneral, DependencyAwaiterJob, handles, jobCount), k_SavannaJobPriorityHigh);
    }

    JobResult JobSystem::AwaitJobOrExecuteImmediateInternal(seJobHandle dependency)
    {
        SAVANNA_INSERT_SCOPED_PROFILER(JobSystem::AwaitJobOrExecuteImmediateInternal(seJobHandle dependency));
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
} // namespace savanna::Concurrency
