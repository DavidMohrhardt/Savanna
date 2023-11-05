#pragma once

#include "Utilities/SavannaCoding.h"

#include <atomic>

namespace Savanna
{
    class SpinLock
    {
        std::atomic<bool> m_Lock;

    public:
        SpinLock() : m_Lock(false) {}
        SpinLock(const SpinLock&) = delete;
        SpinLock(SpinLock&&) = delete;

        SpinLock& operator=(const SpinLock&) = delete;
        SpinLock& operator=(SpinLock&&) = delete;

        struct ScopedLock
        {
            SpinLock& m_Lock;

            ScopedLock(SpinLock& lock) : m_Lock(lock) { m_Lock.Lock(); }
            ~ScopedLock() { m_Lock.Unlock(); }
        };

        void Lock()
        {
            while (m_Lock.exchange(true, std::memory_order_acquire))
            {
                // spin
            }
        }

        void Unlock()
        {
            m_Lock.store(false, std::memory_order_release);
        }

        ScopedLock Auto() { return ScopedLock(*this); }

        SAVANNA_NO_DISCARD bool TryLock()
        {
            return !m_Lock.exchange(true, std::memory_order_acquire);
        }

        SAVANNA_NO_DISCARD bool IsLocked() const
        {
            return m_Lock.load(std::memory_order_acquire);
        }

        SAVANNA_NO_DISCARD bool IsUnlocked() const
        {
            return !m_Lock.load(std::memory_order_acquire);
        }

        SAVANNA_NO_DISCARD operator bool() const
        {
            return IsLocked();
        }

        SAVANNA_NO_DISCARD bool operator!() const
        {
            return IsUnlocked();
        }
    };
} // namespace Savanna
