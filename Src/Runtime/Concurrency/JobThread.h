/**
 * @file JobThread.h
 * @author David Mohrhardt (https://github.com/DavidMohrhardt/Savanna)
 * @brief
 * @version 0.1
 * @date 2023-10-24
 *
 * @copyright Copyright (c) 2023
 *
 */
#pragma once

#include "Public/ISavannaJobs.h"

#include "Utilities/SavannaCoding.h"

#include <thread>

namespace Savanna::Concurrency
{
    class ThreadBase
    {
    private:
        std::thread m_Thread;

    protected:
        ThreadBase() = default;
        ~ThreadBase() = default;

        ThreadBase(const ThreadBase&) = delete;
        ThreadBase(ThreadBase&&) = delete;

        void StartThread();
        void JoinThread();
    };
} // namespace Savanna::Concurrency
