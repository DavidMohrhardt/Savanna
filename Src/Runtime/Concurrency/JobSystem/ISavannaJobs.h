/**
 * @file ISavannaJobs.h
 * @author David Mohrhardt (https://github.com/DavidMohrhardt/Savanna)
 * @brief
 * @version 0.1
 * @date 2023-01-16
 *
 * @copyright Copyright (c) 2023
 *
 */
#pragma once

#include "Public/ISavannaExtension.h"

#if defined(__cplusplus)

#include <atomic>
#include <tuple>

#endif // defined(__cplusplus)

/**
 * @brief An opaque handle to a scheduled job.
 *
 */
typedef se_int64 se_JobHandle_t;

/**
 * @brief An invalid job handle. Is equal to 0LL.
 */
const se_JobHandle_t k_InvalidJobHandle = 0LL;

typedef enum se_JobResult_t
{
    k_SavannaJobResultInvalid,
    k_SavannaJobResultSuccess,
    k_SavannaJobResultError,
    k_SavannaJobResultCancelled,
    k_SavannaJobResultCount
} se_JobResult_t;

/**
 * @brief Defines the state of a job.
 */
typedef enum se_JobState_t
{
    /**
     * @brief The job is invalid.
    */
    k_SavannaJobStateInvalid,

    /**
     * @brief The job is ready to be scheduled.
     */
    k_SavannaJobStateReady,

    /**
     * @brief The job is currently running.
     */
    k_SavannaJobStateRunning,

    /**
     * @brief The total number of states.
     */
    k_SavannaJobStateCount
} se_JobState_t;

/**
 * @brief An enumeration defining the priority of jobs.
 *
 * Jobs are computed in the order they are received but
 * higher priority jobs will always be checked for first.
 */
typedef enum se_JobPriority_t
{

    k_SavannaJobPriorityLow,


    k_SavannaJobPriorityNormal,


    k_SavannaJobPriorityHigh,

    /**
     * @brief The total number of priorities.
     */
    k_SavannaJobPriorityCount
} se_JobPriority_t;

/**
 * @brief Defines the accepted functor type for a given IJob in the C-Api
 */
typedef se_JobResult_t (*se_JobExecuteFunc_t)(void*);

/**
 * @brief Defines the accepted functor type for a given IJob in the C-Api
 */
typedef void(*se_JobResultCallbackFunc_t)(void*);

/**
 * @brief Defines a struct containing the function pointers for a given IJob in the C-Api.
 */
typedef struct se_IJobInterface_t
{
    /**
     * @brief The function pointer to the execute function of the job.
    */
    se_JobExecuteFunc_t executeFunc;

    /**
     * @brief The function pointer to the on complete function of the job.
    */
    se_JobResultCallbackFunc_t onCompleteFunc;

    /**
     * @brief The function pointer to the on cancel function of the job.
    */
    se_JobResultCallbackFunc_t onCancelFunc;

    /**
     * @brief The function pointer to the on error function of the job.
    */
    se_JobResultCallbackFunc_t onErrorFunc;

    /**
     * @brief The user data pointer of the job. User data is passed to the execute function.
    */
    void* pUserData;
} se_IJobInterface_t;

#if defined(__cplusplus)

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
     * @brief TODO
     *
     */
    class IJob
    {
    protected:
        friend class JobManager;

    public:
        IJob() = default;
        virtual ~IJob() = default;

    protected:
        virtual bool TryCancel() { return false; }

    public:
        /**
         * @brief Provides the implementation for the job. This function will be called on a worker thread.
         * It is up to the implementation to handle any synchronization and thread safety. The job system will
         * only provide scheduling services.
         *
         * @return true if the job was executed successfully.
         * @return false if the job failed to execute.
         */
        virtual JobResult Execute() = 0;

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

    class PrimitiveJob : public IJob
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

    /**
     * @brief
     *
     * @tparam Args
     */
    template <typename T>
    class TemporaryJob final : public IJob
    {
    private:
        static_assert(std::is_base_of_v<IJob, T>, "T must be derived from IJob!");
        friend class IJob;
        T m_Job;

    public:
        TemporaryJob() = delete;

        template <typename ...Args>
        TemporaryJob(Args... args)
            : m_Job(args...)
        {}

        TemporaryJob(T&& job)
            : m_Job(std::move(job))
        {}

    private:
        virtual ~TemporaryJob() override {}

        void Dispose()
        {
            delete this;
        }

    public:
        TemporaryJob(const TemporaryJob&) = delete;
        TemporaryJob(TemporaryJob&& other) noexcept
            : m_Job(std::move(other.m_Job))
        {
            SAVANNA_MOVE_MEMBER(m_Job, other);
        }

        TemporaryJob& operator=(const TemporaryJob&) = delete;
        TemporaryJob& operator=(TemporaryJob&& other) noexcept
        {
            SAVANNA_MOVE_MEMBER(m_Job, other);
            return *this;
        }

        virtual JobResult Execute() override
        {
            return m_Job.Execute();
        }

        virtual void OnComplete() override
        {
            m_Job.OnComplete();
            Dispose();
        }

        virtual void OnCancel() override
        {
            m_Job.OnCancel();
            Dispose();
        }

        virtual void OnError() override
        {
            m_Job.OnError();
            Dispose();
        }
    };

    /**
     * @brief
     *
     * @tparam Args
     */

} // namespace Savanna::Concurrency

#endif // end __cplusplus
