#pragma once

#include <Utilities/SavannaCoding.h>

#include <atomic>

#include "Types/Locks/SpinLock.h"
#include "Types/Memory/IAllocator.h"

namespace Savanna
{
    template <class TAlloc>
    requires std::is_base_of_v<IAllocator, TAlloc>
    class AtomicAllocatorWrapper final : public IAllocator
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
    };
} // namespace Savanna
