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
#include "Math/PointerMath.h"
#include "Profiling/Profiler.h"

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
        : FreeListAllocator()
    {
        m_OwnerMemoryArena = arena;
        m_Root = arena->AcquireMemory(size);
        m_Head = reinterpret_cast<MemoryChunkHeader*>(m_Root);
        m_AllocatedBytes = 0;
        m_Size = size;
        m_AllocatedBytes = sizeof(MemoryChunkHeader);
        m_NumberOfBlockLinks = 1;

        m_Head->m_Next = nullptr;
        m_Head->m_Size = static_cast<int32>(size - sizeof(MemoryChunkHeader));
    }

    FreeListAllocator::FreeListAllocator(void* bufferPtr, size_t size)
        : m_OwnerMemoryArena(nullptr)
        , m_Root(bufferPtr)
        , m_Head(reinterpret_cast<MemoryChunkHeader*>(bufferPtr))
        , m_Size(size)
        , m_AllocatedBytes(sizeof(MemoryChunkHeader))
        , m_NumberOfBlockLinks(1)
    {
        m_Head->m_Next = nullptr;
        m_Head->m_Size = static_cast<int32>(size - sizeof(MemoryChunkHeader));
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
        return 0;
    }

    void* FreeListAllocator::Allocate(size_t size, const size_t& alignment)
    {
        SAVANNA_INSERT_SCOPED_PROFILER("FreeListAllocator::Allocate");
        assert(size > 0 && alignment > 0);

        MemoryChunkHeader* previousChunkHeader = nullptr;
        MemoryChunkHeader* currentChunkHeader = m_Head;

        // Need to find the tightest fit for the allocated chunk.
        MemoryChunkHeader* previousBestFitChunkHeader = nullptr;
        MemoryChunkHeader* bestFitChunkHeader = nullptr;
        void* outMemoryChunkPtr = nullptr;

        // Set the best fit an unreachable size.
        int bestFitChunkSize = m_Size;
        int totalRequiredSize = 0;

        while (currentChunkHeader != nullptr)
        {
            int forwardAlignment = GetForwardAlignment(&currentChunkHeader[1], alignment);
            totalRequiredSize = size + forwardAlignment;
            if (currentChunkHeader->m_Size >= totalRequiredSize
                && (bestFitChunkHeader == nullptr || currentChunkHeader->m_Size < bestFitChunkHeader->m_Size))
            {
                previousBestFitChunkHeader = previousChunkHeader;
                bestFitChunkHeader = currentChunkHeader;
                bestFitChunkSize = currentChunkHeader->m_Size;
                outMemoryChunkPtr = &currentChunkHeader[1] + forwardAlignment;

                if (currentChunkHeader->m_Size == totalRequiredSize)
                {
                    break;
                }
            }

            previousChunkHeader = currentChunkHeader;
            currentChunkHeader = currentChunkHeader->m_Next;
        }

        if (outMemoryChunkPtr == nullptr)
        {
            return nullptr;
        }

        if (bestFitChunkHeader->m_Size - totalRequiredSize <= sizeof(MemoryChunkHeader))
        {
            // Can't split the chunk.
            totalRequiredSize = bestFitChunkHeader->m_Size;
            if (previousBestFitChunkHeader != nullptr)
            {
                previousBestFitChunkHeader->m_Next = bestFitChunkHeader->m_Next;
            }
            else
            {
                m_Head = bestFitChunkHeader->m_Next;
            }

            m_AllocatedBytes += totalRequiredSize;
        }
        else
        {
            // Split the chunk.
            MemoryChunkHeader* newChunkHeader = Add(&bestFitChunkHeader[1], totalRequiredSize);

            newChunkHeader->m_Size = bestFitChunkHeader->m_Size - totalRequiredSize;
            bestFitChunkHeader->m_Size = totalRequiredSize;
            newChunkHeader->m_Next = bestFitChunkHeader->m_Next;
            bestFitChunkHeader->m_Next = newChunkHeader;

            if (previousBestFitChunkHeader != nullptr)
            {
                previousBestFitChunkHeader->m_Next = newChunkHeader;
            }
            else
            {
                m_Head = newChunkHeader;
            }

            m_AllocatedBytes += totalRequiredSize + sizeof(MemoryChunkHeader);
        }

#if defined(OLD_IMPLEMENTATION)
        // O(n) search for the tightest fit. Can't do better than this.
        while (currentChunkHeader != nullptr)
        {
            int forwardAlignment = GetForwardAlignment(Add(&currentChunkHeader[1], size), alignment);
            int minimalRequiredSize = size + forwardAlignment;
            if (currentChunkHeader->m_Size > minimalRequiredSize)
            {
                if (currentChunkHeader->m_Size < bestFitChunkSize)
                {
                    previousBestFitChunkHeader = previousChunkHeader;
                    bestFitChunkHeader = currentChunkHeader;
                    bestFitChunkSize = currentChunkHeader->m_Size;
                    outMemoryChunkPtr = Add(&currentChunkHeader[1], forwardAlignment);
                }
            }
            else if (currentChunkHeader->m_Size == minimalRequiredSize)
            {
                // Found a chunk that exactly large enough.
                bestFitChunkHeader = currentChunkHeader;
                outMemoryChunkPtr = Add(&currentChunkHeader[1], forwardAlignment);
                break;
            }

            // Chunk is too small, move to the next chunk.
            // Continue searching.
            previousChunkHeader = currentChunkHeader;
            currentChunkHeader = currentChunkHeader->m_Next;
        }

        // Unlikely unless the allocator is out of memory or heavily fragmented.
        if (bestFitChunkHeader == nullptr) SAVANNA_BRANCH_UNLIKELY
        {
            return nullptr;
        }

        int allocatedSize = 0;

        // Update linked list of free blocks.
        if(bestFitChunkSize <= sizeof(MemoryChunkHeader))
        {
            allocatedSize = bestFitChunkHeader->m_Size;

            // remove the free block from the list
            if(previousBestFitChunkHeader != nullptr)
            {
                previousBestFitChunkHeader->m_Next = bestFitChunkHeader->m_Next;
            }
            else
            {
                m_Head = bestFitChunkHeader->m_Next;
            }
        }
        else
        {
            // split the free block into two
            MemoryChunkHeader* newChunkHeader = GetForwardAlignedPtr<MemoryChunkHeader, void>(
                Add(outMemoryChunkPtr, size), alignof(MemoryChunkHeader));

            newChunkHeader->m_Size = bestFitChunkHeader->m_Size - bestFitChunkSize;
            newChunkHeader->m_Next = bestFitChunkHeader->m_Next;
            // remove the free block from the list
            if(previousBestFitChunkHeader != nullptr)
            {
                previousBestFitChunkHeader->m_Next = newChunkHeader;
            }
            else
            {
                m_Head = newChunkHeader;
            }
        }

        m_AllocatedBytes += allocatedSize;
#endif
        return outMemoryChunkPtr;
    }

    void FreeListAllocator::Deallocate(void* const ptr, const size_t alignment)
    {

    }
} // namespace Savanna
