#include "JobManager.h"

#if !USE_LOCKLESS_CONCURRENCY_STRUCTURES
#include <mutex>
#endif

namespace Savanna::Concurrency
{

#if !USE_LOCKLESS_CONCURRENCY_STRUCTURES
    static std::mutex s_JobQueueMutex;
#endif

    class DependencyAwaiterJob : public IJob
    {
    private:
        std::vector<JobHandle> m_Dependencies;

    public:
        DependencyAwaiterJob(const std::vector<JobHandle>& dependencies)
            : m_Dependencies(dependencies)
        {}

        ~DependencyAwaiterJob() {}

        JobResult Execute() override
        {
            for (const auto& dependency : m_Dependencies)
            {
                JobManager::Get()->AwaitJobOrExecuteImmediateInternal(dependency);
            }
            return k_SavannaJobResultSuccess;
        }
    };

    void JobManager::ProcessJobs()
    {
        // SAVANNA_ASSERT(!ThreadManager::IsMainThread(), "JobManager::ProcessJobs() must not be called from the main thread.");
        auto pJobManager = Get();

        auto& lowPriorityJobQueue = pJobManager->m_LowPriorityJobs;
        auto& normalPriorityJobQueue = pJobManager->m_NormalPriorityJobs;
        auto& highPriorityJobQueue = pJobManager->m_HighPriorityJobs;

        auto& jobHandles = pJobManager->m_JobHandles;

        while (pJobManager->m_ProcessingJobs.load())
        {
            JobHandle jobHandle = k_InvalidJobHandle;

            {
#if !USE_LOCKLESS_CONCURRENCY_STRUCTURES
        std::lock_guard<std::mutex> lock(s_JobQueueMutex);
#endif
                if (!highPriorityJobQueue.empty())
                {
                    jobHandle = highPriorityJobQueue.front();
                    highPriorityJobQueue.pop();
                }
                else if (!normalPriorityJobQueue.empty())
                {
                    jobHandle = normalPriorityJobQueue.front();
                    normalPriorityJobQueue.pop();
                }
                else if (!lowPriorityJobQueue.empty())
                {
                    jobHandle = lowPriorityJobQueue.front();
                    lowPriorityJobQueue.pop();
                }

                // Check if the job has been removed from the queue
                if (jobHandle == k_InvalidJobHandle || jobHandles.find(jobHandle) == jobHandles.end())
                {
                    jobHandle = k_InvalidJobHandle;
                }
                else
                {
                    jobHandles[jobHandle] = k_SavannaJobStateRunning;
                }
            }

            if (jobHandle != k_InvalidJobHandle && pJobManager->GetJobState(jobHandle) == JobState::k_SavannaJobStateReady)
            {
                JobRunner runner { reinterpret_cast<IJob*>(jobHandle) };
                runner.Run();
            }

            std::this_thread::yield();
        }
    }

    JobManager::JobManager(uint8 threadPoolSize)
        : m_ThreadPoolSize(threadPoolSize)
        , m_ProcessingJobs(false)
        , m_JobThreads()
        , m_HighPriorityJobs()
        , m_NormalPriorityJobs()
        , m_LowPriorityJobs()
        , m_JobHandles()
    {
        SAVANNA_ASSERT(
            threadPoolSize > 0,
            "JobManager::JobManager: threadPoolSize must be greater than 0.");

        // SAVANNA_ASSERT(
        //     threadPoolSize <= std::thread::hardware_concurrency - 1,
        //     "JobManager::JobManager: threadPoolSize must be less than the number of available cores - 1 for the main thread.");
    }

    JobManager::~JobManager()
    {
        Stop(true);
    }

    void JobManager::Start()
    {
        // If the atomic started bool is false, then we set it to true and spin up the threads.
        bool isNotStarted = m_ProcessingJobs.exchange(true, std::memory_order_release);
        if (isNotStarted)
        {
#if !USE_LOCKLESS_CONCURRENCY_STRUCTURES
            std::lock_guard<std::mutex> lock(s_JobQueueMutex);
#endif
            m_JobThreads.resize(m_ThreadPoolSize);
            for (int i = 0; i < m_JobThreads.size(); ++i)
            {
                m_JobThreads[i] = std::thread(ProcessJobs);
            }
        }
    }

    void JobManager::Stop(bool synchronized)
    {
        while (m_ProcessingJobs.exchange(false, std::memory_order_release))
        {
#if !USE_LOCKLESS_CONCURRENCY_STRUCTURES
            std::lock_guard<std::mutex> lock(s_JobQueueMutex);
#endif
            // for (const auto& thread : m_JobThreads)
            for (int i = 0; i < m_JobThreads.size(); ++i)
            {
                if (synchronized && m_JobThreads[i].joinable())
                {
                    m_JobThreads[i].join();
                }
                else
                {
                    m_JobThreads[i].detach();
                }
            }
            m_JobThreads.clear();
            return;
        }
    }

