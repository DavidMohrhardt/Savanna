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

#include "IJob.h"
#include "JobThread.h"

#include <atomic>
#include <functional>
#include <memory>
#include <mutex>
#include <queue>
#include <thread>
#include <unordered_map>
#include <vector>

namespace Savanna::Threading::Jobs
{
    class JobSystem : public Singleton<JobSystem>
    {
    private:
        friend class JobThread;

        constexpr static size_t k_HardwareThreadCount = std::thread::hardware_concurrency();
        constexpr static size_t k_MaxJobThreadCount = k_HardwareThreadCount - 1; // main thread is reserved

        static std::thread::id s_MainThreadID;

    private:
        static void AwaitJobs();

    private:
        std::vector<JobThread> m_JobThreads;

        std::unordered_map<JobHandle, std::shared_ptr<IJob>> m_JobMap;
        std::queue<JobHandle> m_LowPriorityJobs;
        std::queue<JobHandle> m_NormalPriorityJobs;
        std::queue<JobHandle> m_HighPriorityJobs;

        std::atomic_int_fast64_t m_JobHandleCounter;
        std::atomic_bool m_Started;

        std::mutex m_JobMapMutex;

    private:
        JobSystem();

    public:
        JobSystem(size_t threadCount = k_MaxJobThreadCount);
        ~JobSystem();

    public:
        void Start();
        void Stop();

    private:
        std::shared_ptr<IJob> PollJobs();
    };
} // namespace Savanna::Threading::Jobs
