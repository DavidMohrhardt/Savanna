/**
 * @file EngineThread.h
 * @author David Mohrhardt (https://github.com/DavidMohrhardt/Savanna)
 * @brief TODO @David.Mohrhardt Document
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

namespace savanna::Concurrency
{
    typedef void (*se_pfnThreadFunction_t)(void* pArgs);

    struct ThreadExecutionInterface
    {
        se_pfnThreadFunction_t m_pFunction;
        void* m_pData;
    };

    class EngineThread
    {
    private:
        friend class ThreadManager;

        static void RunThread(EngineThread* pThread);

        std::thread* m_pThread;
        std::atomic<ThreadExecutionInterface*> m_ExecutionInfo;
        std::atomic_bool m_Active;

    public:
        EngineThread();
        EngineThread(EngineThread&& other) noexcept;
        EngineThread& operator=(EngineThread&& other) noexcept;

        EngineThread(const EngineThread&) = delete;
        EngineThread& operator=(const EngineThread&) = delete;
        ~EngineThread();

        bool IsActive() const { return m_Active.load(std::memory_order_acquire); }

    private:
        void SetExecutionInterface(ThreadExecutionInterface* pJobInterface);

        void Start(std::thread* pThreadBuffer);
        void Stop();
    };

} // namespace savanna::Concurrency
