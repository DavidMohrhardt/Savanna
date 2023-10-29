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

#define SAVANNA_ASSERT_MAIN_THREAD() SAVANNA_ASSERT(SavannaConcurrencyIsMainThread(), "This function must be called from the main thread");

namespace Savanna::Concurrency
{
    static std::thread::id s_MainThreadId;

    void Info::Initialize()
    {
        s_MainThreadId = std::this_thread::get_id();
    }

    void Info::Reset()
    {
        SAVANNA_ASSERT_MAIN_THREAD();
        s_MainThreadId = std::thread::id();
    }
} // namespace Savanna::Concurrency

using namespace Savanna::Concurrency;

SAVANNA_EXPORT(se_bool) SavannaConcurrencyIsMainThread()
{
    return s_MainThreadId == std::this_thread::get_id();
}
