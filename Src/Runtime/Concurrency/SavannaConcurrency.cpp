/**
 * @file SavannaConcurrency.cpp
 * @author David Mohrhardt (https://github.com/DavidMohrhardt/Savanna)
 * @brief
 * @version 0.1
 * @date 2023-10-28
 *
 * @copyright Copyright (c) 2023
 *
 */
#include "SavannaConcurrency.h"

#include "JobManager.h"

#include <thread>

namespace Savanna::Concurrency
{
    static std::thread::id s_MainThreadId;

    void Info::Initialize()
    {
        static auto execOnceSentinel = []()
        {
            s_MainThreadId = std::this_thread::get_id();
            return true;
        }();
    }
} // namespace Savanna::Concurrency

using namespace Savanna::Concurrency;

SAVANNA_EXPORT(se_bool) SavannaConcurrencyIsMainThread()
{
    return s_MainThreadId == std::this_thread::get_id();
}

SAVANNA_EXPORT(se_JobHandle_t) SavannaConcurrencyJobManagerScheduleJob(
    se_IJobInterface_t* pJobInterface,
    se_JobPriority_t priority,
    se_JobHandle_t dependency)
{
    if (auto manager = JobManager::Get())
    {
        // return manager->ScheduleJobFromInterface(pJobInterface, priority, dependency);
    }
    return k_InvalidJobHandle;
}

SAVANNA_EXPORT(void) SavannaConcurrencyJobManagerAwaitJob(se_JobHandle_t jobHandle)
{
    if (auto manager = JobManager::Get())
        manager->AwaitCompletion(jobHandle);
}
