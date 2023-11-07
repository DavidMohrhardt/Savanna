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

#include "Public/ISavannaConcurrency.h"
#include "Public/ISavannaJobs.hpp"

#include "ConcurrencyCapabilities.h"
#include "EngineThread.h"

#include <Types/Containers/Arrays/dynamic_array.h>
#include <Types/Containers/Arrays/fixed_array.h>
#include <Types/Containers/Queues/atomic_queue.h>
#include <Types/Locks/SpinLock.h>

#include <atomic>

namespace Savanna::Concurrency
{
    // TODO @David.Mohrhardt (2023/11/06): Make this a feature on the ThreadManager instead of it's own global manager.
    class JobManager : public GlobalManager<JobManager>
    {
    private:
        DEFINE_GLOBAL_MANAGER_FRIENDS_FOR(JobManager);
        friend class ThreadManager;
        friend class DependencyAwaiterJob;
        friend class DependentJobWrapper;
        friend class JobRunner;

    private:
        static JobResult ExecuteJobInternal(JobHandle handle);
        static void ProcessJobsInternal();
        static void ProcessJobsInternalWithReturn();
        static uint8 s_ThreadPoolSize;

        se_ThreadHandle_t m_PrimaryJobThreadHandle;

        std::atomic_bool m_ProcessingJobs;

        atomic_queue<JobHandle> m_LowPriorityJobs;
        atomic_queue<JobHandle> m_NormalPriorityJobs;
        atomic_queue<JobHandle> m_HighPriorityJobs;

    public:
        JobManager();
        ~JobManager();

    protected:
        virtual bool InitializeInternal() final;
        virtual void StartInternal() final;
        virtual void StopInternal() final;
        virtual void ShutdownInternal() final;

    public:
        JobHandle ScheduleJob(
            IJob* job,
            JobPriority priority = JobPriority::k_SavannaJobPriorityNormal,
            JobHandle dependency = k_InvalidJobHandle);

        void ScheduleJob(
            JobHandle& handle, JobPriority priority = JobPriority::k_SavannaJobPriorityNormal);

        // TODO @david.mohrhardt: Fix this. Currently it can cause issues because there is no user client contract
        // for who owns the memory of a batch job.
        // JobHandle ScheduleJobBatch(IJob** pJobs, const size& jobCount, JobPriority priority = JobPriority::k_SavannaJobPriorityNormal, JobHandle dependency = k_InvalidJobHandle);

        void AwaitCompletion(JobHandle jobHandle);
        void AwaitCompletion(JobHandle *pJobHandles, size_t jobCount);

        bool TryCancelJob(JobHandle jobHandle) SAVANNA_NOEXCEPT;

        SAVANNA_NO_DISCARD JobState GetJobState(JobHandle jobHandle);

        JobHandle CombineDependencies(const JobHandle* handles, size_t jobCount);

        uint8 GetThreadPoolSize() const { return s_ThreadPoolSize; }

    private:
        JobResult AwaitJobOrExecuteImmediateInternal(JobHandle dependency);
    };
} // namespace Savanna::Concurrency
