// #include "JobManager.h"

#if !USE_LOCKLESS_CONCURRENCY_STRUCTURES
#include <mutex>

using lock_sentinel = std::lock_guard<std::mutex>;

#define LOCK_GUARD(__mutex) lock_sentinel(__mutex);

#else

#define LOCK_GUARD(__mutex)

#endif

void ProcessJobsThreaded()
{
    JobHandle jobHandle = k_InvalidJobHandle;
    while (pJobManager->m_ProcessingJobs.load())
}
