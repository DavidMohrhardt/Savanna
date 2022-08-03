#include "FreeListAllocator.h"

#include <cassert>

#include "Profiling/Profiler.h"

namespace Savanna
{
    /**
     * @brief Fast path for accessing the maximum unfragmented size of the list.
     *
     * @return size_t
     */
    SAVANNA_NO_DISCARD size_t FreeListAllocator::max_size() SAVANNA_NO_EXCEPT
    {
        if (m_MaxContiguousBlock != nullptr)
        {
            return m_MaxContiguousBlock->m_Size;
        }

        return 0;
    }

    SAVANNA_NO_DISCARD inline void* FreeListAllocator::allocate_internal(size_t size, const size_t& alignment)
    {
        SAVANNA_INSERT_SCOPED_PROFILER("FreeListAllocator::allocate_internal");
        assert(size > 0 && alignment > 0);

        if (m_Head == nullptr)
        {
            throw BadAllocationException();
        }

        void* pBlock = nullptr;
        if (alignment != L1CacheLineLength())
        {
            size_t forwardAdjust = 0;
            pBlock = FindNextFreeBlockOfSize(size, alignment, forwardAdjust);
        }
        else
        {
            // Use a slightly faster path as the FreeBlockDesc addresses should be aligned to the L1 cache
            // by default.
            pBlock = FindNextFreeBlockOfSizeCacheAligned(size);
        }

        if (pBlock == nullptr)
        {
            throw BadAllocationException();
        }

        return pBlock;
    }

    void deallocate_internal(void* ptr, const size_t alignment)
    {
        // 1. recalculate the previous forward alignment
    }

    SAVANNA_NO_DISCARD inline void* FreeListAllocator::FindNextFreeBlockOfSize(const size_t size, const size_t alignment, size_t& outForwardAdjust)
    {
        SAVANNA_INSERT_SCOPED_PROFILER("FreeListAllocator::FindNextFreeBlockOfSize");
        FreeBlockDesc* pPreviousBlock = nullptr;
        FreeBlockDesc* pCurrentBlock = m_Head;
        while (pCurrentBlock != nullptr)
        {
            size_t totalSize = GetAlignedSizeRequirement(pCurrentBlock, size, alignment);
            if (pCurrentBlock->m_Size > totalSize)
            {
                pPreviousBlock->m_Next = reinterpret_cast<FreeBlockDesc*>(PointerAdd(pCurrentBlock, totalSize));
                pPreviousBlock->m_Next->m_Next = pCurrentBlock->m_Next;
                pPreviousBlock->m_Next->m_Size = pCurrentBlock->m_Size - totalSize;

                return PointerAdd(pCurrentBlock, outForwardAdjust);
            }

            pPreviousBlock = pCurrentBlock;
            pCurrentBlock = pPreviousBlock->m_Next;
        }

        return nullptr;
    }

    SAVANNA_NO_DISCARD inline void* FreeListAllocator::FindNextFreeBlockOfSizeCacheAligned(const size_t size)
    {
        SAVANNA_INSERT_SCOPED_PROFILER("FreeListAllocator::FindNextFreeBlockOfSizeCacheAligned");
        FreeBlockDesc* pPreviousBlock = nullptr;
        FreeBlockDesc* pCurrentBlock = m_Head;
        while (pCurrentBlock != nullptr)
        {
            size_t totalSize = size + GetForwardAlignment(PointerAdd(pCurrentBlock, size), L1CacheLineLength());
            if (pCurrentBlock->m_Size > totalSize)
            {
                pPreviousBlock->m_Next = reinterpret_cast<FreeBlockDesc*>(PointerAdd(pCurrentBlock, totalSize));
                pPreviousBlock->m_Next->m_Next = pCurrentBlock->m_Next;
                pPreviousBlock->m_Next->m_Size = pCurrentBlock->m_Size - totalSize;

                return static_cast<void*>(pCurrentBlock);
            }

            pPreviousBlock = pCurrentBlock;
            pCurrentBlock = pPreviousBlock->m_Next;
        }

        return nullptr;
    }

    SAVANNA_NO_DISCARD inline size_t FreeListAllocator::GetAlignedSizeRequirement(const void* const ptr, const size_t alignment, const size_t size) const
    {
        SAVANNA_INSERT_SCOPED_PROFILER("FreeListAllocator::GetAlignedSizeRequirement");
        size_t forwardAdjust = GetForwardAlignment(ptr, alignment);
        size_t blockHeaderOffset = GetForwardAlignment(PointerAdd(ptr, forwardAdjust + size), alignof(FreeBlockDesc));
        return size + forwardAdjust + blockHeaderOffset;
    }
} // namespace Savanna
