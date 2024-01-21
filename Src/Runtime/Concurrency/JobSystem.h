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

        seThreadHandle m_PrimaryJobThreadHandle;

        std::atomic_bool m_ProcessingJobs;

        atomic_queue<JobHandle> m_LowPriorityJobs;
        atomic_queue<JobHandle> m_NormalPriorityJobs;
        atomic_queue<JobHandle> m_HighPriorityJobs;

        JobSystem();
        ~JobSystem();

        JobResult ExecuteJobInternal(seJobHandle handle);

        void Start(const seThreadHandle primaryThreadHandle);
        void Stop();

    public:
        JobHandle ScheduleJob(
            IJob* job,
            JobPriority priority = JobPriority::k_SavannaJobPriorityNormal,
            JobHandle dependency = k_InvalidJobHandle);

        seJobHandle ScheduleJob(
            const seJobDefinition& jobDefinition,
            JobPriority priority = JobPriority::k_SavannaJobPriorityNormal,
            JobHandle dependency = k_InvalidJobHandle);

        // TODO @david.mohrhardt: Fix this. Currently it can cause issues because there is no user client contract
        // for who owns the memory of a batch job.
        // JobHandle ScheduleJobBatch(IJob** pJobs, const size& jobCount, JobPriority priority = JobPriority::k_SavannaJobPriorityNormal, JobHandle dependency = k_InvalidJobHandle);

        void AwaitCompletion(seJobHandle jobHandle);
        void AwaitCompletion(seJobHandle* pJobHandles, size_t jobCount);

        bool TryCancelJob(seJobHandle jobHandle) SAVANNA_NOEXCEPT;

        SAVANNA_NO_DISCARD JobState GetJobState(seJobHandle jobHandle);

        JobHandle CombineDependencies(const JobHandle* handles, size_t jobCount);

        uint8 GetThreadPoolSize() const { return s_ThreadPoolSize; }

    private:
        void ScheduleJobInternal(
            JobHandle& handle,
            JobPriority priority);

        JobResult AwaitJobOrExecuteImmediateInternal(seJobHandle dependency);
    };
} // namespace savanna::Concurrency
