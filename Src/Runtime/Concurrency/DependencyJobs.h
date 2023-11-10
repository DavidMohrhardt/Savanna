/**
 * @file DependencyJobs.h
 * @author David Mohrhardt (https://github.com/DavidMohrhardt/Savanna)
 * @brief TODO @David.Mohrhardt Document
 * @version 0.1
 * @date 2023-09-16
 *
 * @copyright Copyright (c) 2023
 *
 */
#pragma once

#include <SavannaEngine.h>
#include "ThreadManager.h"
#include "JobSystem.h"
#include "AutoDisposeJob.h"

#include <Types/Containers/Arrays/dynamic_array.h>

namespace Savanna::Concurrency
{
    class DependencyAwaiterJob final : public AutoDisposeJobBase
    {
    private:
        dynamic_array<JobHandle> m_Dependencies;

    public:
        DependencyAwaiterJob(const JobHandle* dependencies, const se_size dependencyCount)
            : AutoDisposeJobBase(k_SavannaAllocatorKindGeneral)
            , m_Dependencies(dependencyCount, k_SavannaAllocatorKindGeneral)
        {
            m_Dependencies.resize_uninitialized(dependencyCount);
            ::memcpy(m_Dependencies.data(), dependencies, dependencyCount * sizeof(JobHandle));
        }

        virtual ~DependencyAwaiterJob() override {}

        JobResult Execute() override
        {
            for (const auto& dependency : m_Dependencies)
            {
                ThreadManager::Get()->GetJobSystem()->AwaitJobOrExecuteImmediateInternal(dependency);
            }

            return k_SavannaJobResultSuccess;
        }
    };

    class DependentJobWrapper : public AutoDisposeJobBase
    {
    private:
        friend class IJob;
        IJob* m_Job;
        JobHandle m_Dependency;

    public:
        DependentJobWrapper() = delete;

        DependentJobWrapper(JobHandle dependency, IJob* job)
            : AutoDisposeJobBase(k_SavannaAllocatorKindGeneral)
            , m_Job(job)
            , m_Dependency(dependency)
        {}

        virtual ~DependentJobWrapper() override {}

        JobResult Execute() override
        {
            ThreadManager::Get()->GetJobSystem()->AwaitJobOrExecuteImmediateInternal(m_Dependency);
            return m_Job->Execute();
        }
    };
} // namespace Savanna
