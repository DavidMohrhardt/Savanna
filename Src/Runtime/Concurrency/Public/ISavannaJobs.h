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

#include "Public/ISavannaEngine.h"

/**
 * @brief An opaque handle to a scheduled job.
 *
 */
typedef se_int64 se_JobHandle_t;

/**
 * @brief An invalid job handle. Is equal to 0LL.
 */
const se_JobHandle_t k_InvalidJobHandle = 0LL;

typedef enum se_JobResult_t : se_uint32
{
    k_SavannaJobResultInvalid = 0,

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
     * @brief The job is finished but may not have completed successfully.
     */
    k_SavannaJobStateFinished,

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
