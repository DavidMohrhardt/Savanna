/**
 * @file JobSystem.h
 * @author David Mohrhardt (https://github.com/DavidMohrhardt/Savanna)
 * @brief TODO @David.Mohrhardt Document
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

namespace savanna::concurrency
{
    // TODO @David.Mohrhardt (2023/11/06): Make this a feature on the ThreadManager instead of it's own global manager.
    class JobSystem
    {
    private:
        friend class ThreadManager;
        friend class DependencyAwaiterJob;
        friend class DependentJobWrapper;
        friend class JobRunner;

    private:
        static void ProcessJobsInternal(void* pArgs);
        static void ProcessJobsInternalWithReturn(void* pArgs);
        static uint8 s_ThreadPoolSize;

        se_ThreadHandle_t m_PrimaryJobThreadHandle;

        std::atomic_bool m_ProcessingJobs;

        atomic_queue<JobHandle> m_LowPriorityJobs;
        atomic_queue<JobHandle> m_NormalPriorityJobs;
        atomic_queue<JobHandle> m_HighPriorityJobs;

        JobSystem();
        ~JobSystem();

        JobResult ExecuteJobInternal(se_JobHandle_t handle);

        void Start(const se_ThreadHandle_t primaryThreadHandle);
        void Stop();

    public:
        JobHandle ScheduleJob(
            IJob* job,
            JobPriority priority = JobPriority::k_SavannaJobPriorityNormal,
            JobHandle dependency = k_InvalidJobHandle);

        se_JobHandle_t ScheduleJob(
            const se_JobDefinition_t& jobDefinition,
            JobPriority priority = JobPriority::k_SavannaJobPriorityNormal,
            JobHandle dependency = k_InvalidJobHandle);

        // TODO @david.mohrhardt: Fix this. Currently it can cause issues because there is no user client contract
        // for who owns the memory of a batch job.
        // JobHandle ScheduleJobBatch(IJob** pJobs, const size& jobCount, JobPriority priority = JobPriority::k_SavannaJobPriorityNormal, JobHandle dependency = k_InvalidJobHandle);

        void AwaitCompletion(se_JobHandle_t jobHandle);
        void AwaitCompletion(se_JobHandle_t* pJobHandles, size_t jobCount);

        bool TryCancelJob(se_JobHandle_t jobHandle) SAVANNA_NOEXCEPT;

        SAVANNA_NO_DISCARD JobState GetJobState(se_JobHandle_t jobHandle);

        JobHandle CombineDependencies(const JobHandle* handles, size_t jobCount);

        uint8 GetThreadPoolSize() const { return s_ThreadPoolSize; }

    private:
        void ScheduleJobInternal(
            JobHandle& handle,
            JobPriority priority);

        JobResult AwaitJobOrExecuteImmediateInternal(se_JobHandle_t dependency);
    };
} // namespace savanna::Concurrency
