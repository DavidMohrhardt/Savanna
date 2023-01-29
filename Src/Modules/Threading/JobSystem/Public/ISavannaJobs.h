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

/**
 * @brief Defines the accepted functor type for a given IJob in the C-Api
 */
typedef void (*se_JobFunc_t)(void* data);

/**
 * @brief The handle
 *
 */
typedef se_JobHandle_t intptr_t;

/**
 * @brief
 */
typedef enum se_JobState_t
{
    k_SavannaJobStateInvalid,
    k_SavannaJobStateReady,
    k_SavannaJobStateRunning,
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

#if defined(__cplusplus)

#include <atomic>

namespace Savanna::Threading::Jobs
{
    /**
     * @brief An interface for a job class that can be submitted to the Savanna Job System.
     */
    class IJob
    {
    private:
        friend class JobSystem;

        JobHandle m_JobHandle;
        std::atomic<JobState> JobState;

    public:
        static const JobHandle ScheduleBatch(IJob** jobs, se_uint32 jobCount, JobPriority* priorities = nullptr);

    public:
        IJob();
        virtual ~IJob();

    private:
        void SetJobState(JobState state);

    protected:
        virtual void ExecuteImpl() = 0;

    public:
        bool Execute(void* pData = nullptr);

        const JobState PollJobsState() const;
        const bool IsCompleted() const;

        const JobHandle Schedule(JobPriority priority = k_SavannaJobPriorityNormal);
    };
} // namespace Savanna::Threading::Jobs

#endif // end __cplusplus

/**
 * @brief Creates and schedules a job with the given jobFunc. The job will be executed on the next available thread.
 *
 * @param jobHandle The handle to the job to be scheduled.
 * @param pData A pointer to the data to be passed to the job. This can be null for jobs that do not require data. The data must
 * be valid until the job is completed.
 * @param priority The priority of the job. The engine maintains a queue for each priority level and several threads are assigned
 * to each queue
 * but there are also general threads that take jobs in priority order. This means that jobs with a higher priority will take
 * precedence over jobs with a lower priority but lower priority jobs will still be executed.
 * @return The engine created JobHandle for the job. This can be used to query the state of the job and must be released with SavannaEngine_ReleaseNativeJob.
 */
SAVANNA_IMPORT(se_JobHandle_t) SavannaEngine_ScheduleNativeJob(se_JobFunc_t jobFunc, void* pData, se_JobPriority_t priority, se_JobHandle_t dependency = k_InvalidJobHandle);

/**
 * @brief Requests the destruction of a given job handle. Since jobs are allocated objects the handle must be returned to the engine when
 * it is no longer needed. Safe to call on executing jobs as it will simply mark the job for destruction when it is completed.
 *
 * @param jobHandle The handle to the job to be released.
 */
SAVANNA_IMPORT(void) SavannaEngine_ReleaseNativeJob(se_JobHandle_t jobHandle);

/**
 * @brief Appends many jobs to the job queue with the given priority. The jobs will be executed on the next available thread.
 *
 * @param pJobFuncs An array of function pointers for jobs to be scheduled.
 * @param ppData An array of pointers to the data to be passed to the jobs. This can be null for jobs that do not require data. The data must be valid until the job is completed.
 * @param jobCount The number of jobs to be scheduled.
 * @param pPriorities An array of priorities for each job. The engine maintains a queue for each priority level and several threads are assigned to each queue
 * but there are also general threads that take jobs in priority order. This means that jobs with a higher priority will take precedence over jobs
 * with a lower priority but lower priority jobs will still be executed.
 * @param pOutJobHandles An output array of job handles that can be used to query the state of the jobs. Must be at least as large as jobCount.
 *
 * @note The job handles must be released with SavannaEngine_ReleaseNativeJob.
 */
SAVANNA_IMPORT(void) SavannaEngine_ScheduleNativeJobBatch(se_JobFunc_t* const pJobFuncs, void** ppData, se_uint32 jobCount, se_JobPriority_t* priorities, se_JobHandle_t* pOutJobHandles, se_JobHandle_t dependency = k_InvalidJobHandle);

/**
 * @brief Waits for the given job to complete.
 *
 * @param jobHandle The handle to the job to be waited on.
 */
SAVANNA_IMPORT(void) SavannaEngine_WaitForJob(se_JobHandle_t jobHandle);

/**
 * @brief Combines the given jobs into a single job that will wait for all of the given jobs to complete before executing.
 * The combined job will implicitly release all the given jobs once this job has been released using SavannaEngine_ReleaseNativeJob.
 */
SAVANNA_IMPORT(se_JobHandle_t) SavannaEngine_CombineJobHandles(se_JobHandle_t* pJobHandles, se_uint32 jobCount);

/**
 * @brief Returns the current state of the given job.
 *
 * @param jobHandle The handle to the job to be queried.
 * @return The current state of the job.
 */
SAVANNA_IMPORT(se_JobState_t) SavannaEngine_PollJobsState(se_JobHandle_t jobHandle);

/**
 * @brief Returns true if the given job has completed.
 *
 * @param jobHandle The handle to the job to be queried.
 * @return True if the job has completed.
 */
SAVANNA_IMPORT(se_bool_t) SavannaEngine_IsJobCompleted(se_JobHandle_t jobHandle);
