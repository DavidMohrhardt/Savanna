/**
 * @file ISavannaJobs.hpp
 * @author David Mohrhardt (https://github.com/DavidMohrhardt/Savanna)
 * @brief TODO @David.Mohrhardt Document
 * @version 0.1
 * @date 2023-09-30
 *
 * @copyright Copyright (c) 2023
 *
 */
#ifndef I_SAVANNA_JOBS_HPP
#define I_SAVANNA_JOBS_HPP

#include "ISavannaJobs.h"
#include "Public/ISavannaEngine.hpp"

#include <atomic>
#include <tuple>

namespace savanna::concurrency
{
    using JobHandle = seJobHandle;
    using JobPriority = seJobPriority;
    using JobResult = seJobResult;
    using JobState = seJobState;
    using JobExecuteFunc = pfn_seJobExecute;
    using JobResultCallbackFunc = pfn_seJobResultCallback;
    using IJobInterface = seIJobInterface;

    class JobSystem;

    /**
     * @brief An interface for a job. Jobs are the basic unit of work in the job system.
     *
     */
    class IJob
    {
    private:
        friend class JobSystem;
        friend class JobRunner;

        std::atomic<uint32> m_JobState = static_cast<uint32>(k_SavannaJobStateInvalid);
        JobHandle m_Dependency = k_InvalidJobHandle;

        void SetDependency(JobHandle dependency) { m_Dependency = dependency; }
        JobHandle GetDependency() const { return m_Dependency; }

    public:
        IJob() = default;

        IJob(const IJob&) = delete;
        IJob& operator=(const IJob&) = delete;

        IJob(IJob&&) = delete;
        IJob& operator=(IJob&&) = delete;

        virtual ~IJob() = default;

        /**
         * @brief Provides the implementation for the job. This function will be called on a worker thread.
         * It is up to the implementation to handle any synchronization and thread safety. The job system will
         * only provide scheduling services.
         *
         * @return true if the job was executed successfully.
         * @return false if the job failed to execute.
         */
        virtual JobResult Execute() = 0;

        JobState GetState() const
        {
            return static_cast<JobState>(m_JobState.load(std::memory_order_relaxed));
        }

    protected:
        /**
         * @brief Attempts to cancel the job. It is up to the implementation to handle cancellation.
         *
         * @return true if the job was cancelled.
         * @return false if the job could not be cancelled.
         */
        virtual bool TryCancel() { return false; }

        /**
         * @brief Called when the job has completed successfully.
         *
         */
        virtual void OnComplete() {};

        /**
         * @brief Called when the job has been cancelled. It is up to the implementation to handle cancellation.
         *
         */
        virtual void OnCancel() {};

        /**
         * @brief Called when the job has failed to execute for any reason.
         *
         */
        virtual void OnError() {};
    };

} // namespace savanna::Concurrency

#endif // I_SAVANNA_JOBS_HPP
