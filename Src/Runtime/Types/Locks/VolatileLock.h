#pragma once

// Idea gathered from Game Engine Architecture 3E by Jason Gregory pg. 326

#include <assert.h>

// #include <Utilities/PreprocessorDefinitions/InterfaceDefinitions.h>
#include "Utilities/SavannaCoding.h"

namespace Savanna
{
    class VolatileLock
    {
    private:
        // Prevent optimization to ensure SAVANNA_ASSERTions catch overlapping accesses to critical section by threads.
        volatile bool m_Locked;

    public:
        /**
         * @brief Acquire the volatile lock if released.
         */
        void Acquire()
        {
            SAVANNA_ASSERT(!m_Locked, "Volatile lock is already acquired. Critical section integrity is compromised.");
            m_Locked = true;
        }

        /**
         * @brief Release the volatile lock if acquired.
         */
        void Release()
        {
            SAVANNA_ASSERT(m_Locked, "Volatile lock is already released. Critical section integrity is compromised.");
            m_Locked = false;
        }

        /**
         * @brief Check if the volatile lock is acquired.
         *
         * @return true if the volatile lock is acquired.
         * @return false if the volatile lock is released.
         */
        SAVANNA_NO_DISCARD inline bool IsAcquired() const noexcept
        {
            return m_Locked;
        }

        SAVANNA_NO_DISCARD VolatileLock& AwaitLock() noexcept
        {
            while (IsAcquired())
            {
                // Spin
            }
            Acquire();
            return *this;
        }
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
