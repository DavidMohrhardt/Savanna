#include "Threading/Threads/ThreadPool.h"

namespace Savanna
{
    std::thread::id ThreadPool::s_MainThreadId;
    bool ThreadPool::s_IsInitialized;

    ThreadPool::ThreadPool(const ThreadPoolDescriptor descriptor)
        : m_ThreadPoolDescriptor(descriptor)
    {
        if (!s_IsInitialized)
        {
            throw std::exception();
        }

        uint16 threadCount = descriptor.m_ThreadCount;

        for (int i = 0;

        i < descriptor.m_ThreadCount;

        ++i)
        {
            m_Threads.push_back(std::thread());
        }
    }

    ThreadPool::~ThreadPool()
    {
        for (std::thread& thread : m_Threads)
        {
            thread.join();
            thread.~thread();
        }

        m_Threads.clear();
    }
}