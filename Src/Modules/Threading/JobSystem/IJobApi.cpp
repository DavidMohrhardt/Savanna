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

#include "JobManager.h"


SAVANNA_EXPORT(se_JobHandle_t) SavannaEngine_AcquireJobHandle(
    const se_IJobInterface_t* pIJobInterface,
    se_JobPriority_t priority,
    se_JobHandle_t dependency = k_InvalidJobHandle)
{
    // These functions require friend access to the JobManager class. So we need to forward the call to the JobManager class.
    return Savanna::Concurrency::JobManager::Get()->AcquireJobHandle(pIJobInterface, priority, dependency);
}

SAVANNA_EXPORT(void) SavannaEngine_ReleaseJobHandle(se_JobHandle_t jobHandle)
{
    Savanna::Concurrency::JobManager::Get()->ReleaseJobHandle(jobHandle);
}

SAVANNA_EXPORT(se_JobScheduleResult_t) SavannaEngine_ScheduleJob(se_JobHandle_t jobHandle)
{
    return Savanna::Concurrency::JobManager::Get()->ScheduleJob(jobHandle);
}

SAVANNA_EXPORT(void) SavannaEngine_AwaitJobCompletion(se_JobHandle_t jobHandle)
{
    Savanna::Concurrency::JobManager::Get()->AwaitCompletion(jobHandle);
}

SAVANNA_EXPORT(se_JobHandle_t) SavannaEngine_CombineDependencies(se_JobHandle_t* pJobHandles, se_uint32 jobCount)
{
    return Savanna::Concurrency::JobManager::Get()->CombineDependencies(pJobHandles, jobCount);
}

SAVANNA_EXPORT(se_JobState_t) SavannaEngine_PollJobState(se_JobHandle_t jobHandle)
{
    return Savanna::Concurrency::JobManager::Get()->GetJobState(jobHandle);
}

SAVANNA_EXPORT(void) SavannaEngine_ScheduleJobBatch(se_JobHandle_t* pJobHandles, se_uint32 jobCount, se_JobHandle_t dependency = k_InvalidJobHandle)
{
    Savanna::Concurrency::JobManager::Get()->ScheduleJobBatch(pJobHandles, jobCount, dependency);
}
