/**
 * @file ConcurrencyTestUtils.h
 * @author David Mohrhardt (https://github.com/DavidMohrhardt/Savanna)
 * @brief TODO @David.Mohrhardt Document
 * @version 0.1
 * @date 2023-05-27
 *
 * @copyright Copyright (c) 2023
 *
 */
#pragma once

#include <gtest/gtest.h>

#include <atomic>
#include <chrono>
#include <mutex>
#include <thread>

namespace savanna::Core::Tests
{
    class ConcurrencyTestFramework : public ::testing::Test
    {
    private:
        std::vector<std::thread> m_ThreadStorage;

    protected:
        ConcurrencyTestFramework() = delete;
        ConcurrencyTestFramework(const ConcurrencyTestFramework&) = delete;
        ConcurrencyTestFramework& operator=(const ConcurrencyTestFramework&) = delete;

        ConcurrencyTestFramework(size_t threadCount)
            : m_ThreadStorage(threadCount)
        {
        }

        virtual ~ConcurrencyTestFramework()
        {
            for (auto& thread : m_ThreadStorage)
            {
                thread.join();
            }
        }

        template <typename Func>
        void RunThreads(Func&& func)
        {
            for (auto& thread : m_ThreadStorage)
            {
                thread = std::thread(func);
            }
        }

        void JoinThreads()
        {
            for (auto& thread : m_ThreadStorage)
            {
                thread.join();
            }
        }

        size_t GetThreadCount() const
        {
            return m_ThreadStorage.size();
        }
    };
}
