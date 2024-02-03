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

namespace savanna::concurrency
{
    typedef void (*pfn_seThreadTask)(void* pArgs);

    constexpr std::thread::id k_InvalidThreadId = std::thread::id();

    struct ThreadExecutionInterface
    {
        pfn_seThreadTask m_pFunction;
        void* m_pData;
    };

    class EngineThread
    {
    private:
        friend class ThreadManager;

        static void RunThread(
            std::stop_token stopToken,
            std::atomic<ThreadExecutionInterface*>& executionInterface);

        std::jthread m_Thread;
        std::atomic<ThreadExecutionInterface*> m_ExecutionInfo;
        std::stop_source m_InternalStopSource;

    public:
        EngineThread();
        EngineThread(EngineThread&& other) noexcept;
        EngineThread& operator=(EngineThread&& other) noexcept;

        EngineThread(const EngineThread&) = delete;
        EngineThread& operator=(const EngineThread&) = delete;
        ~EngineThread();

        inline bool IsActive() const
        {
            return m_Thread.get_id() != k_InvalidThreadId && m_Thread.joinable();
        }

    private:
        void SetExecutionInterface(ThreadExecutionInterface* pJobInterface);

        void Start();
        void RequestStop();
    };

} // namespace savanna::Concurrency
