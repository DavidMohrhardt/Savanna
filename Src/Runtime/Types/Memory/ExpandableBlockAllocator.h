/**
 * @file ExpandableBlockAllocator.h
 * @author David Mohrhardt (https://github.com/DavidMohrhardt/Savanna)
 * @brief
 * @version 0.1
 * @date 2023-02-03
 *
 * @copyright Copyright (c) 2023
 *
 */
#pragma once

#include "Utilities/SavannaCoding.h"

#include "Allocator.h"
#include "AtomicAllocatorWrapper.h"
#include "MemoryChunkDescriptors.h"
#include "MemoryBuffer.h"

#include "Memory/MemoryLabel.h"

#include "Types/Containers/Arrays/DynamicArray.h"

namespace Savanna
{
    class ExpandableBlockAllocator : public Allocator
    {
    private:
        MemoryLabel m_MemoryLabel;

        MemoryChunkDescriptor* m_Head = nullptr;
        MemoryChunkDescriptor* m_Tail = nullptr;
        size_t m_BufferBlockSize = 0;
        size_t m_AllocatedBytes = 0;
        size_t m_Size = 0;

        DynamicArray<MemoryBuffer> m_MemoryPoolContainer;


#if SAVANNA_ENABLE_RIGOROUS_MEMORY_VALIDATION || true
        size_t m_AllocationCount = 0;
        size_t m_FreeCount = 0;
#endif

    public:
        ExpandableBlockAllocator(bool contiguous = true, const MemoryLabel label = k_SavannaMemoryLabelHeap);
        ExpandableBlockAllocator(size_t initialBufferCount, size_t bufferBlockSize, bool contiguous = true, const MemoryLabel label = k_SavannaMemoryLabelHeap);
        ~ExpandableBlockAllocator();

        ExpandableBlockAllocator(ExpandableBlockAllocator&& other) noexcept { *this = std::move(other); }

        ExpandableBlockAllocator& operator=(ExpandableBlockAllocator&& other);

    public:
        SAVANNA_NO_DISCARD void* alloc(const size_t& size, const size_t& alignment) SAVANNA_OVERRIDE;
        void free(void* const ptr, const size_t& alignment) SAVANNA_OVERRIDE;

        SAVANNA_NO_DISCARD size_t GetAllocatedBytes() const { return m_AllocatedBytes; }
        SAVANNA_NO_DISCARD size_t GetSize() const { return m_Size; }

    private:
        MemoryChunkDescriptor* AllocateAdditionalBuffers(size_t minimumSize);
        void CreateBufferWithMemoryChunkDescs(
            const size_t bufferSize,
            MemoryChunkDescriptor** ppOutHead,
            MemoryChunkDescriptor** ppOutTail);
    };

    using AtomicExpandableBlockAllocator = AtomicAllocatorWrapper<ExpandableBlockAllocator>;
} // namespace Savanna
