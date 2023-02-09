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

#include <vector>

namespace Savanna
{
    class ExpandableBlockAllocator : public Allocator, NonCopyable
    {
    private:
        size_t m_BufferBlockSize = 0;

        std::vector<MemoryBuffer> m_MemoryPoolContainer;
        MemoryChunkDescriptor* m_Head = nullptr;
        MemoryChunkDescriptor* m_Tail = nullptr;

    public:
        ExpandableBlockAllocator() = default;
        ExpandableBlockAllocator(size_t initialBufferCount, size_t bufferBlockSize);
        ~ExpandableBlockAllocator();

    public:
        SAVANNA_NO_DISCARD void* alloc(const size_t& size, const size_t& alignment) SAVANNA_OVERRIDE;
        void free(void* const ptr, const size_t& alignment) SAVANNA_OVERRIDE;

        SAVANNA_NO_DISCARD ArraySlice GetReadonlyMemoryView(const size_t& start, const size_t& length) const;

    private:
        MemoryChunkDescriptor* AllocateAdditionalBuffers(size_t minimumSize);
    };
} // namespace Savanna
