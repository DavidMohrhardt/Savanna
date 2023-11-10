#include "AutoDisposeJob.h"

namespace Savanna::Concurrency
{
    inline void AutoDisposeJobBase::Dispose()
    {
        SAVANNA_DELETE(m_AllocatorKind, this);
    }

    AutoDisposeJobBase::AutoDisposeJobBase(AllocatorKind allocatorKind)
        : m_AllocatorKind{allocatorKind}
    {}

    void AutoDisposeJobBase::OnComplete() { Dispose(); }

    void AutoDisposeJobBase::OnError() { Dispose(); }

    void AutoDisposeJobBase::OnCancel() { Dispose(); }

    AutoDisposePrimitiveJob::AutoDisposePrimitiveJob(const se_JobDefinition_t &jobDefinition, AllocatorKind allocatorKind)
        : AutoDisposeJobBase(allocatorKind)
        , m_JobDefinition{jobDefinition}
    {}

    JobResult AutoDisposePrimitiveJob::Execute()
    {
        if (m_JobDefinition.m_pInterface != nullptr)
        {
            return (*m_JobDefinition.m_pInterface).executeFunc(m_JobDefinition.m_pJobInputData);
        }

        return JobResult::k_SavannaJobResultInvalid;
    }
} // namespace Savanna::Concurrency
