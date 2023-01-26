#include "ThreadPool.h"

namespace Savanna::Threading
{
    std::thread::id ThreadPool::s_MainThreadId;
    bool ThreadPool::s_IsInitialized;

    ThreadPool::ThreadPool(const ThreadPoolDescriptor descriptor)
        : m_ThreadPoolDescriptor(descriptor)
        , m_Threads(descriptor.m_ThreadCount)
    {
        static auto _ = []() -> bool
        {
            s_MainThreadId = std::this_thread::get_id();
            s_IsInitialized = true;
            return true;
        }();
    }

    void ThreadPool::Stop()
    {
        for (std::thread& thread : m_Threads)
        {
            thread.join();
        }
    }

    void ThreadPool::Join()
    {
        for (std::thread& thread : m_Threads)
        {
            thread.join();
        }
    }
}
