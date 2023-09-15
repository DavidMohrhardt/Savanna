#include "JobManager.h"
#include "Types/Containers/Arrays/DynamicArray.h"

namespace Savanna::Concurrency
{
    class DependencyAwaiterJob final : public IJob
    {
    private:
        DynamicArray<JobHandle> m_Dependencies;

    public:
        DependencyAwaiterJob(const JobHandle* dependencies, const size dependencyCount)
            : m_Dependencies(dependencyCount, false)
        {
            for (size_t i = 0; i < dependencyCount; ++i)
            {
                m_Dependencies.Append(dependencies[i]);
            }
        }

        virtual ~DependencyAwaiterJob() override {}

        JobResult Execute() override
        {
            for (const auto& dependency : m_Dependencies)
            {
                JobManager::Get().AwaitJobOrExecuteImmediateInternal(dependency);
            }

            return k_SavannaJobResultSuccess;
        }

        void OnComplete() override
        {
            this->Dispose();
        }

        void OnError() override
        {
            this->Dispose();
        }

        void OnCancel() override
        {
            this->Dispose();
        }

    private:
        void Dispose()
        {
            SAVANNA_DELETE(this);
        }
    };

    class DependentJobWrapper : public IJob
    {
    private:
        friend class IJob;
        IJob* m_Job;
        JobHandle m_Dependency;
    public:
        DependentJobWrapper() = delete;

        DependentJobWrapper(JobHandle dependency, IJob* job)
            : m_Job(job)
            , m_Dependency(dependency)
        {}

        virtual ~DependentJobWrapper() override {}

        JobResult Execute() override
        {
            JobManager::Get().AwaitJobOrExecuteImmediateInternal(m_Dependency);
            return m_Job->Execute();
        }

        void OnComplete() override
        {
            this->Dispose();
        }

        void OnError() override
        {
            this->Dispose();
        }

        void OnCancel() override
        {
            this->Dispose();
        }

    private:
        void Dispose()
        {
            SAVANNA_DELETE(this);
        }
    };

