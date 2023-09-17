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
#define MAX_CONCURRENT_JOBS 1024

#include <SavannaEngine.h>
#include <Utilities/SavannaCoding.h>

#include "Types/Manager/GlobalManager.h"

#include "ISavannaJobs.h"

#include "ConcurrencyCapabilities.h"

#include "Types/Containers/Arrays/DynamicArray.h"
#include "Types/Containers/Concurrent/LocklessQueue.h"
#include "Types/Locks/SpinLock.h"

#include <atomic>

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
      static JobResult ExecuteJobInternal(JobHandle handle);
      static void ProcessJobsInternal();

      uint8 m_ThreadPoolSize;
      std::atomic_bool m_ProcessingJobs;
      DynamicArray<std::thread> m_JobThreads;

      LocklessQueue<JobHandle> m_LowPriorityJobs;
      LocklessQueue<JobHandle> m_NormalPriorityJobs;
      LocklessQueue<JobHandle> m_HighPriorityJobs;

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
        // TODO @david.mohrhardt: Fix this. Currently it can cause issues because there is no user client contract
        // for who owns the memory of a batch job.
        // JobHandle ScheduleJobBatch(IJob** pJobs, const size& jobCount, JobPriority priority = JobPriority::k_SavannaJobPriorityNormal, JobHandle dependency = k_InvalidJobHandle);

        void AwaitCompletion(JobHandle jobHandle);
        void AwaitCompletion(JobHandle *pJobHandles, size_t jobCount);

        bool TryCancelJob(JobHandle jobHandle) SAVANNA_NOEXCEPT;

        SAVANNA_NO_DISCARD JobState GetJobState(JobHandle jobHandle);

        JobHandle CombineDependencies(const JobHandle* handles, size_t jobCount);

    private:
        JobResult AwaitJobOrExecuteImmediateInternal(JobHandle dependency);
    };
} // namespace Savanna::Concurrency
