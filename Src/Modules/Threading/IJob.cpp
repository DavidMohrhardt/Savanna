#include "IJob.h"

#include "JobSystem.h"
#include "ISavannaJobs.h"

namespace Savanna::Threading::Jobs
{
    IJob::IJob()
        : m_JobHandle(JobSystem::AcquireHandle())
        , m_JobState(JobState::k_SavannaJobStateReady)
    {}

    IJob::~IJob()
    {
        JobSystem::WaitForJob(m_JobHandle);
        JobSystem::ReleaseJobHandle(m_JobHandle);
    }

    void IJob::SetJobState(JobState state)
    {
        m_JobState.store(state, std::memory_order_relaxed);
    }

    bool IJob::Execute(void *pData)
    {
        auto result = m_JobState.compare_exchange_strong(
            JobState::k_SavannaJobStateReady,
            JobState::k_SavannaJobStateRunning,
            std::memory_order_relaxed);

        if (result)
        {
            ExecuteImpl(pData);
            m_JobState.store(JobState::k_SavannaJobStateCompleted, std::memory_order_relaxed);
            return true;
        }

        return false;
    }

    inline const JobState IJob::PollJobsState() const
    {
        return m_JobState.load(std::memory_order_relaxed);
    }

    const bool IJob::IsCompleted() const
    {
        return PollJobsState() == JobState::k_SavannaJobStateCompleted;
    }

    const JobHandle IJob::Schedule(JobPriority priority)
    {
        return JobSystem::ScheduleJob(this);
    }

    const JobHandle IJob::ScheduleBatch(IJob **jobs, se_uint32 jobCount, JobPriority *priorities)
    {
        return JobSystem::ScheduleJobBatch(jobs, jobCount, priorities);
    }
} // namespace Savanna::Threading::Jobs
