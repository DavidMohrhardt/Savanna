#pragma once

// Idea gathered from Game Engine Architecture 3E by Jason Gregory pg. 326

#include <cassert>

// #include <Utilities/PreprocessorDefinitions/InterfaceDefinitions.h>
#include <Utilities/SavannaCoding.h>

namespace Savanna
{

    class VolatileLock
    {
    private:
        // Prevent optimization to ensure assertions catch overlapping accesses to critical section by threads.
        volatile bool m_Locked;

    public:
        /**
         * @brief Acquire the volatile lock if released.
         */
        void Acquire()
        {
            assert(!m_Locked);
            m_Locked = true;
        }

        /**
         * @brief Release the volatile lock if acquired.
         */
        void Release()
        {
            assert(m_Locked);
            m_Locked = false;
        }
    };

    class VolatileLockGuard
    {
    private:
        VolatileLock& m_Lock;
    public:
        VolatileLockGuard(VolatileLock& lock) : m_Lock(lock)
        {
            lock.Acquire();
        }

        ~VolatileLockGuard()
        {
            m_Lock.Release();
        }
    };

    // SAVANNA_EXPORT(void*) Savanna_VolatileLock_CreateVolatileLock();
    // SAVANNA_EXPORT(void) Savanna_VolatileLock_Acquire(void* lockHandle);
    // SAVANNA_EXPORT(void) Savanna_VolatileLock_Release(void* lockHandle);
    // SAVANNA_EXPORT(void) Savanna_VolatileLock_DestroyVolatileLock(void*);
} // namespace Savanna
