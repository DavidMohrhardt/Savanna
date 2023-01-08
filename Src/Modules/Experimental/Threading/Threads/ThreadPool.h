#pragma once

#include <thread>
#include <vector>


#include "Types/Primitive/PrimitiveTypes.h"
// #include "Containers/Stl/SavannaVector.h"

namespace Savanna
{
    class ThreadManager;

    struct ThreadPoolDescriptor
    {
        ThreadPoolDescriptor(uint16 threadCount) : m_ThreadCount(threadCount) {}

        uint16 m_ThreadCount;
    };

    class ThreadPool : public SavannaClass<ThreadPool>
    {
    private:
        friend class ThreadManager;

    public:
        const uint32 k_ProcessorCount = std::thread::hardware_concurrency();
        const uint32 k_AvailableThreads = k_ProcessorCount - 1; // Main thread is reserved

    private:
        std::vector<std::thread::id> m_ThreadIds;
        std::vector<std::thread> m_Threads;

        const ThreadPoolDescriptor m_ThreadPoolDescriptor;

    public:
        const ThreadPoolDescriptor GetThreadPoolDescriptor() const
        {
            return m_ThreadPoolDescriptor;
        }
    private:

    private:
        static std::thread::id s_MainThreadId;
        static bool s_IsInitialized;

    private:
        ThreadPool(const ThreadPoolDescriptor descriptor);
        ~ThreadPool();

        static void InitializeDefaultValues(std::thread::id mainThreadId)
        {
            if (!s_IsInitialized)
            {
                s_MainThreadId = mainThreadId;
                s_IsInitialized = true;;
            }
        }
    };
}
