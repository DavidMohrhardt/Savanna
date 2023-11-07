/**
 * @file DependencyJobs.h
 * @author David Mohrhardt (https://github.com/DavidMohrhardt/Savanna)
 * @brief
 * @version 0.1
 * @date 2023-09-16
 *
 * @copyright Copyright (c) 2023
 *
 */
#pragma once

#include <SavannaEngine.h>
#include "JobManager.h"

#include <Types/Containers/Arrays/dynamic_array.h>

namespace Savanna::Concurrency
{
    class DependencyAwaiterJob final : public IJob
    {
    private:
        dynamic_array<JobHandle> m_Dependencies;

    public:
        DependencyAwaiterJob(const JobHandle* dependencies, const se_size dependencyCount)
            : m_Dependencies(dependencyCount)
        {
            m_Dependencies.resize_uninitialized(dependencyCount);
            memcpy(m_Dependencies.data(), dependencies, dependencyCount * sizeof(JobHandle));
        }

        virtual ~DependencyAwaiterJob() override {}

        JobResult Execute() override
        {
            for (const auto& dependency : m_Dependencies)
            {
                JobManager::Get()->AwaitJobOrExecuteImmediateInternal(dependency);
            }

            return k_SavannaJobResultSuccess;
        }

        void OnComplete() override
        {
            Dispose();
        }

        void OnError() override
        {
            Dispose();
        }

        void OnCancel() override
        {
            Dispose();
        }

    private:
        void Dispose()
        {
            SAVANNA_DELETE(k_SavannaMemoryLabelGeneral, this);
        }
    };

    class DependentJobWrapper : public IJob
    {
    private:
        friend class IJob;
        IJob* m_Job;
        JobHandle m_Dependency;

    public:
        DependentJobWrapper() = delete;

        DependentJobWrapper(JobHandle dependency, IJob* job)
            : m_Job(job)
            , m_Dependency(dependency)
        {}

        virtual ~DependentJobWrapper() override {}

        JobResult Execute() override
        {
            JobManager::Get()->AwaitJobOrExecuteImmediateInternal(m_Dependency);
            return m_Job->Execute();
        }

        void OnComplete() override
        {
            Dispose();
        }

        void OnError() override
        {
            Dispose();
        }

        void OnCancel() override
        {
            Dispose();
        }

    private:
        void Dispose()
        {
            SAVANNA_DELETE(k_SavannaMemoryLabelGeneral, this);
        }
    };
} // namespace Savanna
