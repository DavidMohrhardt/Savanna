/**
 * @file FreeListAllocator.h
 * @author David Mohrhardt (david.mohrhardt@gmail.com)
 * @brief TODO @DavidMohrhardt Document
 * @version 0.1
 * @date 2022-08-02
 *
 * @copyright Copyright (c) 2022
 *
 */

#pragma once

#include "AllocatorUtils.h"

#include "Memory/CacheLine.h"
#include "Memory/MemoryArena.h"

#include "Utilities/SavannaCoding.h"

namespace Savanna
{
    struct alignas(L1CacheLineLength()) FreeBlockDesc
    {
        FreeBlockDesc* m_Next;
        size_t m_Size;

        bool IsFreeBlock() const { return this == (m_Next - 1); }
    };

    class FreeListAllocator
    {
    private:
        void* m_Root;
        FreeBlockDesc* m_Head;
        FreeBlockDesc* m_MaxContiguousBlock;
        size_t m_Size;
        size_t m_AllocatedBytes;

    public:
        FreeListAllocator(MemoryArena& arena, size_t size);
        ~FreeListAllocator();

        SAVANNA_NO_DISCARD size_t max_size() SAVANNA_NO_EXCEPT;

    protected:
        SAVANNA_NO_DISCARD void* allocate_internal(size_t size, const size_t& alignment);
        void deallocate_internal(void* ptr, const size_t sizeOfType, const size_t alignment);

    private:
        SAVANNA_NO_DISCARD void* FindNextFreeBlockOfSize(const size_t size, const size_t alignment, size_t& outForwardAdjust);
        SAVANNA_NO_DISCARD void* FindNextFreeBlockOfSizeCacheAligned(const size_t size);

        SAVANNA_NO_DISCARD size_t GetAlignedSizeRequirement(const void* const ptr, const size_t alignment, const size_t size) const;
    };
} // namespace Savanna
