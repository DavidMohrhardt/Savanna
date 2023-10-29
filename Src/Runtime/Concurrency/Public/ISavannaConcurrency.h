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
#ifndef I_SAVANNA_CONCURRENCY_H
#define I_SAVANNA_CONCURRENCY_H

#include "Public/ISavannaEngine.h"

typedef se_int64 se_ThreadId_t;

SAVANNA_EXPORT(se_bool) SavannaConcurrencyIsMainThread();

#endif
