#include "ThreadManager.h"

#include "SavannaConcurrency.h"

namespace Savanna::Concurrency
{
    static const uint8 k_MaxThreadCount = std::thread::hardware_concurrency() - 1;

    ThreadExecutionInterface* ThreadManager::s_pDefaultUnreservedThreadInterface = nullptr;

    DEFINE_ENUM(se_ReservationState_t, ReservationState, uint8_t,
        Unreserved,
        Awaiting,
        Reserved);

    inline void ThreadManager::StartUnreservedThreadsInternal()
    {
        dynamic_array<EngineThread>& threadPool = m_ThreadPool;
        dynamic_array<std::atomic_uint8_t>& reservationStates = m_ReservationStates;
        std::thread* pScratchBuffer = reinterpret_cast<std::thread*>(m_ThreadScratchBuffer.GetBuffer());

        for (int i = 0; i < k_MaxThreadCount; ++i)
        {
            auto& thread = threadPool[i];
            auto& reservationState = reservationStates[i];

            uint8 expected = Unreserved;
            if (reservationState.compare_exchange_strong(expected, Awaiting))
            {
                thread.SetExecutionInterface(s_pDefaultUnreservedThreadInterface);

                if (s_pDefaultUnreservedThreadInterface == nullptr && thread.IsActive())
                {
                    thread.Stop();
                }
                else
                {
                    thread.Start(pScratchBuffer + i);
                }

                // Explicitly set the thread to unreserved so it can be reserved but
                // passively allowed to execute jobs.
                reservationState.store(Unreserved, std::memory_order_release);
            }
        }
    }

    void ThreadManager::SetUnreservedThreadDefaultExecution(ThreadExecutionInterface *pExecutionInterface)
    {
        SAVANNA_ASSERT_MAIN_THREAD();

        s_pDefaultUnreservedThreadInterface = pExecutionInterface;
        Get()->StartUnreservedThreadsInternal();
    }

    ThreadManager::ThreadManager()
        : m_ThreadScratchBuffer{k_SavannaMemoryLabelGeneral}
        , m_ThreadPool{0, k_SavannaMemoryLabelGeneral}
        , m_ReservationStates{0, k_SavannaMemoryLabelGeneral}
        , m_ReservedThreadCount(0)
    {}

    ThreadManager::~ThreadManager()
    {
    }

    bool ThreadManager::TryAcquireThreads(const uint8 requestedThreads, se_ThreadHandle_t *pOutThreadHandles)
    {
        SAVANNA_ASSERT_MAIN_THREAD();
        if (requestedThreads > k_MaxThreadCount)
        {
            return false;
        }

        if (m_ReservedThreadCount + requestedThreads > k_MaxThreadCount)
        {
            return false;
        }

        uint8 reservedThreads = 0;
        for (int i = 0; i < k_MaxThreadCount; ++i)
        {
            auto& thread = m_ThreadPool[i];
            auto& reservationState = m_ReservationStates[i];

            uint8 expected = Unreserved;
            if (reservationState.compare_exchange_strong(expected, Awaiting))
            {
                thread.Stop();
                pOutThreadHandles[reservedThreads++] = (se_ThreadHandle_t)i;
                reservationState.store(Reserved, std::memory_order_release);
            }

            if (reservedThreads == requestedThreads)
            {
                break;
            }
        }
        m_ReservedThreadCount += reservedThreads;
        return true;
    }

    void ThreadManager::ReleaseThreads(const uint8 threadCount, const se_ThreadHandle_t *pThreadHandles)
    {
        SAVANNA_ASSERT_MAIN_THREAD();
        for (int i = 0; i < threadCount; ++i)
        {
            auto& thread = m_ThreadPool[pThreadHandles[i]];
            auto& reservationState = m_ReservationStates[pThreadHandles[i]];
            uint8 expected = Reserved;
            if (reservationState.compare_exchange_strong(expected, Awaiting))
            {
                thread.SetExecutionInterface(s_pDefaultUnreservedThreadInterface);
                m_ReservedThreadCount--;
            }
        }
    }

    void ThreadManager::SetThreadExecutionInterface(
        const uint8 threadCount, const se_ThreadHandle_t *pThreadHandles,
        ThreadExecutionInterface *pExecutionInterface)
    {
        if (pThreadHandles != nullptr)
        {
            for (int i = 0; i < threadCount; ++i)
            {
                auto& thread = m_ThreadPool[pThreadHandles[i]];
                auto& reservationState = m_ReservationStates[pThreadHandles[i]];
                uint8 expected = Reserved;
                if (reservationState.compare_exchange_strong(expected, Awaiting))
                {
                    thread.SetExecutionInterface(pExecutionInterface);
                    reservationState.store(Reserved, std::memory_order_release);
                }
            }
        }
    }

    void ThreadManager::StartThreads(const uint8 threadCount, const se_ThreadHandle_t *pThreadHandles)
    {
        if (pThreadHandles != nullptr)
        {
            auto scratchBuffer = reinterpret_cast<std::thread*>(m_ThreadScratchBuffer.GetBuffer());
            for (int i = 0; i < threadCount; ++i)
            {
                auto& thread = m_ThreadPool[pThreadHandles[i]];
                auto& reservationState = m_ReservationStates[pThreadHandles[i]];
                uint8 expected = Reserved;
                if (reservationState.compare_exchange_strong(expected, Awaiting))
                {
                    if (thread.IsActive())
                    {
                        thread.Stop();
                    }
                    thread.Start(scratchBuffer + pThreadHandles[i]);
                    reservationState.store(Reserved, std::memory_order_release);
                }
            }
        }
    }

    bool ThreadManager::InitializeInternal()
    {
        Info::Initialize();
        m_ThreadScratchBuffer = MemoryBuffer{sizeof(std::thread) * k_MaxThreadCount, k_SavannaMemoryLabelGeneral};
        m_ThreadPool.resize_initialized(k_MaxThreadCount);
        m_ReservationStates.resize_initialized(k_MaxThreadCount);

        // Zero out the thread scratch buffer
        ::memset(m_ThreadScratchBuffer.GetBuffer(), 0, m_ThreadScratchBuffer.GetSize());

        return true;
    }

    void ThreadManager::StartInternal()
    {
        StartUnreservedThreadsInternal();
    }

    void ThreadManager::StopInternal()
    {
        for (auto& thread : m_ThreadPool)
        {
            thread.Stop();
        }
    }

    void ThreadManager::ShutdownInternal()
    {
        StopInternal();
        m_ThreadPool.clear();
        m_ReservationStates.clear();
        m_ThreadScratchBuffer = MemoryBuffer{k_SavannaMemoryLabelGeneral};
    }
} // namespace Savanna::Concurrency

// PUBLIC API
using namespace Savanna::Concurrency;

SAVANNA_EXPORT(se_bool) SavannaConcurrencyThreadManagerTryAcquireThreads(
    se_uint8 requestedThreads,
    se_ThreadHandle_t* acquiredThreadHandles)
{
    if (ThreadManager::Get() != nullptr)
    {
        return ThreadManager::Get()->TryAcquireThreads(requestedThreads, acquiredThreadHandles);
    }

    return false;
}

SAVANNA_EXPORT(void) SavannaConcurrencyThreadManagerReleaseThreads(
    se_uint8 handleCount,
    se_ThreadHandle_t* pThreadHandles)
{
    if (ThreadManager::Get() != nullptr)
    {
        ThreadManager::Get()->ReleaseThreads(handleCount, pThreadHandles);
    }
}
