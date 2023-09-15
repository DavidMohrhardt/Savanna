/**
 * @file JobSystem.h
 * @author David Mohrhardt (https://github.com/DavidMohrhardt/Savanna)
 * @brief
 * @version 0.1
 * @date 2023-02-02
 *
 * @copyright Copyright (c) 2023
 *
 */
#pragma once

#define USE_LOCKLESS_CONCURRENCY_STRUCTURES 1

#include <SavannaEngine.h>
#include <Utilities/SavannaCoding.h>

#include "Types/Manager/GlobalManager.h"

#include "ISavannaJobs.h"

#include "ConcurrencyCapabilities.h"

#include "Types/Containers/Concurrent/LocklessQueue.h"
#include "Types/Locks/SpinLock.h"

#include <atomic>
#include <unordered_map>
#include <vector>

namespace Savanna::Concurrency
{
    class JobManager final : public GlobalManager<JobManager>
    {
    private:
        DEFINE_GLOBAL_MANAGER_FRIENDS_FOR(JobManager);
        friend class DependencyAwaiterJob;
        friend class DependentJobWrapper;
        friend class JobRunner;

    public:
        static uint8 k_MaxThreadPoolSize;

    private:
        static void ProcessJobs();

        uint8 m_ThreadPoolSize;
        std::atomic_bool m_ProcessingJobs;
        std::vector<std::thread> m_JobThreads;

        LocklessQueue<JobHandle> m_LowPriorityJobs;
        LocklessQueue<JobHandle> m_NormalPriorityJobs;
        LocklessQueue<JobHandle> m_HighPriorityJobs;

        SpinLock m_JobHandlesLock;
        std::unordered_map<JobHandle, JobState> m_JobHandles;

    public:
        JobManager();
        ~JobManager();

    protected:
        virtual bool InitializeInternal() final override;
        virtual void StartInternal() final override;
        virtual void StopInternal() final override;
        virtual void ShutdownInternal() final override;

    public:
        JobHandle ScheduleJob(
            IJob* job,
            JobPriority priority = JobPriority::k_SavannaJobPriorityNormal,
            JobHandle dependency = k_InvalidJobHandle);

        void ScheduleJob(JobHandle& handle, JobPriority priority = JobPriority::k_SavannaJobPriorityNormal);
        JobHandle ScheduleJobBatch(IJob** pJobs, const size& jobCount, JobPriority priority = JobPriority::k_SavannaJobPriorityNormal, JobHandle dependency = k_InvalidJobHandle);

        void AwaitCompletion(JobHandle jobHandle);
        void AwaitCompletion(JobHandle *pJobHandles, size_t jobCount);

        bool TryCancelJob(JobHandle jobHandle) SAVANNA_NOEXCEPT;

        SAVANNA_NO_DISCARD JobState GetJobState(JobHandle jobHandle);

        JobHandle CombineDependencies(const JobHandle* handles, size_t jobCount);

    private:
        JobResult AwaitJobOrExecuteImmediateInternal(JobHandle dependency);

        void SetJobState(JobHandle handle, JobState state);

        void OnJobCompletedInternal(JobHandle handle);
    };
} // namespace Savanna::Concurrency
