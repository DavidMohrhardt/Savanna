#include "JobRunner.h"
#include "JobManager.h"

namespace Savanna::Concurrency
{
    JobRunner::JobRunner(IJob *pJob)
        : m_pJob(pJob)
    {

    }

    JobRunner::~JobRunner() {}

    JobResult JobRunner::Run()
    {
        auto jobManager = JobManager::Get();

        jobManager->SetJobState(reinterpret_cast<JobHandle>(m_pJob), JobState::k_SavannaJobStateRunning);
        auto result = m_pJob->Execute();
        jobManager->SetJobState(reinterpret_cast<JobHandle>(m_pJob), JobState::k_SavannaJobStateCompleted);

        switch (result)
        {
        case k_SavannaJobResultSuccess:
            m_pJob->OnComplete();
            break;
        case k_SavannaJobResultError:
            m_pJob->OnError();
            break;
        case k_SavannaJobResultCancelled:
            m_pJob->OnCancel();
            break;
        default:
            break;
        }

        return result;
    }
}
