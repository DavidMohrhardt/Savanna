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
        CoreAllocatorWrapper(void* root, size_t size, AllocatorType allocatorType);

        CoreAllocatorWrapper(CoreAllocatorWrapper& other) = delete;
        CoreAllocatorWrapper(CoreAllocatorWrapper&& other);

        // Move Assignment Operator
        CoreAllocatorWrapper(FreeListAllocator&& freeListAllocator)
            : m_CoreAllocatorWrapperType(k_FreeList)
            , m_FreeListAllocator(std::move(freeListAllocator))
        {
        }

        ~CoreAllocatorWrapper() {}

    public:
        void* Allocate(size_t& size, size_t& alignment);
        void Free(void* ptr, const size_t& alignment);

        size_t GetSize() const;
        size_t GetAllocatedSize() const;

        void Reset();
    };
} // namespace Savanna
