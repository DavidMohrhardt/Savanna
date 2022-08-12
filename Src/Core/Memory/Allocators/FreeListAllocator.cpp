/**
 * @file FreeListAllocator.cpp
 * @author David Mohrhardt (david.mohrhardt@gmail.com)
 * @brief TODO @DavidMohrhardt Document
 * @version 0.1
 * @date 2022-08-02
 *
 * @copyright Copyright (c) 2022
 *
 */
#include "FreeListAllocator.h"

#include <cassert>

#include "Memory/MemoryArena.h"
#include "Math/MathHelpers.h"
#include "Profiling/Profiler.h"
#include "Types/Pointers/UniversalPointer.h"
#include "Types/Pointers/PointerUtilities.h"

namespace Savanna
{
    FreeListAllocator::FreeListAllocator()
        : m_OwnerMemoryArena(nullptr)
        , m_Root(nullptr)
        , m_Head(nullptr)
        , m_Size(0)
        , m_AllocatedBytes(0)
        , m_NumberOfBlockLinks(0)
    {}

    FreeListAllocator::FreeListAllocator(MemoryArena* arena, size_t size)
    {
        m_OwnerMemoryArena = arena;

        m_Root = arena->AcquireMemory(size);
        m_Head = reinterpret_cast<FreeBlockDesc*>(m_Root);
        m_Head->m_Next = nullptr;
        m_Head->m_Size = m_Size - sizeof(FreeBlockDesc);
        m_AllocatedBytes = 0;
    }

    FreeListAllocator::FreeListAllocator(void* bufferPtr, size_t size)
        : m_OwnerMemoryArena(nullptr)
        , m_Root(bufferPtr)
        , m_Head(reinterpret_cast<FreeBlockDesc*>(bufferPtr))
        , m_Size(size)
        , m_AllocatedBytes(sizeof(FreeBlockDesc))
        , m_NumberOfBlockLinks(1)
    {
        if (m_Root == nullptr)
        {
            throw RuntimeErrorException("Attempted to initialize Allocator with no backing buffer.");
        }
    }

    FreeListAllocator::~FreeListAllocator()
    {
        if (m_OwnerMemoryArena != nullptr)
        {
            m_OwnerMemoryArena->ReleaseMemory(m_Root);
        }
    }

    /**
     * @brief Fast path for accessing the maximum unfragmented size of the list.
     *
     * @return size_t
     */
    size_t FreeListAllocator::MaxSize() SAVANNA_NO_EXCEPT
    {
        if (m_MaxContiguousBlock != nullptr)
        {
            return m_MaxContiguousBlock->m_Size;
        }
        return 0;
    }

    inline void* FreeListAllocator::Allocate(size_t size, const size_t& alignment)
    {
        SAVANNA_INSERT_SCOPED_PROFILER("FreeListAllocator::Allocate");
        assert(size > 0 && alignment > 0);

        if (m_Head == nullptr)
        {
            throw BadAllocationException();
        }

        void* freeBlockPtr = FindNextFreeBlockOfSize(size, alignment);
        if (freeBlockPtr == nullptr)
        {
            throw BadAllocationException();
        }

        return freeBlockPtr;
    }

    void FreeListAllocator::Deallocate(void* const ptr, const size_t alignment)
    {
        if (ptr == nullptr || ptr <= m_Root || ptr > (Add(m_Root, m_Size)))
        {
            return;
        }

        FreeBlockDesc* blockHeaderPtr = nullptr;
        if (alignment == alignof(FreeBlockDesc))
        {
            blockHeaderPtr = reinterpret_cast<FreeBlockDesc*>(Sub(ptr, sizeof(FreeBlockDesc)));
        }
        else
        {
            void* pointerTwoBlocksBack = Sub(ptr, sizeof(FreeBlockDesc) * 2);
            size_t forwardAlignment = GetForwardAlignment(pointerTwoBlocksBack, alignof(FreeBlockDesc));
            blockHeaderPtr = reinterpret_cast<FreeBlockDesc*>(Add(ptr, forwardAlignment));
        }

        FreeBlockDesc* currentBlockPtr = m_Head;
        while (currentBlockPtr->m_Next < blockHeaderPtr)
        {
            currentBlockPtr = currentBlockPtr->m_Next;
        }

        blockHeaderPtr->m_Next = currentBlockPtr->m_Next;
        currentBlockPtr->m_Next = blockHeaderPtr;
    }

