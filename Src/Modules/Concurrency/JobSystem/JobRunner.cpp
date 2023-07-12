#include "JobRunner.h"
#include "JobManager.h"

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
            JobManager::Get()->AwaitJobOrExecuteImmediateInternal(m_Dependency);
        }

        auto result = m_pJob->Execute();

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
    }
}
