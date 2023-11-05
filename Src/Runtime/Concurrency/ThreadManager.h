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

#include "Public/ISavannaJobs.hpp"

#include <Types/Manager/GlobalManager.h>

#include "ConcurrencyCapabilities.h"

#include <Types/Containers/Arrays/dynamic_array.h>
#include <Types/Containers/Arrays/fixed_array.h>
#include <Types/Containers/Queues/atomic_queue.h>
#include <Types/Locks/SpinLock.h>

#include <atomic>

namespace Savanna::Concurrency
{
    using se_ThreadId_t = int64;

    class Thread
    {
    private:
        std::thread m_Thread;
        std::atomic_bool m_IsRunning;
        std::atomic_bool m_IsFinished;

        friend class std::thread;

        inline static void SleepThreadInternal()
        {
            std::this_thread::yield();
        }

    public:
        Thread()
            : m_Thread{SleepThreadInternal}
            , m_IsRunning{false}
            , m_IsFinished{false}
        {}

        ~Thread() = default;

        Thread(const Thread&) = delete;
        Thread& operator=(const Thread&) = delete;

        Thread(Thread&& other) noexcept
            : m_Thread{SleepThreadInternal}
            , m_IsRunning{false}
            , m_IsFinished{false}
        {
            *this = std::move(other);
        }

        Thread& operator=(Thread&& other) noexcept
        {
            if (this != &other)
            {
                m_Thread = std::move(other.m_Thread);
                m_IsRunning = other.m_IsRunning.load(std::memory_order_acquire);
                m_IsFinished = other.m_IsFinished.load(std::memory_order_acquire);

                other.m_IsRunning = false;
                other.m_IsFinished = false;
            }
            return *this;
        }

        template <typename Function, typename... Args>
        void Start(Function& function, Args&&... args)
        {
            m_IsRunning = true;
            m_IsFinished = false;
            m_Thread = std::thread(function, std::forward<Args>(args)...);
        }

        void Join()
        {
            m_Thread.join();
        }

        bool IsRunning() const noexcept
        {
            return m_IsRunning.load(std::memory_order_acquire);
        }

        bool IsFinished() const noexcept
        {
            return m_IsFinished.load(std::memory_order_acquire);
        }
    };

    class ThreadManager final : public GlobalManager<ThreadManager>
    {
    private:
        DEFINE_GLOBAL_MANAGER_FRIENDS_FOR(ThreadManager);

        struct ThreadReservation
        {
            Thread m_Thread;
            bool m_IsReserved;

            ThreadReservation()
                : m_Thread{}
                , m_IsReserved{false}
            {}
            ThreadReservation(const ThreadReservation&) = delete;

            ThreadReservation(ThreadReservation&& other) noexcept
                : m_Thread{std::move(other.m_Thread)}
                , m_IsReserved{other.m_IsReserved}
            {
                other.m_IsReserved = false;
            }

            ~ThreadReservation() = default;
        };

        dynamic_array<ThreadReservation> m_Threads;

    public:
        ThreadManager();
        ~ThreadManager();

        bool TryReserveThreads(uint8 threadCount, se_ThreadId_t* pThreadIds);
        void ReleaseThreads(uint8 threadCount, se_ThreadId_t* pThreadIds);

        uint8 GetReservedThreadCount() const noexcept;

        Thread& GetThread(se_ThreadId_t threadId);

    protected:
        virtual bool InitializeInternal() final;
        virtual void StartInternal() final;
        virtual void StopInternal() final;
        virtual void ShutdownInternal() final;
    };
}