    inline size_t FreeListAllocator::GetRequiredSizeWithHeader(void* const ptr, const size_t size, const size_t alignment) const
    {
        // return GetForwardAlignment(ptr, alignment)
        //     + GetLengthInBytesToNextCacheLine(size)
        //     + sizeof(FreeBlockDesc);

        auto alignedPtr = Add(ptr, GetForwardAlignment(ptr, alignment));
        void* nextBlockAlignedPtr =
            GetForwardAlignedPtr<void, void>(Add(alignedPtr, size), alignof(FreeBlockDesc));
        return reinterpret_cast<size_t>(Sub(ptr, reinterpret_cast<uintptr>(nextBlockAlignedPtr)));
    }

    inline void* FreeListAllocator::FindNextFreeBlockOfSize(const size_t size, const size_t alignment)
    {
        SAVANNA_INSERT_SCOPED_PROFILER("FreeListAllocator::FindNextFreeBlockOfSize");
        FreeBlockDesc* previousBlockPtr = m_Head;
        FreeBlockDesc* currentBlockPtr = m_Head;

        while (currentBlockPtr != nullptr)
        {
            if (currentBlockPtr->m_Size >= size)
            {
                // We have found a suitable block
                size_t requiredSizeWithHeader = alignment != L1CacheLineLength()
                    ? GetRequiredSizeWithHeader(currentBlockPtr + 1, size, alignment)
                    // Can skip forward alignment here
                    : GetLengthInBytesToNextCacheLine(size) + sizeof(FreeBlockDesc) + 1;

                // TODO @DavidMohrhardt May want to add additional profiling tools to ensure a given branch is actually the most likely
                if (currentBlockPtr->m_Size > requiredSizeWithHeader) SAVANNA_BRANCH_LIKELY
                {
                    // More likely that allocations won't perfectly fit but have some breathing room
                    // until higher memory fragmentation in the allocator
                    previousBlockPtr->m_Next = static_cast<FreeBlockDesc*>(Add(currentBlockPtr, requiredSizeWithHeader));
                    currentBlockPtr->m_Size = requiredSizeWithHeader - sizeof(FreeBlockDesc);
                    currentBlockPtr->m_Next = nullptr;

                    // Number of available block links has not been updated so no need to update

                    return reinterpret_cast<void*>(currentBlockPtr + 1);
                }
                else SAVANNA_BRANCH_UNLIKELY
                {
                    // Unlikely unless the allocator becomes highly fragmented and the number of bytes left over
                    // is less than the size of a block + 1 byte or size is equivalent to n * L1CacheLineLength
                    // for some positive integer n

                    previousBlockPtr->m_Next = currentBlockPtr->m_Next;
                    currentBlockPtr->m_Size = size;

                    if (m_Head == currentBlockPtr)
                    {
                        m_Head = previousBlockPtr->m_Next;
                    }

                    // Lost a free block so subtract the number of block links
                    m_NumberOfBlockLinks--;
                    return reinterpret_cast<void*>(currentBlockPtr + 1);
                }
            }

            previousBlockPtr = currentBlockPtr;
            currentBlockPtr = currentBlockPtr->m_Next;
        }

        return nullptr;
    }

    inline size_t FreeListAllocator::GetAlignedSizeRequirement(const void* const ptr, const size_t alignment, const size_t size) const
    {
        SAVANNA_INSERT_SCOPED_PROFILER("FreeListAllocator::GetAlignedSizeRequirement");
        size_t forwardAdjust = GetForwardAlignment(ptr, alignment);
        size_t blockHeaderOffset = GetForwardAlignment(Add(ptr, forwardAdjust + size), alignof(FreeBlockDesc));
        return size + forwardAdjust + blockHeaderOffset;
    }
} // namespace Savanna
