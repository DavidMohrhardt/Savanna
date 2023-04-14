/**
 * @file ISavannaConcurrency.h
 * @author David Mohrhardt (https://github.com/DavidMohrhardt/Savanna)
 * @brief
 * @version 0.1
 * @date 2023-01-16
 *
 * @copyright Copyright (c) 2023
 *
 */
#pragma once

#include "ISavannaJobs.h"

SAVANNA_EXPORT(se_threadId_t) SavannaEngine_Concurrency_GetMainThreadId();

SAVANNA_EXPORT(se_threadId_t) SavannaEngine_Concurrency_GetCurrentThreadId();

SAVANNA_EXPORT(se_bool_t) SavannaEngine_Concurrency_IsMainThread();
