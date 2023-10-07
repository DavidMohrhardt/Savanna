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
#ifdef false
#pragma once

#include "Public/ISavannaEngine.h"

SAVANNA_EXPORT(se_threadId_t) SavannaConcurrencyGetMainThreadID();
SAVANNA_EXPORT(se_threadId_t) SavannaConcurrencyGetCurrentThreadId();
SAVANNA_EXPORT(se_bool_t) SavannaConcurrencyIsMainThread();
#endif
