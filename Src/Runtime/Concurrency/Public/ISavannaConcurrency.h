/**
 * @file ISavannaConcurrency.h
 * @author David Mohrhardt (https://github.com/DavidMohrhardt/Savanna)
 * @brief TODO @David.Mohrhardt Document
 * @version 0.1
 * @date 2023-01-16
 *
 * @copyright Copyright (c) 2023
 *
 */
#ifndef I_SAVANNA_CONCURRENCY_H
#define I_SAVANNA_CONCURRENCY_H

#include "Public/ISavannaEngine.h"

typedef se_intptr se_ThreadHandle_t;

#undef k_SavannaInvalidThreadHandle
#define k_SavannaInvalidThreadHandle ((se_ThreadHandle_t)NULL)

SAVANNA_EXPORT(bool) SavannaConcurrencyIsMainThread();

/**
 * @brief Attempts to acquire the requested number of
 * threads. Once acquired the threads can be submitted a
 * function to execute. Must be released via SavannaConcurrencyThreadManagerReleaseThreads.
 *
 */
SAVANNA_EXPORT(bool) SavannaConcurrencyThreadManagerTryAcquireThreads(se_uint8 requestedThreads, se_ThreadHandle_t* acquiredThreadHandles);

SAVANNA_EXPORT(void) SavannaConcurrencyThreadManagerReleaseThreads(se_uint8 handleCount, se_ThreadHandle_t* pThreadHandles);

#endif
