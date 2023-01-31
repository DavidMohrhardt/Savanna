#include "JobThread.h"

namespace Savanna::Threading::Job
{
    JobThread::JobThread()
    {
    }

    JobThread::~JobThread()
    {
        Join();
    }

    void JobThread::Start()
    {
        if (!m_Started.compare_exchange_strong(false, true))
        {
            if (m_Thread.joinable())
            {
                m_Thread.join();
            }

            m_Thread = std::thread(ProcessJobs, *this);
        }
    }

    void JobThread::Stop()
    {
        m_Started.store(false);
    }

    void JobThread::Join()
    {
        // Must stop the thread before joining it
        Stop();
        if (m_Thread.joinable())
        {
            m_Thread.join();
        }
    }

    void JobThread::Detach()
    {
        // Must stop the thread before detaching it
        Stop();
        if (m_Thread.joinable())
        {
            m_Thread.detach();
        }
    }

    void JobThread::ProcessJobs(const JobThread& executingThread)
    {
        // Ensure the job system exists while this thread is running
        std::shared_ptr<JobSystem> spJobSystem = JobSystem::Get();

        if (spJobSystem == nullptr)
        {
            return;
        }

        while (executingThread.m_Started.load())
        {
            executingThread.m_spActiveJob = spJobSystem->PollJobs();

            if (executingThread.m_spActiveJob != nullptr)
            {
                executingThread.m_spActiveJob->Execute();
                executingThread.m_spActiveJob = nullptr;
            }
            else
            {
#if USE_SLEEP_FOR
                std::this_thread::sleep_for(std::chrono::milliseconds(1));
#else
                std::this_thread::yield();
#endif
            }
        }
    }
}
