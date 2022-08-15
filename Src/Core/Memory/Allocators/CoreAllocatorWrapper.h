/**
 * @file SavannaAllocator.h
 * @author David Mohrhardt (https://github.com/DavidMohrhardt/Savanna)
 * @brief
 * @version 0.1
 * @date 2022-08-14
 *
 * @copyright Copyright (c) 2022
 *
 */

#pragma once

#include "Utilities/SavannaCoding.h"

#include "AllocatorKind.h"
#include "FreeListAllocator.h"
// #include "LinearAllocator.h"
// #include "StackAllocator.h"

namespace Savanna
{
    /**
     * @brief A wrapper for all core allocators. This is an easy abstraction for using the core allocators without
     * virtual methods or inheritance.
     *
     * Space inefficiency is not the concern for this class.
     *
     */
    class CoreAllocatorWrapper
    {
    private:
        AllocatorKind m_AllocatorKind;
        union
        {
            FreeListAllocator m_FreeListAllocator;
            // LinearAllocator m_LinearAllocator;
        };

    public:
        CoreAllocatorWrapper(MemoryArena* arena, size_t size, AllocatorType allocatorKind)
            : m_AllocatorKind(allocatorKind)
        {
            switch (allocatorKind)
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
            m_AllocatorKind = other.m_AllocatorKind;
            switch (m_AllocatorKind)
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
            m_AllocatorKind = other.m_AllocatorKind;
            other.m_AllocatorKind = k_None;
            switch (m_AllocatorKind)
            {
            case k_FreeList:
                m_FreeListAllocator = other.m_FreeListAllocator;
                other.m_FreeListAllocator = FreeListAllocator();
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

        CoreAllocatorWrapper(FreeListAllocator& FreeListAllocator)
        {
            m_AllocatorKind = k_FreeList;
            m_FreeListAllocator = FreeListAllocator;
        }

        CoreAllocatorWrapper(FreeListAllocator&& FreeListAllocator)
        {
            m_AllocatorKind = k_FreeList;
            m_FreeListAllocator = FreeListAllocator;
            FreeListAllocator = FreeListAllocator();
        }

        ~CoreAllocatorWrapper() {}

    public:
        void* allocate(const size_t& size, const size_t& alignment);
        void free(void* ptr, const size_t& alignment);
    };
} // namespace Savanna
