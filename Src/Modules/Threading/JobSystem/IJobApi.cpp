/**
 * @file IJob.cpp
 * @author David Mohrhardt (https://github.com/DavidMohrhardt/Savanna)
 * @brief
 * @version 0.1
 * @date 2023-01-30
 *
 * @copyright Copyright (c) 2023
 *
 */
#include "Public/ISavannaJobs.h"

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
SAVANNA_EXPORT(se_JobHandle_t) SavannaEngine_ScheduleNativeJob(const se_IJobFuncs_t* pJobFunctions, void* pData, se_JobPriority_t priority, se_JobHandle_t dependency = k_InvalidJobHandle)
{
    using namespace Savanna::Concurrency
    CJob job(pJobFunctions, pData);
    return JobSystem::Get().ScheduleJob(job, pData, priority, dependency);
}

/**
 * @brief Requests the destruction of a given job handle. Since jobs are allocated objects the handle must be returned to the engine when
 * it is no longer needed. Safe to call on executing jobs as it will simply mark the job for destruction when it is completed.
 *
 * @param jobHandle The handle to the job to be released.
 */
SAVANNA_EXPORT(void) SavannaEngine_ReleaseNativeJob(se_JobHandle_t jobHandle)
{
    JobSystem::Get().ReleaseJob(jobHandle);
}

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
SAVANNA_EXPORT(void) SavannaEngine_ScheduleNativeJobBatch(se_IJobFuncs_t* const pJobFuncs, void** ppData, se_uint32 jobCount, se_JobPriority_t* priorities, se_JobHandle_t* pOutJobHandles, se_JobHandle_t dependency = k_InvalidJobHandle)
{
    using namespace Savanna::Concurrency
    CJob* pJobs = new CJob[jobCount];
    for (se_uint32 i = 0; i < jobCount; ++i)
    {
        pJobs[i] = CJob(pJobFuncs[i], ppData[i]);
    }
    JobSystem::Get().ScheduleJobBatch(pJobs, jobCount, priorities, pOutJobHandles, dependency);
    delete[] pJobs;
}

/**
 * @brief Waits for the given job to complete.
 *
 * @param jobHandle The handle to the job to be waited on.
 */
SAVANNA_EXPORT(void) SavannaEngine_WaitForJob(se_JobHandle_t jobHandle)
{
    JobSystem::Get().WaitForJob(jobHandle);
}

/**
 * @brief Combines the given jobs into a single job that will wait for all of the given jobs to complete before executing.
 * The combined job will implicitly release all the given jobs once this job has been released using SavannaEngine_ReleaseNativeJob.
 */
SAVANNA_EXPORT(se_JobHandle_t) SavannaEngine_CombineJobHandles(se_JobHandle_t* pJobHandles, se_uint32 jobCount)
{
    return JobSystem::Get().CombineJobHandles(pJobHandles, jobCount);
}

/**
 * @brief Returns the current state of the given job.
 *
 * @param jobHandle The handle to the job to be queried.
 * @return The current state of the job.
 */
SAVANNA_EXPORT(se_JobState_t) SavannaEngine_PollJobsState(se_JobHandle_t jobHandle)
{
    return JobSystem::Get().PollJobState(jobHandle);
}

/**
 * @brief Returns true if the given job has completed.
 *
 * @param jobHandle The handle to the job to be queried.
 * @return True if the job has completed.
 */
SAVANNA_EXPORT(se_bool_t) SavannaEngine_IsJobCompleted(se_JobHandle_t jobHandle)
{
    return JobSystem::Get().IsJobCompleted(jobHandle);
}
