#include "ThreadManager.h"

#include "SavannaConcurrency.h"

namespace Savanna::Concurrency
{
    static const uint8 k_MaxThreadCount = std::thread::hardware_concurrency() - 1;

    ThreadManager::ThreadManager()
        : m_Threads{k_MaxThreadCount, k_SavannaMemoryLabelGeneral}
    {
    }

    ThreadManager::~ThreadManager()
    {
    }

    bool ThreadManager::TryReserveThreads(uint8 threadCount, se_ThreadId_t* pThreadIds)
    {
        // Can only reserve threads from the main thread
        SAVANNA_ASSERT_MAIN_THREAD();

        uint8 fulfilledThreadCount = 0;
        for (auto& reservedThread : m_Threads)
        {
            if (!reservedThread.m_IsReserved)
            {
                reservedThread.m_IsReserved = true;
                pThreadIds[fulfilledThreadCount] = fulfilledThreadCount;
                ++fulfilledThreadCount;
            }

            if (fulfilledThreadCount == threadCount)
            {
                break;
            }
        }

        if (fulfilledThreadCount != threadCount)
        {
            ReleaseThreads(fulfilledThreadCount, pThreadIds);
            return false;
        }

        return true;
    }

    Thread& ThreadManager::GetThread(se_ThreadId_t threadId)
    {
        return m_Threads[threadId].m_Thread;
    }

    void ThreadManager::ReleaseThreads(uint8 threadCount, se_ThreadId_t *pThreadIds)
    {
        // Can only release threads from the main thread
        SAVANNA_ASSERT_MAIN_THREAD();

        for (uint8 i = 0; i < threadCount; ++i)
        {
            m_Threads[pThreadIds[i]].m_Thread.Join();
            m_Threads[pThreadIds[i]].m_IsReserved = false;
        }
    }

    bool ThreadManager::InitializeInternal()
    {
        Info::Initialize();
        return true;
    }

    void ThreadManager::StartInternal()
    {
        m_Threads.clear();
        for (int i = 0; i < k_MaxThreadCount; ++i)
        {
            m_Threads.push_back(
                std::move(ThreadReservation{}));
        }
    }

    void ThreadManager::StopInternal()
    {
        for (auto& thread : m_Threads)
        {
            if (thread.m_Thread.IsRunning())
            {
                thread.m_Thread.Join();
            }
            thread.m_IsReserved = false;
        }
    }

    void ThreadManager::ShutdownInternal()
    {
        StopInternal();
        m_Threads.clear();
        Info::Reset();
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
        return ThreadManager::Get()->TryReserveThreads(requestedThreads, acquiredThreadHandles);
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
