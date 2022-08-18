#pragma once

#include "MemoryBlocks.h"
#include "MemoryArena.h"

namespace Savanna
{
    class MemoryPool
    {
    private:
        MemoryArena m_Arena;
        UnifiedMemoryBlock64KiB* m_Root;
        int32 m_BlockCount;
        int32 m_NextAvailableBlock;
        FreeListAllocator m_PoolAllocator;

    public:
        MemoryPool();
        MemoryPool(size_t size);
        MemoryPool(MemoryPool& other) = delete;
        MemoryPool(MemoryPool&& other);

        ~MemoryPool();
        UnifiedMemoryBlock64KiB* AcquireBlock();
        void ReleaseBlock(UnifiedMemoryBlock64KiB* block);
    };

} // namespace Savanna
