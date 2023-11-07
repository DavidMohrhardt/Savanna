/**
 * @file ThreadManager.h
 * @author David Mohrhardt (https://github.com/DavidMohrhardt/Savanna)
 * @brief
 * @version 0.1
 * @date 2023-11-05
 *
 * @copyright Copyright (c) 2023
 *
 */
#pragma once

#include <SavannaEngine.h>
#include <Utilities/SavannaCoding.h>
#include <Types/Manager/GlobalManager.h>
#include <Types/Containers/Arrays/dynamic_array.h>
#include <Types/Containers/Arrays/fixed_array.h>
#include <Types/Containers/Queues/atomic_queue.h>
#include <Types/Locks/SpinLock.h>

#include "Public/ISavannaConcurrency.h"
#include "Public/ISavannaJobs.hpp"
#include "ConcurrencyCapabilities.h"
#include "EngineThread.h"

#include <atomic>

namespace Savanna::Concurrency
{
    class ThreadManager final : public GlobalManager<ThreadManager>
    {
    private:
        DEFINE_GLOBAL_MANAGER_FRIENDS_FOR(ThreadManager);
        friend class JobManager;

        static ThreadExecutionInterface* s_pDefaultUnreservedThreadInterface;
        static void SetUnreservedThreadDefaultExecution(ThreadExecutionInterface* pExecutionInterface);

        // EngineThread uses a pointer to a std::thread internally so we provide that thread
        // space here to avoid allocations during runtime.
        MemoryBuffer m_ThreadScratchBuffer;
        dynamic_array<EngineThread> m_ThreadPool;
        dynamic_array<std::atomic_uint8_t> m_ReservationStates;

        size_t m_ReservedThreadCount;

        void StartUnreservedThreadsInternal();

    public:
        ThreadManager();
        ~ThreadManager();

        bool TryAcquireThreads(const uint8 requestedThreads, se_ThreadHandle_t* pOutThreadHandles);
        void ReleaseThreads(const uint8 threadCount, const se_ThreadHandle_t* pThreadHandles);

        void SetThreadExecutionInterface(const uint8 threadCount, const se_ThreadHandle_t* pThreadHandles, ThreadExecutionInterface* pExecutionInterface);

        void StartThreads(const uint8 threadCount, const se_ThreadHandle_t* pThreadHandles);
        void StopThreads(const uint8 threadCount, const se_ThreadHandle_t* pThreadHandles);

    protected:
        virtual bool InitializeInternal() final;
        virtual void StartInternal() final;
        virtual void StopInternal() final;
        virtual void ShutdownInternal() final;
    };
}
