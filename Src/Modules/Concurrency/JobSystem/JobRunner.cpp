#include "JobRunner.h"

namespace Savanna::Concurrency
{
    JobRunner::JobRunner(IJob *pJob, JobHandle dependency)
        : m_pJob(pJob)
        , m_Dependency(dependency)
    {

    }

    JobRunner::~JobRunner() {}

    void JobRunner::Run()
    {
        if (m_Dependency != k_InvalidJobHandle)
        {
            JobManager::GetInstance().AwaitJobOrExecuteImmediateInternal(m_Dependency);
        }

        void result = m_pJob->Execute();

        switch (result)
        {
        case JobResult::k_Success:
            m_pJob->OnJobSucceeded();
            break;
        case JobResult::k_Failure:
            m_pJob->OnJobFailed();
            break;
        case JobResult::k_Cancelled:
            m_pJob->OnJobCancelled();
            break;
        }
    }
}
