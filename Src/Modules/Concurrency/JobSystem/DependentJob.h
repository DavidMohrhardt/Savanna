/**
 * @file DependentJob.h
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
#include <JobManager.h>

namespace Savanna::Concurrency
{
    template<typename T>
    class DependentJob : public IJob
    {
    private:
        JobHandle m_Dependency;
        T m_Job;

    public:
        DependentJob(JobHandle dependency, T& job) : m_Dependency(dependency), m_Job(job) {}
        ~DependentJob() = default;

        virtual JobResult Execute() override
        {
            JobManager::Get().WaitForJob(m_Dependency);
            return m_Job.Execute();
        }

        virtual void OnJobFinished(JobResult result) override
        {
            m_Job.OnJobFinished(result);
        }

        virtual void OnJobCancelled() override
        {
            m_Job.OnJobCancelled();
        }

        virtual void OnJobFailed() override
        {
            m_Job.OnJobFailed();
        }

        virtual void OnJobSucceeded() override
        {
            m_Job.OnJobSucceeded();
        }
    };
} // namespace Savanna::Concurrency
