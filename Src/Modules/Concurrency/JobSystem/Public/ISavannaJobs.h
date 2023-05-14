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
#include <ISavannaInterface.h>


#if defined(__cplusplus)

#include <atomic>

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
     * @brief The job has been completed. Check the result.
     */
    k_SavannaJobStateCompleted,


    k_SavannaJobStateCount
} se_JobState_t;

/**
 * @brief
 */
typedef enum se_JobPriority_t
{
    k_SavannaJobPriorityLow,
    k_SavannaJobPriorityNormal,
    k_SavannaJobPriorityHigh,
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
    using JobState = se_JobState_t;
    using JobPriority = se_JobPriority_t;
    using JobResult = se_JobResult_t;
    using JobExecuteFunc = se_JobExecuteFunc_t;
    using JobResultCallbackFunc = se_JobResultCallbackFunc_t;
    using IJobInterface = se_IJobInterface_t;

    /**
     * @brief
     *
     */
    class IJob
    {
    private:
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

    class LowLevelJob : public IJob
    {
    private:
        IJobInterface m_JobInterface = { nullptr, nullptr, nullptr, nullptr, nullptr };

    public:
        LowLevelJob() = default;
        LowLevelJob(const IJobInterface& jobInterface)
            : m_JobInterface(jobInterface)
        {
        }

        virtual ~LowLevelJob()
        {
            SavannaEngine_ReleaseJobHandle(this);
        }

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

#endif // end __cplusplus

/**
 * @brief Requests a job handle from the job system. The job handle is used to schedule a job for execution
 * as well as to wait for the job to complete. This function is thread safe and allocates memory from the
 * job system's memory pool. The job handle must be released when it is no longer needed using the
 * SavannaEngine_ReleaseJobHandle function.
 */
SAVANNA_IMPORT(se_JobHandle_t) SavannaEngine_AcquireJobHandle(const se_IJobInterface_t* pIJobInterface, se_JobPriority_t priority, se_JobHandle_t dependency = k_InvalidJobHandle);

/**
 * @brief Releases a job handle back to the job system. This function is thread safe and frees memory from the
 * job system's memory pool. The job handle must not be used after it has been released.
 */
SAVANNA_IMPORT(void) SavannaEngine_ReleaseJobHandle(se_JobHandle_t jobHandle);

/**
 * @brief Schedules a job for execution. This function is thread safe and will return immediately. The job
 * will be executed on a worker thread. The job handle must be released when it is no longer needed using the
 * SavannaEngine_ReleaseJobHandle function. It is recommended to release the job handle via the se_IJobInterface_t
 * callback functions.
 *
 * @param jobHandle The job handle to schedule for execution.
 * @return se_JobScheduleResult_t The result of the scheduling operation.
 */
SAVANNA_IMPORT(se_JobScheduleResult_t) SavannaEngine_ScheduleJob(se_JobHandle_t jobHandle);

/**
 * @brief Waits for a job to complete. This function is thread safe and will block the calling thread until
 * the job has completed.
 *
 */
SAVANNA_IMPORT(void) SavannaEngine_AwaitJobCompletion(se_JobHandle_t jobHandle);

/**
 * @brief Combines the dependencies of multiple jobs into a single job handle. This function is thread safe.
 * The original job handles must be released manually still using the SavannaEngine_ReleaseJobHandle function.
 * Releasing jobs prior to the completion of the combined job will result is a no op.
 *
 * @param pJobHandles An array of job handles to combine.
 * @param jobCount The number of job handles in the array.
 *
 * @return se_JobHandle_t The combined job handle.
 */
SAVANNA_IMPORT(se_JobHandle_t) SavannaEngine_CombineDependencies(se_JobHandle_t* pJobHandles, se_uint32 jobCount);

/**
 * @brief Polls the state of a job. This function is thread safe and will return immediately.
 *
 * @param jobHandle The job handle to poll.
 * @return se_JobState_t The state of the job.
 */
SAVANNA_IMPORT(se_JobState_t) SavannaEngine_PollJobState(se_JobHandle_t jobHandle);

/**
 * @brief Schedules a batch of jobs for execution. This function is thread safe and will return immediately.
 * The job handles must be released when they are no longer needed using the SavannaEngine_ReleaseJobHandle
 * function. It is recommended to release the job handles via the se_IJobInterface_t callback functions.
 *
 * @param pJobHandles An array of job handles to schedule for execution.
 * @param jobCount The number of job handles in the array.
 * @param dependency The job handle to use as a dependency for the batch of jobs. This is optional and can be
 * set to k_InvalidJobHandle if no dependency is needed.
 */
SAVANNA_IMPORT(void) SavannaEngine_ScheduleJobBatch(se_JobHandle_t* pJobHandles, se_uint32 jobCount, se_JobHandle_t dependency = k_InvalidJobHandle);
