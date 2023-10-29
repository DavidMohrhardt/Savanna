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

    class AtomicCounter
    {
        std::atomic<int> m_Counter;

    public:
        AtomicCounter() : m_Counter(0) {}
        AtomicCounter(const AtomicCounter&) = delete;
        AtomicCounter(AtomicCounter&&) = delete;

        AtomicCounter& operator=(const AtomicCounter&) = delete;
        AtomicCounter& operator=(AtomicCounter&&) = delete;

        struct ScopedIncrementer
        {
            AtomicCounter& m_Counter;

            ScopedIncrementer(AtomicCounter& lock) : m_Counter(lock) { m_Counter.Increment(); }
            ~ScopedIncrementer() { m_Counter.Decrement(); }
        };

        void Increment()
        {
            m_Counter.fetch_add(1, std::memory_order_acquire);
        }

        void Decrement()
        {
            m_Counter.fetch_sub(1, std::memory_order_release);
        }

        ScopedIncrementer Auto() { return ScopedIncrementer(*this); }

        SAVANNA_NO_DISCARD int GetValue() const
        {
            return m_Counter.load(std::memory_order_acquire);
        }

        SAVANNA_NO_DISCARD operator int() const
        {
            return GetValue();
        }

        SAVANNA_NO_DISCARD auto operator<=>(const AtomicCounter& other) const
        {
            return GetValue() <=> other.GetValue();
        }

        void AwaitZero()
        {
            while (m_Counter.load(std::memory_order_acquire) != 0)
            {
                // spin
            }
        }
    };
} // namespace Savanna
