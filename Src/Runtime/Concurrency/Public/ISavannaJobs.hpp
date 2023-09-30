/**
 * @file ISavannaJobs.hpp
 * @author David Mohrhardt (https://github.com/DavidMohrhardt/Savanna)
 * @brief
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

namespace Savanna::Concurrency
{
    using JobHandle = se_JobHandle_t;
    using JobPriority = se_JobPriority_t;
    using JobResult = se_JobResult_t;
    using JobState = se_JobState_t;
    using JobExecuteFunc = se_JobExecuteFunc_t;
    using JobResultCallbackFunc = se_JobResultCallbackFunc_t;
    using IJobInterface = se_IJobInterface_t;

    /**
     * @brief An interface for a job. Jobs are the basic unit of work in the job system.
     *
     */
    class IJob
    {
    private:
        friend class JobManager;
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

    /**
     * @brief A primitive job is a job that is created from a set of function pointers.
     * This is useful for creating jobs from C functions and lambdas.
     *
     */
    class PrimitiveJob final : public IJob
    {
    private:
        IJobInterface m_JobInterface = { nullptr, nullptr, nullptr, nullptr, nullptr };

    public:
        PrimitiveJob() = default;
        PrimitiveJob(const IJobInterface& jobInterface)
            : m_JobInterface(jobInterface)
        {
        }

        PrimitiveJob(const se_JobExecuteFunc_t& executeFunc, void* pUserData)
            : m_JobInterface({ executeFunc, nullptr, nullptr, nullptr, pUserData })
        {}

        PrimitiveJob(const se_JobExecuteFunc_t& executeFunc, const se_JobResultCallbackFunc_t& onCompleteFunc, void* pUserData)
            : m_JobInterface({ executeFunc, onCompleteFunc, nullptr, nullptr, pUserData })
        {}

        PrimitiveJob(const se_JobExecuteFunc_t& executeFunc, const se_JobResultCallbackFunc_t& onCompleteFunc, const se_JobResultCallbackFunc_t& onCancelFunc, void* pUserData)
            : m_JobInterface({ executeFunc, onCompleteFunc, onCancelFunc, nullptr, pUserData })
        {}

        PrimitiveJob(const se_JobExecuteFunc_t& executeFunc, const se_JobResultCallbackFunc_t& onCompleteFunc, const se_JobResultCallbackFunc_t& onCancelFunc, const se_JobResultCallbackFunc_t& onErrorFunc, void* pUserData)
            : m_JobInterface({ executeFunc, onCompleteFunc, onCancelFunc, onErrorFunc, pUserData })
        {}

        virtual ~PrimitiveJob() {}

    public:
        virtual JobResult Execute() override
        {
            return m_JobInterface.executeFunc != nullptr
                ? m_JobInterface.executeFunc(m_JobInterface.pUserData)
                : k_SavannaJobResultInvalid;
        }

        virtual void OnComplete() override
        {
            if (m_JobInterface.onCompleteFunc)
            {
                m_JobInterface.onCompleteFunc(m_JobInterface.pUserData);
            }
        }

        virtual void OnCancel() override
        {
            if (m_JobInterface.onCancelFunc)
            {
                m_JobInterface.onCancelFunc(m_JobInterface.pUserData);
            }
        }

        virtual void OnError() override
        {
            if (m_JobInterface.onErrorFunc)
            {
                m_JobInterface.onErrorFunc(m_JobInterface.pUserData);
            }
        }

        void SetJobInterface(const IJobInterface& jobInterface)
        {
            m_JobInterface = jobInterface;
        }
    };

} // namespace Savanna::Concurrency

#endif // I_SAVANNA_JOBS_HPP
