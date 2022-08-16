/**
 * @file SavannaAllocator.h
 * @author David Mohrhardt (https://github.com/DavidMohrhardt/Savanna)
 * @brief
 * @version 0.1
 * @date 2022-08-14
 *
 */

#pragma once

#include "Utilities/SavannaCoding.h"

#include "AllocatorType.h"
#include "FreeListAllocator.h"
// #include "LinearAllocator.h"
// #include "StackAllocator.h"

namespace Savanna
{
    /**
     * @brief A wrapper for all core allocators. This is an easy abstraction for using the core allocators without
     * virtual methods or inheritance.
     *
     * Instead, this class uses a union to store the core allocator. This is a completely awful hack, but it's an
     * easy way to avoid virtual methods. Needs testing.
     */
    class CoreAllocatorWrapper
    {
    private:
        AllocatorType m_CoreAllocatorWrapperType;
        union
        {
            FreeListAllocator m_FreeListAllocator;
            // LinearAllocator m_LinearAllocator;
        };

    public:
        CoreAllocatorWrapper(MemoryArena* arena, size_t size, AllocatorType allocatorType)
            : m_CoreAllocatorWrapperType(allocatorType)
        {
            switch (allocatorType)
            {
            case k_FreeList:
                m_FreeListAllocator = FreeListAllocator(arena, size);
                break;
            // case k_Linear:
            //     m_LinearAllocator = LinearAllocator(arena, size);
            //     break;
            // case k_Stack:
            //     m_StackAllocator = StackAllocator(arena, size);
            //     break;
            default:
                break;
            }
        }

        CoreAllocatorWrapper(CoreAllocatorWrapper& other)
        {
            m_CoreAllocatorWrapperType = other.m_CoreAllocatorWrapperType;
            switch (m_CoreAllocatorWrapperType)
            {
            case k_FreeList:
                m_FreeListAllocator = other.m_FreeListAllocator;
                break;
            // case k_Linear:
            //     m_LinearAllocator = other.m_LinearAllocator;
            //     break;
            // case k_Stack:
            //     m_StackAllocator = other.m_StackAllocator;
            //     break;
            default:
                break;
            }
        }

        // Move Constructor
        CoreAllocatorWrapper(CoreAllocatorWrapper&& other)
        {
            m_CoreAllocatorWrapperType = other.m_CoreAllocatorWrapperType;
            other.m_CoreAllocatorWrapperType = k_None;
            switch (m_CoreAllocatorWrapperType)
            {
            case k_FreeList:
                m_FreeListAllocator = other.m_FreeListAllocator;
                other.m_FreeListAllocator = FreeListAllocator();
                break;
            default:
                break;
            }
        }

        CoreAllocatorWrapper(FreeListAllocator& freeListAllocator)
        {
            m_CoreAllocatorWrapperType = k_FreeList;
            m_FreeListAllocator = freeListAllocator;
        }

        // Move Assignment Operator
        CoreAllocatorWrapper(FreeListAllocator&& freeListAllocator)
        {
            m_CoreAllocatorWrapperType = k_FreeList;
            m_FreeListAllocator = freeListAllocator;
            freeListAllocator = FreeListAllocator();
        }

        ~CoreAllocatorWrapper() {}

    public:
        void* Allocate(size_t& size, size_t& alignment);
        void Free(void* ptr, const size_t& alignment);

        size_t GetSize() const;
        size_t GetAllocatedSize() const;

        void Reset();

        bool operator==(const CoreAllocatorWrapper& other) const
        {
            if (m_CoreAllocatorWrapperType != other.m_CoreAllocatorWrapperType)
            {
                return false;
            }

            switch (m_CoreAllocatorWrapperType)
            {
            case k_FreeList:
                return m_FreeListAllocator == other.m_FreeListAllocator;
            // case k_Linear:
            //     return m_LinearAllocator == other.m_LinearAllocator;
            // case k_Stack:
            //     return m_StackAllocator == other.m_StackAllocator;
            default:
                return false;
            }
        }
    };
} // namespace Savanna
