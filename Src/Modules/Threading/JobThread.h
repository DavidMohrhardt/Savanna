/**
 * @file JobThread.h
 * @author David Mohrhardt (https://github.com/DavidMohrhardt/Savanna)
 * @brief
 * @version 0.1
 * @date 2023-01-20
 *
 * @copyright Copyright (c) 2023
 *
 */
#pragma once

#include <SavannaEngine.h>
#include <Utilities/SavannaCoding.h>

#include <atomic>
#include <thread>

namespace Savanna::Threading::Jobs
{
    class JobThread //: public ThreadHandler
    {
        friend class JobSystem;

    private:
        static void ProcessJobs(const JobThread& executingThread);

    private:
        std::thread m_Thread;
        std::atomic_bool m_Started;

        std::shared_ptr<IJob> m_spActiveJob;

    public:
        JobThread();
        ~JobThread();

    private:
        void Start();
        void Stop();
        void Join();
        void Detach();
    };
} // namespace Savanna::Threading::Jobs