    /**
     * @brief Schedules a job to be executed by the job system.
     * The job pointer must remain valid until the job has finished
     * executing.
    */
    JobHandle JobManager::ScheduleJob(IJob *pJob, JobPriority priority, JobHandle dependency)
    {
        JobHandle handle = k_InvalidJobHandle;

        if (pJob != nullptr)
        {
            // since handles are opaque memory addresses, we can just use the job pointer as the handle.
            handle = reinterpret_cast<JobHandle>(pJob);
            ScheduleJob(handle, priority);
        }

        return handle;
    }

    void JobManager::ScheduleJob(JobHandle &handle, JobPriority priority)
    {
#if !USE_LOCKLESS_CONCURRENCY_STRUCTURES
        std::lock_guard<std::mutex> lock(s_JobQueueMutex);
#endif
        switch (priority)
        {
        case k_SavannaJobPriorityHigh:
            m_HighPriorityJobs.push(handle);
            break;
        case k_SavannaJobPriorityNormal:
            m_NormalPriorityJobs.push(handle);
            break;
        case k_SavannaJobPriorityLow:
        default:
            m_LowPriorityJobs.push(handle);
            break;
        }
    }

    void JobManager::ScheduleJobBatch(JobHandle *handles, size_t jobCount, JobPriority priority)
    {
#if !USE_LOCKLESS_CONCURRENCY_STRUCTURES
        std::lock_guard<std::mutex> lock(s_JobQueueMutex);
#endif

        switch (priority)
        {
        case k_SavannaJobPriorityHigh:
            for (size_t i = 0; i < jobCount; ++i)
            {
                m_HighPriorityJobs.push(handles[i]);
            }
            break;
        case k_SavannaJobPriorityNormal:
            for (size_t i = 0; i < jobCount; ++i)
            {
                m_NormalPriorityJobs.push(handles[i]);
            }
            break;
        case k_SavannaJobPriorityLow:
        default:
            for (size_t i = 0; i < jobCount; ++i)
            {
                m_LowPriorityJobs.push(handles[i]);
            }
            break;
        }
    }

    bool JobManager::TryCancelJob(JobHandle jobHandle) noexcept
    {
#if !USE_LOCKLESS_CONCURRENCY_STRUCTURES
        std::lock_guard<std::mutex> lock(s_JobQueueMutex);
#endif
        auto it = m_JobHandles.find(jobHandle);
        if (it != m_JobHandles.end())
        {
            switch (it->second)
            {
            case k_SavannaJobStateReady:
                m_JobHandles.erase(it);
                return true;

            case k_SavannaJobStateRunning:
                if (reinterpret_cast<IJob*>(jobHandle)->TryCancel())
                {
                    m_JobHandles.erase(it);
                    return true;
                }
                break;

            case k_SavannaJobStateCompleted:
                return true;

            default:
                break;
            }
        }
        return false;
    }

    void JobManager::AwaitCompletion(JobHandle jobHandle)
    {
        while (GetJobState(jobHandle) != k_SavannaJobStateCompleted)
        {
            std::this_thread::yield();
        }
    }

    JobState JobManager::GetJobState(JobHandle jobHandle)
    {
#if !USE_LOCKLESS_CONCURRENCY_STRUCTURES
        std::lock_guard<std::mutex> lock(s_JobQueueMutex);
#endif

        auto it = m_JobHandles.find(jobHandle);
        if (it != m_JobHandles.end())
        {
            return it->second;
        }
        return k_SavannaJobStateInvalid;
    }

    JobHandle JobManager::CombineDependencies(
        JobHandle *handles,
        size_t jobCount)
    {
        if (jobCount == 0 || handles == nullptr)
        {
            return k_InvalidJobHandle;
        }

        return ScheduleJob(
            new AutomaticJob<DependencyAwaiterJob>(std::vector<JobHandle>(handles, handles + jobCount)),
            k_SavannaJobPriorityHigh);
    }

    JobResult JobManager::AwaitJobOrExecuteImmediateInternal(JobHandle dependency)
    {
        IJob* pJob = nullptr;
        {
#if !USE_LOCKLESS_CONCURRENCY_STRUCTURES
            std::lock_guard<std::mutex> lock(s_JobQueueMutex);
#endif

            JobState dependencyState = m_JobHandles[dependency];
            if (dependencyState == k_SavannaJobStateReady)
            {
                m_JobHandles[dependency] = k_SavannaJobStateRunning;
                pJob = reinterpret_cast<IJob*>(dependency);
            }
        }

        if (pJob != nullptr)
        {
            return pJob->Execute();
        }
        else
        {
            AwaitCompletion(dependency);
            return k_SavannaJobResultSuccess;
        }
    }
} // namespace Savanna::Concurrency

