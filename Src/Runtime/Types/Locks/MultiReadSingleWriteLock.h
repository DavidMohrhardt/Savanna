/**
 * @file MultiReadSingleWriteLock.h
 * @author David Mohrhardt (https://github.com/DavidMohrhardt/Savanna)
 * @brief
 * @version 0.1
 * @date 2023-10-31
 *
 * @copyright Copyright (c) 2023
 *
 */
#pragma once

#include "Utilities/SavannaCoding.h"

#include "AtomicCounter.h"
#include "SpinLock.h"

namespace Savanna
{
    class MultiReadSingleWriteLock
    {
    private:
        AtomicCounter m_ReaderCounter;
        SpinLock m_WriterLock;

    public:
        MultiReadSingleWriteLock() = default;
        MultiReadSingleWriteLock(const MultiReadSingleWriteLock&) = delete;
        MultiReadSingleWriteLock(MultiReadSingleWriteLock&&) = delete;

        MultiReadSingleWriteLock& operator=(const MultiReadSingleWriteLock&) = delete;
        MultiReadSingleWriteLock& operator=(MultiReadSingleWriteLock&&) = delete;

        struct ScopedReader
        {
            MultiReadSingleWriteLock& m_Lock;

            ScopedReader(MultiReadSingleWriteLock& lock) : m_Lock(lock) { m_Lock.ReadLock(); }
            ~ScopedReader() { m_Lock.ReadUnlock(); }
        };

        struct ScopedWriter
        {
            MultiReadSingleWriteLock& m_Lock;

            ScopedWriter(MultiReadSingleWriteLock& lock) : m_Lock(lock) { m_Lock.WriteLock(); }
            ~ScopedWriter() { m_Lock.WriteUnlock(); }
        };

        void ReadLock()
        {
            m_WriterLock.Lock();
            m_ReaderCounter.Increment();
            m_WriterLock.Unlock();
        }

        void ReadUnlock()
        {
            m_ReaderCounter.Decrement();
        }

        void WriteLock()
        {
            m_WriterLock.Lock();
            m_ReaderCounter.AwaitZero();
        }

        void WriteUnlock()
        {
            m_WriterLock.Unlock();
        }

        ScopedReader AutoRead() { return ScopedReader(*this); }
        ScopedWriter AutoWrite() { return ScopedWriter(*this); }
    };
} // namespace Savanna

#ifndef SAVANNA_READ_CRITICAL_SECTION
#define SAVANNA_READ_CRITICAL_SECTION(__lockName, __code) \
    {                                                        \
        __lockName.ReadLock();                               \
        __code;                                              \
        __lockName.ReadUnlock();                             \
    }
#endif // SAVANNA_READ_CRITICAL_SECTION

#ifndef SAVANNA_WRITE_CRITICAL_SECTION
#define SAVANNA_WRITE_CRITICAL_SECTION(__lockName, __code) \
    {                                                         \
        __lockName.WriteLock();                               \
        __code;                                               \
        __lockName.WriteUnlock();                             \
    }
#endif // SAVANNA_WRITE_CRITICAL_SECTION
