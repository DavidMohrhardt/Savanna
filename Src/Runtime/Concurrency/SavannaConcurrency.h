/**
 * @file SavannaConcurrency.h
 * @author David Mohrhardt (https://github.com/DavidMohrhardt/Savanna)
 * @brief
 * @version 0.1
 * @date 2023-10-28
 *
 * @copyright Copyright (c) 2023
 *
 */
#pragma once

#include "Public/ISavannaConcurrency.h"
#include "Public/ISavannaJobs.hpp"

namespace Savanna::Concurrency
{
    class Info
    {
    private:
        friend class JobManager;

        static void Initialize();
        static void Reset();

    public:
    };
} // namespace Savanna::Concurrency
