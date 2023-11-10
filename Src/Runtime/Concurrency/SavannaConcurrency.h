/**
 * @file SavannaConcurrency.h
 * @author David Mohrhardt (https://github.com/DavidMohrhardt/Savanna)
 * @brief TODO @David.Mohrhardt Document
 * @version 0.1
 * @date 2023-10-28
 *
 * @copyright Copyright (c) 2023
 *
 */
#pragma once

#include "Public/ISavannaConcurrency.h"
#include "Public/ISavannaJobs.hpp"

#define SAVANNA_ASSERT_MAIN_THREAD() \
    SAVANNA_ASSERT( \
        SavannaConcurrencyIsMainThread(), \
        "This function must be called from the main thread");

namespace Savanna::Concurrency
{
    class Info
    {
    private:
        friend class ThreadManager;
        friend class JobSystem;

        static void Initialize();
        static void Reset();
    };
} // namespace Savanna::Concurrency
