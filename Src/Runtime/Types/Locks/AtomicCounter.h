/**
 * @file AtomicCounter.h
 * @author David Mohrhardt (https://github.com/DavidMohrhardt/Savanna)
 * @brief TODO @David.Mohrhardt Document
 * @version 0.1
 * @date 2023-10-31
 *
 * @copyright Copyright (c) 2023
 *
 */
#pragma once

#include "Utilities/SavannaCoding.h"

#include <atomic>

namespace Savanna
{
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
