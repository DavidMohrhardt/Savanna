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

#include <SavannaEngine.h>
#include <Utilities/SavannaCoding.h>
#include <Types/Singleton/Singleton.h>

#include "ISavannaJobs.h"

#include "JobRunner.h"
#include "ConcurrencyCapabilities.h"

#include <atomic>
#include <queue>
#include <unordered_map>
#include <vector>

namespace Savanna::Concurrency
{
    class JobManager : public Singleton<JobManager>
    {
    private:
        friend class Singleton<JobManager>;
        friend class DependencyAwaiterJob;
        friend class JobRunner;

    public:
        static uint8 k_MaxThreadPoolSize;

    private:
        static void ProcessJobs();

        uint8 m_ThreadPoolSize;
        std::atomic_bool m_ProcessingJobs;
        std::vector<std::thread> m_JobThreads;

        std::queue<JobHandle> m_LowPriorityJobs;
        std::queue<JobHandle> m_NormalPriorityJobs;
        std::queue<JobHandle> m_HighPriorityJobs;

        std::unordered_map<JobHandle, JobState> m_JobHandles;

        JobManager(uint8 threadPoolSize);
        ~JobManager();

    public:
        void Start();
        void Stop(bool synchronized = false);

        JobHandle ScheduleJob(
            IJob* job,
            JobPriority priority = JobPriority::k_SavannaJobPriorityNormal,
            JobHandle dependency = k_InvalidJobHandle);

        void ScheduleJob(JobHandle& handle, JobPriority priority = JobPriority::k_SavannaJobPriorityNormal);
        void ScheduleJobBatch(JobHandle* handles, size_t jobCount, JobPriority priority = JobPriority::k_SavannaJobPriorityNormal);

        void AwaitCompletion(JobHandle jobHandle);
        bool TryCancelJob(JobHandle jobHandle) SAVANNA_NOEXCEPT;

        SAVANNA_NO_DISCARD JobState GetJobState(JobHandle jobHandle);

        JobHandle CombineDependencies(JobHandle* handles, size_t jobCount);

    private:
        JobResult AwaitJobOrExecuteImmediateInternal(JobHandle dependency);
    };
} // namespace Savanna::Concurrency
