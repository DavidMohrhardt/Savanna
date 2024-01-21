/**
 * @file ISavannaJobs.h
 * @author David Mohrhardt (https://github.com/DavidMohrhardt/Savanna)
 * @brief TODO @David.Mohrhardt Document
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
typedef se_int64 seJobHandle;

/**
 * @brief An invalid job handle. Is equal to 0LL.
 */
const seJobHandle k_InvalidJobHandle = 0LL;

typedef enum seJobResult : se_uint32
{
    k_SavannaJobResultInvalid = 0,

    k_SavannaJobResultSuccess,
    k_SavannaJobResultError,
    k_SavannaJobResultCancelled,
    k_SavannaJobResultCount
} seJobResult;

/**
 * @brief Defines the state of a job.
 */
typedef enum seJobState
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
} seJobState;

/**
 * @brief An enumeration defining the priority of jobs.
 *
 * Jobs are computed in the order they are received but
 * higher priority jobs will always be checked for first.
 *
 * TODO @david.mohrhardt: Rework priority to instead use a work stealing queue.
 */
typedef enum seJobPriority
{

    k_SavannaJobPriorityLow,


    k_SavannaJobPriorityNormal,


    k_SavannaJobPriorityHigh,

    /**
     * @brief The total number of priorities.
     */
    k_SavannaJobPriorityCount
} seJobPriority;

/**
 * @brief Defines the accepted functor type for a given IJob in the C-Api
 */
typedef seJobResult (*pfn_seJobExecute)(void*);

/**
 * @brief Defines the accepted functor type for a given IJob in the C-Api
 */
typedef void(*pfn_seJobResultCallback)(void*);

/**
 * @brief Defines a struct containing the function pointers for a given IJob in the C-Api.
 */
typedef struct seIJobInterface
{
    /**
     * @brief The function pointer to the execute function of the job.
    */
    pfn_seJobExecute executeFunc;

    /**
     * @brief The function pointer to the on complete function of the job.
    */
    pfn_seJobResultCallback onCompleteFunc;

    /**
     * @brief The function pointer to the on cancel function of the job.
    */
    pfn_seJobResultCallback onCancelFunc;

    /**
     * @brief The function pointer to the on error function of the job.
    */
    pfn_seJobResultCallback onErrorFunc;
} seIJobInterface;

typedef struct seJobDefinition
{
    /**
     * @brief A pointer to the interface the job should be using. Must remain
     * valid for the duration of the job.
     */
    seIJobInterface* m_pInterface;

    /**
     * @brief A pointer to the job input data. Must be valid for the
     * duration of the job.
     *
     */
    void* m_pJobInputData;
} seJobDefinition;

SAVANNA_EXPORT(seJobHandle) SavannaConcurrencyJobSystemScheduleJob(
    seJobDefinition& jobInterface,
    seJobPriority priority,
    seJobHandle dependency);

SAVANNA_EXPORT(void) SavannaConcurrencyJobSystemAwaitJob(seJobHandle jobHandle);


