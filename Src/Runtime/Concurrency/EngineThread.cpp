#include "EngineThread.h"

namespace savanna::concurrency
{
    EngineThread::EngineThread()
        : m_pThread(nullptr)
        , m_ExecutionInfo(nullptr)
        , m_Active(false)
    {}

    EngineThread::~EngineThread()
    {
        Stop();
    }

    EngineThread::EngineThread(EngineThread&& other) noexcept
        : EngineThread()
    {
        *this = std::move(other);
    }

    EngineThread &EngineThread::operator=(EngineThread &&other) noexcept
    {
        if (this != &other)
        {
            Stop();

            m_pThread = other.m_pThread;
            m_ExecutionInfo.store(other.m_ExecutionInfo.load(std::memory_order_acquire), std::memory_order_release);
            m_Active.store(other.m_Active.load(std::memory_order_acquire), std::memory_order_release);

            other.m_pThread = nullptr;
            other.m_ExecutionInfo.store(nullptr, std::memory_order_release);
            other.m_Active.store(false, std::memory_order_release);
        }

        return *this;
    }

    void EngineThread::RunThread(EngineThread* pThread)
    {
        while (pThread->m_Active.load(std::memory_order_acquire))
        {
            ThreadExecutionInterface* pExecInterface = pThread->m_ExecutionInfo.load(std::memory_order_acquire);
            if (pExecInterface != nullptr && pExecInterface->m_pFunction != nullptr)
            {
                pExecInterface->m_pFunction(pExecInterface->m_pData);
            }
            else
            {
                std::this_thread::yield();
            }
        }
    }

    void EngineThread::SetExecutionInterface(ThreadExecutionInterface *pJobInterface)
    {
        m_ExecutionInfo.store(pJobInterface, std::memory_order_release);
    }

    void EngineThread::Start(std::thread *pThreadBuffer) {
        bool expected = false;
        if (m_Active.compare_exchange_strong(expected, true, std::memory_order_acq_rel))
        {
            // The address of the thread object is passed in so that the memory can be reused
            m_pThread = new (pThreadBuffer) std::thread(RunThread, this);
        }
    }

    void EngineThread::Stop()
    {
        m_Active.store(false, std::memory_order_release);
        if (m_pThread != nullptr)
        {
            // Ensure the thread has been torn down but the address should still be valid
            m_pThread->join();
            m_pThread->~thread();
            m_pThread = nullptr;
        }
    }

} // namespace savanna::Concurrency
