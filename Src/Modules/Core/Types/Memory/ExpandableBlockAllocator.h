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

#include "SavannaEngine.h"
#include "Utilities/SavannaCoding.h"

#include "Allocator.h"
#include "CacheLine.h"
#include "MemoryBlocks.h"
#include "MemoryBuffer.h"
#include "MemoryChunkDescriptors.h"

#include "Types/Containers/Arrays/ArraySlice.h"
#include "Types/Containers/Arrays/DynamicArray.h"

namespace Savanna
{
    class ExpandableBlockAllocator : public Allocator, NonCopyable
    {
    private:

        size_t m_BufferBlockSize = 0;

        DynamicArray<MemoryBuffer> m_MemoryPoolContainer;

        MemoryChunkDescriptor* m_Head = nullptr;
        MemoryChunkDescriptor* m_Tail = nullptr;

        size_t m_AllocatedBytes = 0;
        size_t m_Size = 0;

    public:
        ExpandableBlockAllocator() = default;
        ExpandableBlockAllocator(size_t initialBufferCount, size_t bufferBlockSize, bool contiguous = true);
        ~ExpandableBlockAllocator();

        ExpandableBlockAllocator(ExpandableBlockAllocator&& other) { *this = std::move(other); }

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
} // namespace Savanna
