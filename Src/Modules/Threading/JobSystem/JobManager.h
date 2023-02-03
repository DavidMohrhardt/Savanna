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

#include <Public/ISavannaJobs.h>

#include <queue>
#include <unordered_map>
#include <vector>

namespace Savanna::Concurrency
{
    class JobManager : public Singleton<JobManager>
    {
    private:
        std::vector<JobThreads> m_JobThreads;

        std::queue<JobHandle> m_LowPriorityJobs;
        std::queue<JobHandle> m_NormalPriorityJobs;
        std::queue<JobHandle> m_HighPriorityJobs;

        std::unordered_map<JobHandle, JobState> m_JobHandles;

    public:
        JobManager();
        ~JobManager();

    public:
        template<typename T, Args... args>
        requires std::derived_from<T, IJob>
        JobHandle AcquireJobHandle(JobHandle dependency = k_InvalidJobHandle, Args... args)
        {
            T* pJob = m_Pool.Allocate<T>();
            pJob->T(std::forward<Args>(args)...);
            return reinterpret_cast<JobHandle>(pJob);
        }

        JobHandle AcquireJobHandle(const IJobInterface* pIJobInterface, JobPriority priority, JobHandle dependency = k_InvalidJobHandle);

        void ReleaseJobHandle(JobHandle jobHandle);

        JobHandle ScheduleJob(IJob* job, JobPriority priority = JobPriority::Normal, JobHandle dependency = k_InvalidJobHandle);

        void ScheduleJob(JobHandle& handle, JobPriority priority = JobPriority::Normal);
        void ScheduleJobBatch(JobHandle* handles, size_t jobCount, JobPriority priority = JobPriority::Normal);

        void AwaitCompletion(JobHandle jobHandle);
        bool TryCancelJob(JobHandle jobHandle) SAVANNA_NOEXCEPT;

        SAVANNA_NO_DISCARD JobState GetJobState(JobHandle jobHandle);

    private:
        inline JobHandle GetNextAvailableJobHandle()
        {
            while (outHandle == k_InvalidJobHandle)
            {
                outHandle = m_JobHandleCounter.fetch_add(1);
            }
            return outHandle;
        }
    };
} // namespace Savanna::Concurrency
