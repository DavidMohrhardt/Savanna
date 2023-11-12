/**
 * @file AtomicAllocatorWrapper.h
 * @author David Mohrhardt (https://github.com/DavidMohrhardt/Savanna)
 * @brief TODO @David.Mohrhardt Document
 * @version 0.1
 * @date 2023-10-21
 *
 * @copyright Copyright (c) 2023
 *
 */
#pragma once

#include <Utilities/SavannaCoding.h>

#include <atomic>

#include "Types/Locks/SpinLock.h"
#include "Types/Memory/IAllocator.h"
#include "Types/Memory/Allocator.h"

namespace Savanna
{
    /**
     * @brief Provides a naive atomic wrapper for an allocator.
     * Is not the most efficient way to do this, but it works.
     * TODO @DavidMohrhardt: Implement a more efficient way to do this.
     *
     * @tparam TAlloc
     */
    template <class TAlloc>
    requires std::is_base_of_v<IAllocator, TAlloc>
    class AtomicAllocatorWrapper final : public Allocator
    {
        TAlloc m_Allocator;
        mutable SpinLock m_Lock;

    public:
        AtomicAllocatorWrapper() = default;

        template <typename... TArgs>
        AtomicAllocatorWrapper(TArgs&&... args) : m_Allocator(std::forward<TArgs>(args)...), m_Lock() {}

        AtomicAllocatorWrapper(TAlloc&& allocator) : m_Allocator(std::move(allocator)), m_Lock() {}
        AtomicAllocatorWrapper(AtomicAllocatorWrapper&& other) noexcept { *this = std::move(other); }

        AtomicAllocatorWrapper& operator=(AtomicAllocatorWrapper&& other) noexcept
        {
            // Acquire both locks in the same order to avoid deadlock
            if (this != &other)
            {
                auto sentinel = other.m_Lock.Auto();
                auto mySentinel = m_Lock.Auto();
                SAVANNA_MOVE_MEMBER(m_Allocator, other);
            }
            return *this;
        }

        virtual ~AtomicAllocatorWrapper() {}

        SAVANNA_NO_DISCARD void* alloc(const size_t& size, const size_t& alignment) SAVANNA_OVERRIDE
        {
            auto sentinel = m_Lock.Auto();
            return m_Allocator.alloc(size, alignment);
        }

        void free(void* const ptr, const size_t& alignment) SAVANNA_OVERRIDE
        {
            auto sentinel = m_Lock.Auto();
            m_Allocator.free(ptr, alignment);
        }

        SAVANNA_NO_DISCARD void* realloc(void* const ptr, const size_t& newSize, const size_t& alignment) SAVANNA_OVERRIDE
        {
            auto sentinel = m_Lock.Auto();
            return m_Allocator.realloc(ptr, newSize, alignment);
        }

        class AutoAllocatorGuard
        {
        private:
            friend class AtomicAllocatorWrapper<TAlloc>;
            TAlloc& m_AllocatorRef;
            SpinLock& m_SpinLockRef;

            AutoAllocatorGuard() = delete;
            AutoAllocatorGuard(TAlloc& allocRef, SpinLock& lockRef)
                : m_AllocatorRef{allocRef}
                , m_SpinLockRef{lockRef}
            {
                m_SpinLockRef.Lock();
            }

        public:
            ~AutoAllocatorGuard()
            {
                m_SpinLockRef.Unlock();
            }

            TAlloc& Get() { return m_AllocatorRef; }

            operator TAlloc&() { return m_AllocatorRef; }
        };

        SAVANNA_NO_DISCARD AutoAllocatorGuard GetSafeAllocator()
        {
            return AutoAllocatorGuard(m_Allocator, m_Lock);
        }
    };
} // namespace Savanna
