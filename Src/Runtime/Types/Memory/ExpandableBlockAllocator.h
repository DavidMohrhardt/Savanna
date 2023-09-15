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

// #include "Types/Containers/Arrays/DynamicArray.h"
#include <vector>

namespace Savanna
{
    class ExpandableBlockAllocator : public Allocator
    {
    private:
        size_t m_BufferBlockSize = 0;

        MemoryChunkDescriptor* m_Head = nullptr;
        MemoryChunkDescriptor* m_Tail = nullptr;

        // DynamicArray<MemoryBuffer> m_MemoryPoolContainer;
        std::vector<MemoryBuffer> m_MemoryPoolContainer;

        size_t m_AllocatedBytes = 0;
        size_t m_Size = 0;

    public:
        ExpandableBlockAllocator();
        ExpandableBlockAllocator(size_t initialBufferCount, size_t bufferBlockSize, bool contiguous = true);
        ~ExpandableBlockAllocator();

        ExpandableBlockAllocator(ExpandableBlockAllocator&& other) noexcept { *this = std::move(other); }

        ExpandableBlockAllocator& operator=(ExpandableBlockAllocator&& other);

    public:
        SAVANNA_NO_DISCARD void* alloc(const size_t& size, const size_t& alignment) SAVANNA_OVERRIDE;
        void free(void* const ptr, const size_t& alignment) SAVANNA_OVERRIDE;

        SAVANNA_NO_DISCARD size_t GetAllocatedBytes() const SAVANNA_OVERRIDE { return m_AllocatedBytes; }
        SAVANNA_NO_DISCARD size_t GetSize() const SAVANNA_OVERRIDE { return m_Size; }

    private:
        MemoryChunkDescriptor* AllocateAdditionalBuffers(size_t minimumSize);

        void CreateBufferWithMemoryChunkDescs(
            const size_t bufferSize,
            MemoryBuffer& outBuffer,
            MemoryChunkDescriptor** ppOutHead,
            MemoryChunkDescriptor** ppOutTail);
    };

    using AtomicExpandableBlockAllocator = AtomicAllocatorWrapper<ExpandableBlockAllocator>;
} // namespace Savanna