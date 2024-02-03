/**
 * @file ThreadManager.h
 * @author David Mohrhardt (https://github.com/DavidMohrhardt/Savanna)
 * @brief TODO @David.Mohrhardt Document
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
#include "JobSystem.h"

#include <atomic>

namespace savanna::concurrency
{
    class ThreadManager final : public GlobalManager<ThreadManager>
    {
    private:
        DEFINE_GLOBAL_MANAGER_FRIENDS_FOR(ThreadManager);
        friend class JobSystem;

        static ThreadExecutionInterface* s_pDefaultUnreservedThreadInterface;
        static void SetUnreservedThreadDefaultExecution(ThreadExecutionInterface* pExecutionInterface);

        dynamic_array<EngineThread> m_ThreadPool;
        dynamic_array<std::atomic_uint8_t> m_ReservationStates;
        size_t m_ReservedThreadCount;

        JobSystem m_JobSystem;

        void StartUnreservedThreadsInternal();

    public:
        ThreadManager();
        ~ThreadManager();

        bool TryAcquireThreads(const uint8 requestedThreads, seThreadHandle* pOutThreadHandles);
        void ReleaseThreads(const uint8 threadCount, const seThreadHandle* pThreadHandles);

        void SetThreadExecutionInterface(const uint8 threadCount, const seThreadHandle* pThreadHandles, ThreadExecutionInterface* pExecutionInterface);

        void StartThreads(const uint8 threadCount, const seThreadHandle* pThreadHandles);
        void StopThreads(const uint8 threadCount, const seThreadHandle* pThreadHandles);

        bool StartJobSystem();
        void StopJobSystem();
        JobSystem* GetJobSystem() { return &m_JobSystem; }

    protected:
        virtual bool InitializeInternal() final;
        virtual void StartInternal() final;
        virtual void StopInternal() final;
        virtual void ShutdownInternal() final;
    };
}
