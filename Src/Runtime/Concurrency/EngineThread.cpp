#include "EngineThread.h"

namespace savanna::concurrency
{
    EngineThread::EngineThread()
        : m_Thread()
        , m_ExecutionInfo(nullptr)
    {}

    EngineThread::~EngineThread()
    {
        RequestStop();
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
            RequestStop();

            m_Thread = std::move(other.m_Thread);
            m_ExecutionInfo.store(other.m_ExecutionInfo.load(std::memory_order_acquire), std::memory_order_release);
            other.m_ExecutionInfo.store(nullptr, std::memory_order_release);
        }

        return *this;
    }

    void EngineThread::RunThread(
        std::stop_token stopToken,
        std::atomic<ThreadExecutionInterface*>& executionInterface)
    {
        while (!stopToken.stop_requested())
        {
            ThreadExecutionInterface* pExecInterface = executionInterface.load(std::memory_order_acquire);
            if (pExecInterface != nullptr && pExecInterface->m_pFunction != nullptr)
            {
                pExecInterface->m_pFunction(pExecInterface->m_pData);
            }
            else if (stopToken.stop_requested())
            {
                break;
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

    void EngineThread::Start()
    {
        m_Thread = std::jthread(RunThread, std::ref(m_ExecutionInfo));
    }

    void EngineThread::RequestStop()
    {
        if (IsActive())
        {
            m_Thread.request_stop();
        }
    }

} // namespace savanna::Concurrency
