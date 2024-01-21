/**
 * @file SavannaConcurrency.cpp
 * @author David Mohrhardt (https://github.com/DavidMohrhardt/Savanna)
 * @brief TODO @David.Mohrhardt Document
 * @version 0.1
 * @date 2023-10-28
 *
 * @copyright Copyright (c) 2023
 *
 */
#include "SavannaConcurrency.h"

#include "JobSystem.h"

#include <thread>

namespace savanna::concurrency
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
} // namespace savanna::Concurrency

#include "ThreadManager.h"

using namespace savanna::concurrency;

SAVANNA_EXPORT(bool) SavannaConcurrencyIsMainThread()
{
    return s_MainThreadId == std::this_thread::get_id();
}

SAVANNA_EXPORT(seJobHandle) SavannaConcurrencyJobSystemScheduleJob(
    seJobDefinition& jobDefinition,
    seJobPriority priority,
    seJobHandle dependency)
{
    if (auto manager = ThreadManager::Get())
    {
        return manager->GetJobSystem()->ScheduleJob(jobDefinition, priority, dependency);
    }
    return k_InvalidJobHandle;
}

SAVANNA_EXPORT(void) SavannaConcurrencyJobSystemAwaitJob(seJobHandle jobHandle)
{
    if (auto manager = ThreadManager::Get())
        manager->GetJobSystem()->AwaitCompletion(jobHandle);
}
