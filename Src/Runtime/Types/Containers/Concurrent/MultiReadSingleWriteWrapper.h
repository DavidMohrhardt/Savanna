/**
 * @file MultiReadSingleWriteWrapper.h
 * @author David Mohrhardt (https://github.com/DavidMohrhardt/Savanna)
 * @brief
 * @version 0.1
 * @date 2023-10-24
 *
 * @copyright Copyright (c) 2023
 *
 */
#pragma once

#include <atomic>

namespace Savanna
{
    template<typename TContainer, typename UStorage>
    class MultiReadSingleWriteWrapper
    {
    private:
        TContainer m_Container;
        std::atomic_int16_t m_Readers = 0;
        std::atomic_bool m_Writer = false;

        void ReadLock()
        {
            while (m_Writer.load(std::memory_order_acquire))
            {
                // Wait for writer to finish.
            }
            m_Readers.fetch_add(1, std::memory_order_acquire);
        }

        void ReadUnlock()
        {
            m_Readers.fetch_sub(1, std::memory_order_release);
        }

        void WriteLock()
        {
            // Set writer to true and wait for all readers to finish.
            while (m_Writer.exchange(true, std::memory_order_acquire))
            {
                // Wait for writer to finish.
            }

            while (m_Readers.load(std::memory_order_acquire) > 0)
            {
                // Wait for readers to finish.
            }
        }

        void WriteUnlock()
        {
            m_Writer.store(false, std::memory_order_release);
        }

    public:
        MultiReadSingleWriteWrapper() = default;
        ~MultiReadSingleWriteWrapper() = default;

        MultiReadSingleWriteWrapper(const MultiReadSingleWriteWrapper&) = delete;
        MultiReadSingleWriteWrapper(MultiReadSingleWriteWrapper&&) = delete;

        MultiReadSingleWriteWrapper& operator=(const MultiReadSingleWriteWrapper&) = delete;

        template <typename... TArgs>
        void Read(UStorage& outStorage, TArgs&&... args)
        {
            ReadLock();
            outStorage = m_Container.Read(std::forward<TArgs>(args)...);
            ReadUnlock();
        }

        template <typename... TArgs>
        void Write(const UStorage& inStorage, TArgs&&... args)
        {
            WriteLock();
            m_Container.Write(inStorage, std::forward<TArgs>(args)...);
            WriteUnlock();
        }
    };
} // namespace Savanna
