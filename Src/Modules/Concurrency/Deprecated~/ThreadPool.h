/**
 * @file ThreadPool.h
 * @author David Mohrhardt (https://github.com/DavidMohrhardt/Savanna)
 * @brief
 * @version 0.1
 * @date 2023-01-16
 *
 * @copyright Copyright (c) 2023
 *
 */
#pragma once

#include <SavannaEngine.h>
#include <Utilities/SavannaCoding.h>

#include <thread>
#include <vector>

namespace Savanna::Threading
{
    class ThreadPool
    {
    public:
        const uint32 k_ProcessorCount = std::thread::hardware_concurrency();
        const uint32 k_AvailableThreads = k_ProcessorCount - 1; // Main thread is reserved

    private:
        std::vector<std::thread::id> m_ThreadIds;
        std::vector<std::thread> m_Threads;

        const ThreadPoolDescriptor m_ThreadPoolDescriptor;

    public:
        const ThreadPoolDescriptor GetThreadPoolDescriptor() const
        {
            return m_ThreadPoolDescriptor;
        }

    private:
        static std::thread::id s_MainThreadId;
        static bool s_IsInitialized;

    private:
        ThreadPool(const ThreadPoolDescriptor descriptor);
        ~ThreadPool();

    public:
        virtual void Start() = 0;
        virtual void Stop() = 0;

    public:
        void Join();
    };

} // namespace Savanna::Threading
