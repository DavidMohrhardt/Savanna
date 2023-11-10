/**
 * @file MultiListAllocator.h
 * @author David Mohrhardt (https://github.com/DavidMohrhardt/Savanna)
 * @brief TODO @David.Mohrhardt Document
 * @version 0.1
 * @date 2023-10-10
 *
 * @copyright Copyright (c) 2023
 *
 */
#pragma once

#include "Utilities/SavannaCoding.h"

#include "Allocator.h"
#include "AtomicAllocatorWrapper.h"
#include "FreeListAllocator.h"

#include "Types/Containers/Arrays/dynamic_array.h"
#include "Types/Memory/MemoryBuffer.h"

namespace Savanna
{
    class MultiListAllocator final : public Allocator
    {
    private:
        friend class IAllocator;

        struct MemoryPool
        {
            MemoryBuffer m_MemoryBuffer;
            FreeListAllocator m_FreeListAllocator;

            MemoryPool()
                : m_MemoryBuffer()
                , m_FreeListAllocator(m_MemoryBuffer.GetBuffer(), m_MemoryBuffer.GetSize())
            {}

            MemoryPool(size_t size, AllocatorKind allocatorKind)
                : m_MemoryBuffer(size, allocatorKind)
                , m_FreeListAllocator(m_MemoryBuffer.GetBuffer(), size)
            {}

            MemoryPool(MemoryPool&& other) noexcept
                : m_MemoryBuffer(std::move(other.m_MemoryBuffer))
                , m_FreeListAllocator(std::move(other.m_FreeListAllocator))
            {}

            MemoryPool& operator=(MemoryPool&& other) noexcept
            {
                if (this != &other)
                {
                    m_MemoryBuffer = std::move(other.m_MemoryBuffer);
                    m_FreeListAllocator = std::move(other.m_FreeListAllocator);
                }
                return *this;
            }
        };

        AllocatorKind m_AllocatorKind;

        size_t m_BufferBlockSize = 0;
        size_t m_AllocatedBytes = 0;
        size_t m_Size = 0;

        dynamic_array<MemoryPool> m_Pools;

#if SAVANNA_ENABLE_RUNTIME_MEMORY_VALIDATION
        size_t m_Allocations = 0;
        size_t m_Frees = 0;
#endif

    public:
        MultiListAllocator(const AllocatorKind allocatorKind = k_SavannaAllocatorKindHeap);
        MultiListAllocator(size_t initialBufferCount, size_t bufferBlockSize, const AllocatorKind allocatorKind = k_SavannaAllocatorKindHeap);

        virtual ~MultiListAllocator() = default;

        MultiListAllocator(MultiListAllocator&& other) noexcept { *this = std::move(other); }
        MultiListAllocator& operator=(MultiListAllocator&& other);

        SAVANNA_NO_DISCARD void* alloc(const size_t& size, const size_t& alignment) SAVANNA_OVERRIDE;
        void free(void* const ptr, const size_t& alignment) SAVANNA_OVERRIDE;
        SAVANNA_NO_DISCARD void* realloc(void* const ptr, const size_t& newSize, const size_t& alignment) SAVANNA_OVERRIDE;

        SAVANNA_NO_DISCARD size_t GetAllocatedBytes() const { return m_AllocatedBytes; }
        SAVANNA_NO_DISCARD size_t GetSize() const { return m_Size; }

    private:
        void AllocateAdditionalMemoryBuffer(size_t size);
        FreeListAllocator& FindPointerInPools(void* const ptr);
        FreeListAllocator& FindAllocatorForSize(size_t size);
    };

    using AtomicMultiListAllocator = AtomicAllocatorWrapper<MultiListAllocator>;
} // namespace Savanna
