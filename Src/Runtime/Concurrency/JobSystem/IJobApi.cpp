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
#include "ISavannaJobs.h"

#include "JobManager.h"

// SAVANNA_EXPORT(se_JobHandle_t) SavannaEngine_AcquireJobHandle(
//     const se_IJobInterface_t* pIJobInterface,
//     se_JobPriority_t priority,
//     se_JobHandle_t dependency)
// {
//     // These functions require friend access to the JobManager class. So we need to forward the call to the JobManager class.
//     using namespace Savanna::Concurrency;
//     auto pJobManager = JobManager::Get();
//     if (pJobManager == nullptr)
//     {
//         return k_InvalidJobHandle;
//     }
//     return pJobManager->ScheduleJob(
//         new AutomaticJob<PrimitiveJob>(pIJobInterface),
//         priority,
//         dependency);
// }

// SAVANNA_EXPORT(void) SavannaEngine_ReleaseJobHandle(se_JobHandle_t jobHandle)
// {
//     Savanna::Concurrency::JobManager::Get()->ReleaseJobHandle(jobHandle);
// }

// SAVANNA_EXPORT(se_JobHandle_t) SavannaEngine_ScheduleJob(void* pJob)
// {
//     return Savanna::Concurrency::JobManager::Get()->ScheduleJob(jobHandle);
// }

// SAVANNA_EXPORT(void) SavannaEngine_AwaitJobCompletion(se_JobHandle_t jobHandle)
// {
//     Savanna::Concurrency::JobManager::Get()->AwaitCompletion(jobHandle);
// }

// SAVANNA_EXPORT(se_JobHandle_t) SavannaEngine_CombineDependencies(se_JobHandle_t* pJobHandles, se_uint32 jobCount)
// {
//     return Savanna::Concurrency::JobManager::Get()->CombineDependencies(pJobHandles, jobCount);
// }

// SAVANNA_EXPORT(se_JobState_t) SavannaEngine_PollJobState(se_JobHandle_t jobHandle)
// {
//     return Savanna::Concurrency::JobManager::Get()->GetJobState(jobHandle);
// }

// SAVANNA_EXPORT(void) SavannaEngine_ScheduleJobBatch(se_JobHandle_t* pJobHandles, se_uint32 jobCount, se_JobHandle_t dependency)
// {
//     Savanna::Concurrency::JobManager::Get()->ScheduleJobBatch(pJobHandles, jobCount, dependency);
// }
