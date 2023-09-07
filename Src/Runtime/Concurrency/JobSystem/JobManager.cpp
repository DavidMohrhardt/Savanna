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
                JobManager::Get().AwaitJobOrExecuteImmediateInternal(dependency);
            }
            return k_SavannaJobResultSuccess;
        }
    };

    void JobManager::ProcessJobs()
    {
        while (JobManager::Get().m_ProcessingJobs.load())
        {
            JobHandle jobHandle = k_InvalidJobHandle;

            {
#if !USE_LOCKLESS_CONCURRENCY_STRUCTURES
                if (!s_JobQueueMutex.try_lock())
                {
                    std::this_thread::yield();
                    continue;
                }
#endif
                if (!JobManager::Get().m_HighPriorityJobs.empty())
                {
                    jobHandle = JobManager::Get().m_HighPriorityJobs.front();
                    JobManager::Get().m_HighPriorityJobs.pop();
                }
                else if (!JobManager::Get().m_NormalPriorityJobs.empty())
                {
                    jobHandle = JobManager::Get().m_NormalPriorityJobs.front();
                    JobManager::Get().m_NormalPriorityJobs.pop();
                }
                else if (!JobManager::Get().m_LowPriorityJobs.empty())
                {
                    jobHandle = JobManager::Get().m_LowPriorityJobs.front();
                    JobManager::Get(). m_LowPriorityJobs.pop();
                }

                if (jobHandle != k_InvalidJobHandle)
                {
                    if (JobManager::Get().m_JobHandles.find(jobHandle) == JobManager::Get().m_JobHandles.end())
                    {
                        // Job has already been completed elsewhere either by a dependent that got dequeued before it
                        // or a manual await call. Skip it.
                        jobHandle = k_InvalidJobHandle;
                    }
                    else
                    {
                        JobManager::Get().m_JobHandles[jobHandle] = k_SavannaJobStateRunning;
                    }
                }

#if !USE_LOCKLESS_CONCURRENCY_STRUCTURES
                s_JobQueueMutex.unlock();
#endif
            }

            if (jobHandle != k_InvalidJobHandle && JobManager::Get().GetJobState(jobHandle) == JobState::k_SavannaJobStateRunning)
            {
                IJob* pJob = reinterpret_cast<IJob*>(jobHandle);
                switch (pJob->Execute())
                {
                    case k_SavannaJobResultSuccess:
                        pJob->OnComplete();
                        break;
                    case k_SavannaJobResultCancelled:
                        pJob->OnCancel();
                        break;
                    case k_SavannaJobResultError:
                    default:
                        pJob->OnError();
                        break;
                }
                JobManager::Get().OnJobCompletedInternal(jobHandle);
            }

            std::this_thread::yield();
        }
    }

    JobManager::JobManager()
        : m_ThreadPoolSize()
        , m_ProcessingJobs(false)
        , m_JobThreads()
        , m_HighPriorityJobs()
        , m_NormalPriorityJobs()
        , m_LowPriorityJobs()
        , m_JobHandles()
    {}

    JobManager::~JobManager() {}

    bool JobManager::InitializeInternal()
    {
        SAVANNA_INSERT_SCOPED_PROFILER(JobManager::InitializeInternal());
        m_ThreadPoolSize = (uint8)std::thread::hardware_concurrency() - 1;
        m_JobThreads.reserve(m_ThreadPoolSize);
        return true;
    }

    void JobManager::StartInternal()
    {
        SAVANNA_INSERT_SCOPED_PROFILER(JobManager::StartInternal());
        // If the atomic started bool is false, then we set it to true and spin up the threads.
        bool expected = false;
        if (m_ProcessingJobs.compare_exchange_weak(expected, true, std::memory_order_release))
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

    void JobManager::StopInternal()
    {
        SAVANNA_INSERT_SCOPED_PROFILER(JobManager::StopInternal());
        while (m_ProcessingJobs.exchange(false, std::memory_order_release))
        {
            {
#if !USE_LOCKLESS_CONCURRENCY_STRUCTURES
                std::lock_guard<std::mutex> lock(s_JobQueueMutex);
#endif
                for (int i = 0; i < m_JobThreads.size(); ++i)
                {
                    m_JobThreads[i].join();
                }
                m_JobThreads.clear();
            }

            for (auto& jobHandleStatePair : m_JobHandles)
            {
                if (!TryCancelJob(jobHandleStatePair.first))
                {
                    AwaitJobOrExecuteImmediateInternal(jobHandleStatePair.first);
                }
            }
        }
    }

    void JobManager::ShutdownInternal()
    {
        SAVANNA_INSERT_SCOPED_PROFILER(JobManager::ShutdownInternal());
        StopInternal();
        m_JobHandles.clear();
        m_JobThreads.clear();
    }

    /**
     * @brief Schedules a job to be executed by the job system.
     * The job pointer must remain valid until the job has finished
     * executing.
    */
    JobHandle JobManager::ScheduleJob(IJob *pJob, JobPriority priority, JobHandle dependency)
    {
        SAVANNA_INSERT_SCOPED_PROFILER(JobManager::ScheduleJob(IJob *pJob, JobPriority priority, JobHandle dependency));
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
        if (handle == k_InvalidJobHandle) SAVANNA_BRANCH_UNLIKELY
            return;

        SAVANNA_INSERT_SCOPED_PROFILER(JobManager::ScheduleJob(JobHandle &handle, JobPriority priority));

#if !USE_LOCKLESS_CONCURRENCY_STRUCTURES
        std::lock_guard<std::mutex> lock(s_JobQueueMutex);
#endif
        if (m_JobHandles.find(handle) != m_JobHandles.end()) SAVANNA_BRANCH_UNLIKELY
            return;

        m_JobHandles[handle] = k_SavannaJobStateReady;
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
        if (handles == nullptr || jobCount == 0) SAVANNA_BRANCH_UNLIKELY
            return;

        SAVANNA_INSERT_SCOPED_PROFILER(JobManager::ScheduleJobBatch(JobHandle *handles, size_t jobCount, JobPriority priority));
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
        if (jobHandle == k_InvalidJobHandle) SAVANNA_BRANCH_UNLIKELY
            return false;

        SAVANNA_INSERT_SCOPED_PROFILER(JobManager::TryCancelJob(JobHandle jobHandle));
#if !USE_LOCKLESS_CONCURRENCY_STRUCTURES
        std::lock_guard<std::mutex> lock(s_JobQueueMutex);
#endif
        auto it = m_JobHandles.find(jobHandle);
        if (it != m_JobHandles.end()) SAVANNA_BRANCH_LIKELY
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

            default:
                m_JobHandles.erase(it);
                break;
            }
        }
        return false;
    }

    void JobManager::AwaitCompletion(JobHandle jobHandle)
    {
        if (jobHandle == k_InvalidJobHandle) SAVANNA_BRANCH_UNLIKELY
            return;

        SAVANNA_INSERT_SCOPED_PROFILER(JobManager::AwaitCompletion(JobHandle jobHandle));
        do
        {
            {
#if !USE_LOCKLESS_CONCURRENCY_STRUCTURES
                std::lock_guard<std::mutex> lock(s_JobQueueMutex);
#endif
                if (m_JobHandles.find(jobHandle) == m_JobHandles.end())
                    return;
            }

            std::this_thread::yield();
        } while (true);
    }

    void JobManager::AwaitCompletion(JobHandle* pJobHandles, size_t jobCount)
    {
        JobHandle combinedJobHandles = CombineDependencies(pJobHandles, jobCount);
        AwaitCompletion(combinedJobHandles);
    }

    JobState JobManager::GetJobState(JobHandle jobHandle)
    {
        if (jobHandle == k_InvalidJobHandle) SAVANNA_BRANCH_UNLIKELY
            return k_SavannaJobStateInvalid;

        SAVANNA_INSERT_SCOPED_PROFILER(JobManager::GetJobState(JobHandle jobHandle));

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
        SAVANNA_INSERT_SCOPED_PROFILER(JobManager::CombineDependencies(JobHandle *handles, size_t jobCount));
        if (jobCount == 0 || handles == nullptr)
        {
            return k_InvalidJobHandle;
        }

        return ScheduleJob(
            SAVANNA_NEW(TemporaryJob<DependencyAwaiterJob>, std::vector<JobHandle>(handles, handles + jobCount)),
            k_SavannaJobPriorityHigh);
    }

    JobResult JobManager::AwaitJobOrExecuteImmediateInternal(JobHandle dependency)
    {
        SAVANNA_INSERT_SCOPED_PROFILER(JobManager::AwaitJobOrExecuteImmediateInternal(JobHandle dependency));
        if (dependency == k_InvalidJobHandle) SAVANNA_BRANCH_UNLIKELY
            return k_SavannaJobResultError;

        IJob* pJob = nullptr;
        {
#if !USE_LOCKLESS_CONCURRENCY_STRUCTURES
            std::lock_guard<std::mutex> lock(s_JobQueueMutex);
#endif

            if (m_JobHandles.find(dependency) != m_JobHandles.end() && m_JobHandles[dependency] == k_SavannaJobStateReady)
            {
                m_JobHandles[dependency] = k_SavannaJobStateRunning;
                pJob = reinterpret_cast<IJob*>(dependency);
            }
        }

        if (pJob != nullptr)
        {
            JobResult result = pJob->Execute();
            switch (result)
            {
                case k_SavannaJobResultSuccess:
                    pJob->OnComplete();
                    break;
                case k_SavannaJobResultCancelled:
                    pJob->OnCancel();
                    break;
                case k_SavannaJobResultError:
                default:
                    pJob->OnError();
                    break;
            }

            OnJobCompletedInternal(dependency);
            return result;
        }
        else
        {
            AwaitCompletion(dependency);
            return k_SavannaJobResultSuccess;
        }
    }

    void JobManager::OnJobCompletedInternal(JobHandle handle)
    {
        SAVANNA_INSERT_SCOPED_PROFILER(JobManager::OnJobCompletedInternal(JobHandle handle));
#if !USE_LOCKLESS_CONCURRENCY_STRUCTURES
        std::lock_guard<std::mutex> lock(s_JobQueueMutex);
#endif
        m_JobHandles.erase(handle);
    }
} // namespace Savanna::Concurrency
