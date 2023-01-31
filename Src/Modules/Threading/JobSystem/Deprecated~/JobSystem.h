/**
 * @file JobSystem.h
 * @author David Mohrhardt (https://github.com/DavidMohrhardt/Savanna)
 * @brief
 * @version 0.1
 * @date 2023-01-18
 *
 * @copyright Copyright (c) 2023
 *
 */
#pragma once

#include <SavannaEngine.h>
#include <Utilities/SavannaCoding.h>

#include "JobThread.h"

#include <atomic>
#include <functional>
#include <memory>
#include <mutex>
#include <queue>
#include <thread>
#include <unordered_map>
#include <vector>

namespace Savanna::Concurrency
{
    class JobSystem : public Singleton<JobSystem>
    {
        private:
            friend class JobThread;

            constexpr static size_t k_HardwareThreadCount = std::thread::hardware_concurrency();
            constexpr static size_t k_MaxJobThreadCount = k_HardwareThreadCount - 1; // main thread is reserved

            static std::thread::id s_MainThreadID;

        private:
            // std::vector<std::>

    };
} // namespace Savanna::Concurrency
