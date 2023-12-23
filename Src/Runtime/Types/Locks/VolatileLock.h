/**
 * @file VolatileLock.h
 * @author David Mohrhardt (https://github.com/DavidMohrhardt/Savanna)
 * @brief TODO @David.Mohrhardt Document
 * @version 0.1
 * @date 2023-09-25
 *
 */
#pragma once

// Idea gathered from Game Engine Architecture 3E by Jason Gregory pg.326

#include <assert.h>

#include "Utilities/SavannaCoding.h"

namespace savanna
{
    /**
     * @brief A lock that leverages the properties of the volatile keyword to ensure that the compiler
     * does not optimize away the lock or any of its accesses.
     *
     * @note This lock is not actually a lock. Instead it merely acts as a sentinel for catching
     * overlapping accesses to a critical section by multiple threads when they should not be
     * overlapping.
     *
     */
    class VolatileLock
    {
    private:
        // Prevent optimization to ensure SAVANNA_ASSERTions catch overlapping accesses to critical section by threads.
        volatile bool m_Locked = false;

    public:
        VolatileLock() = default;
        ~VolatileLock() = default;

        VolatileLock(const VolatileLock&) = delete;
        VolatileLock(VolatileLock&&) = delete;

        /**
         * @brief Acquire the volatile lock if released.
         */
        void Acquire() SAVANNA_NOEXCEPT;

        /**
         * @brief Release the volatile lock if acquired.
         */
        void Release() SAVANNA_NOEXCEPT;

        /**
         * @brief Check if the volatile lock is acquired.
         *
         * @return true if the volatile lock is acquired.
         * @return false if the volatile lock is released.
         */
        SAVANNA_NO_DISCARD inline bool IsAcquired() const SAVANNA_NOEXCEPT;

        /**
         * @brief Await the volatile lock to be released.
         *
         * @return VolatileLock& A reference to the volatile lock.
         */
        SAVANNA_NO_DISCARD VolatileLock& AwaitLock() SAVANNA_NOEXCEPT;
    };

    class VolatileLockGuard
    {
    private:
        VolatileLock& m_Lock;

    public:
        VolatileLockGuard(VolatileLock& lock)
            : m_Lock(lock.AwaitLock())
        {
        }

        ~VolatileLockGuard()
        {
            m_Lock.Release();
        }
    };
} // namespace Savanna