    void JobManager::ProcessJobs()
    {
        while (JobManager::Get().m_ProcessingJobs.load())
        {
            JobHandle jobHandle = k_InvalidJobHandle;

            {
                // In the current implementation of the lockless queue you can simply pop from the queue
                if (JobManager::Get().m_HighPriorityJobs.TryPop(jobHandle) ||
                    JobManager::Get().m_NormalPriorityJobs.TryPop(jobHandle) ||
                    JobManager::Get().m_LowPriorityJobs.TryPop(jobHandle))
                {
                    auto sentinel = JobManager::Get().m_JobHandlesLock.Auto();
                    if (JobManager::Get().m_JobHandles.find(jobHandle) == JobManager::Get().m_JobHandles.end())
                    {
                        // Job has already been completed elsewhere either by a dependent that got dequeued before it
                        // or a manual await call. Skip it.
                        jobHandle = k_InvalidJobHandle;
                        continue;
                    }
                    else
                    {
                        JobManager::Get().m_JobHandles[jobHandle] = k_SavannaJobStateRunning;
                    }
                }
            }

            if (jobHandle != k_InvalidJobHandle && JobManager::Get().GetJobState(jobHandle) == JobState::k_SavannaJobStateRunning)
            {
                IJob* pJob = reinterpret_cast<IJob*>(jobHandle);
                printf("Executing job with ptr: %p\n", pJob);
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
                m_JobHandlesLock.Auto();
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

        if (pJob != nullptr) SAVANNA_BRANCH_LIKELY
        {
            if (dependency != k_InvalidJobHandle)
            {
                pJob = SAVANNA_NEW(DependentJobWrapper, dependency, pJob);
            }

            // since handles are opaque memory addresses, we can just use the job pointer as the handle.
            handle = reinterpret_cast<JobHandle>(pJob);
            ScheduleJob(handle, priority);
        }

        return handle;
    }

    inline void JobManager::ScheduleJob(JobHandle &handle, JobPriority priority)
    {
        if (handle == k_InvalidJobHandle) SAVANNA_BRANCH_UNLIKELY
            return;

        SAVANNA_INSERT_SCOPED_PROFILER(JobManager::ScheduleJob(JobHandle &handle, JobPriority priority));

        m_JobHandlesLock.Auto();
        if (m_JobHandles.find(handle) != m_JobHandles.end()) SAVANNA_BRANCH_UNLIKELY
            return;

        m_JobHandles[handle] = k_SavannaJobStateReady;
        switch (priority)
        {
        case k_SavannaJobPriorityHigh:
            m_HighPriorityJobs.Push(std::move(handle));
            break;
        case k_SavannaJobPriorityNormal:
            m_NormalPriorityJobs.Push(std::move(handle));
            break;
        case k_SavannaJobPriorityLow:
        default:
            m_LowPriorityJobs.Push(std::move(handle));
            break;
        }
    }

    JobHandle JobManager::ScheduleJobBatch(IJob** pJobs, const size& jobCount, JobPriority priority, JobHandle dependency)
    {
        if (pJobs == nullptr || jobCount == 0) SAVANNA_BRANCH_UNLIKELY
            return k_InvalidJobHandle;

        SAVANNA_INSERT_SCOPED_PROFILER(JobManager::ScheduleJobBatch(IJob* pJobs, const size& jobCount, JobPriority priority, JobHandle dependency));

        JobHandle outJobHandle = k_InvalidJobHandle;

        auto& jobQueue = priority == k_SavannaJobPriorityHigh ? m_HighPriorityJobs
            : priority == k_SavannaJobPriorityNormal ? m_NormalPriorityJobs
            : m_LowPriorityJobs;

        DynamicArray<JobHandle> handles(jobCount, false);
        {
            m_JobHandlesLock.Auto();
            for (size_t i = 0; i < jobCount; ++i)
            {
                JobHandle handle = k_InvalidJobHandle;
                if (dependency != k_InvalidJobHandle)
                {
                    handle = reinterpret_cast<JobHandle>(SAVANNA_NEW(DependentJobWrapper, dependency, pJobs[i]));
                }
                else
                {
                    handle = reinterpret_cast<JobHandle>(pJobs[i]);
                    if (m_JobHandles.find(handle) != m_JobHandles.end()) SAVANNA_BRANCH_UNLIKELY
                    {
                        continue;
                    }
                }

                m_JobHandles[handle] = k_SavannaJobStateReady;
                jobQueue.Push(std::move(handle));
                handles.Append(handle);
            }

            DependencyAwaiterJob* pDependencyAwaiterJob = SAVANNA_NEW(DependencyAwaiterJob, handles.Data(), handles.Size());
            outJobHandle = reinterpret_cast<JobHandle>(pDependencyAwaiterJob);

            m_JobHandles[outJobHandle] = k_SavannaJobStateReady;
            jobQueue.Push(std::move(outJobHandle));
        }

        return outJobHandle;
    }

    void JobManager::AwaitCompletion(JobHandle jobHandle)
    {
        if (jobHandle == k_InvalidJobHandle) SAVANNA_BRANCH_UNLIKELY
            return;

        SAVANNA_INSERT_SCOPED_PROFILER(JobManager::AwaitCompletion(JobHandle jobHandle));
        do
        {
            {
                auto sentinel = m_JobHandlesLock.Auto();
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

    bool JobManager::TryCancelJob(JobHandle jobHandle) noexcept
    {
        if (jobHandle == k_InvalidJobHandle) SAVANNA_BRANCH_UNLIKELY
            return false;

        SAVANNA_INSERT_SCOPED_PROFILER(JobManager::TryCancelJob(JobHandle jobHandle));
        auto sentinel = m_JobHandlesLock.Auto();
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

    JobState JobManager::GetJobState(JobHandle jobHandle)
    {
        if (jobHandle == k_InvalidJobHandle) SAVANNA_BRANCH_UNLIKELY
            return k_SavannaJobStateInvalid;

        SAVANNA_INSERT_SCOPED_PROFILER(JobManager::GetJobState(JobHandle jobHandle));

        m_JobHandlesLock.Auto();
        auto it = m_JobHandles.find(jobHandle);
        if (it != m_JobHandles.end())
        {
            return it->second;
        }
        return k_SavannaJobStateInvalid;
    }

    JobHandle JobManager::CombineDependencies(
        const JobHandle *handles,
        size_t jobCount)
    {
        SAVANNA_INSERT_SCOPED_PROFILER(JobManager::CombineDependencies(JobHandle *handles, size_t jobCount));
        if (jobCount == 0 || handles == nullptr)
        {
            return k_InvalidJobHandle;
        }

        return ScheduleJob(SAVANNA_NEW(DependencyAwaiterJob, handles, jobCount), k_SavannaJobPriorityHigh);
    }

    JobResult JobManager::AwaitJobOrExecuteImmediateInternal(JobHandle dependency)
    {
        SAVANNA_INSERT_SCOPED_PROFILER(JobManager::AwaitJobOrExecuteImmediateInternal(JobHandle dependency));
        if (dependency == k_InvalidJobHandle) SAVANNA_BRANCH_UNLIKELY
            return k_SavannaJobResultError;

        IJob* pJob = nullptr;
        {
            m_JobHandlesLock.Auto();

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

    void JobManager::OnJobCompletedInternal(JobHandle handle) {
        SAVANNA_INSERT_SCOPED_PROFILER(JobManager::OnJobCompletedInternal(JobHandle handle));
        auto sentinel = m_JobHandlesLock.Auto();
        m_JobHandles.erase(handle);
    }
} // namespace Savanna::Concurrency
