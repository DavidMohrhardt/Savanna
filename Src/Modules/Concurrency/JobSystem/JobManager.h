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

#include "Public/ISavannaJobs.h"

#include "JobRunner.h"

#include <atomic>
#include <queue>
#include <unordered_map>
#include <vector>

namespace Savanna::Concurrency
{
    class JobManager : public Singleton<JobManager>
    {
    public:
        static constexpr uint8 k_MaxThreadPoolSize = std::thread::hardware_concurrency() - 1;

    private:
        static void ProcessJobs();

    private:
        uint8 m_ThreadPoolSize;
        std::atomic_bool m_ProcessingJobs;
        std::vector<std::thread> m_JobThreads;

        std::queue<JobHandle> m_LowPriorityJobs;
        std::queue<JobHandle> m_NormalPriorityJobs;
        std::queue<JobHandle> m_HighPriorityJobs;

        std::unordered_map<JobHandle, JobState> m_JobHandles;

    public:
        JobManager(uint8 threadPoolSize = k_MaxThreadPoolSize);
        ~JobManager();

    public:
        template<typename T, Args... args>
        requires std::derived_from<T, IJob>
        JobHandle AcquireJobHandle(JobHandle dependency = k_InvalidJobHandle, Args... args)
        {
            void* pJob = nullptr;
            if (dependency != k_InvalidJobHandle)
            {
                pJob = m_Pool.Allocate<JobRunner<T>>(dependency, T(std::forward<Args>(args)...));
            }
            else
            {
                pJob = m_Pool.Allocate<T>();
                reinterpret_cast<T*>(pJob)->T(std::forward(args)...);
            }
            return { pJob };
        }

        void Start();
        void Stop(bool synchronized = false);

        void ReleaseJobHandle(JobHandle jobHandle);

        JobHandle ScheduleJob(IJob* job, JobPriority priority = JobPriority::Normal, JobHandle dependency = k_InvalidJobHandle);

        void ScheduleJob(JobHandle& handle, JobPriority priority = JobPriority::Normal);
        void ScheduleJobBatch(JobHandle* handles, size_t jobCount, JobPriority priority = JobPriority::Normal);

        void AwaitCompletion(JobHandle jobHandle);
        bool TryCancelJob(JobHandle jobHandle) SAVANNA_NOEXCEPT;

        SAVANNA_NO_DISCARD JobState GetJobState(JobHandle jobHandle);
    };
} // namespace Savanna::Concurrency
