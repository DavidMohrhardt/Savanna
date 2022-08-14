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

#include "Memory/MemoryArena.h"
#include "Math/MathHelpers.h"
#include "Math/PointerMath.h"
#include "Profiling/Profiler.h"
#include "Utilities/Assertions/SavannaAssertions.h"

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

    void* FreeListAllocator::Allocate(size_t size, const size_t& alignment)
    {
        SAVANNA_INSERT_SCOPED_PROFILER("FreeListAllocator::Allocate");
        SAVANNA_MEMORY_SAFETY_ASSERT(size > 0 && alignment > 0, "Allocation size and alignment must be greater than 0");

        MemoryChunkHeader* previousChunkHeader = nullptr;
        MemoryChunkHeader* currentChunkHeader = m_Head;

        // Need to find the tightest fit for the allocated chunk.
        MemoryChunkHeader* previousBestFitChunkHeader = nullptr;
        MemoryChunkHeader* bestFitChunkHeader = nullptr;
        void* outMemoryChunkPtr = nullptr;

        // Set the best fit an unreachable size.
        int bestFitChunkSize = m_Size;
        int totalRequiredSize = 0;
        int forwardAlignment = 0;

        while (currentChunkHeader != nullptr)
        {
            forwardAlignment = GetForwardAlignment(&currentChunkHeader[1], alignment);
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

        AllocatedChunkDescriptor* descriptorPtr
            = reinterpret_cast<AllocatedChunkDescriptor*>(outMemoryChunkPtr) - 1;

        descriptorPtr->m_Size = totalRequiredSize + sizeof(MemoryChunkHeader);
        descriptorPtr->m_Offset = forwardAlignment;

        return outMemoryChunkPtr;
    }

    void FreeListAllocator::Deallocate(void* const ptr, const size_t alignment)
    {
        SAVANNA_INSERT_SCOPED_PROFILER("FreeListAllocator::Deallocate");
        SAVANNA_MEMORY_SAFETY_ASSERT(ptr != nullptr && alignment > 0, "Invalid arguments");
        SAVANNA_MEMORY_SAFETY_ASSERT(m_AllocatedBytes > sizeof(MemoryChunkHeader), "Attempting to deallocate pointer from an empty allocator");

        MemoryChunkHeader* allocatedChunkHeader = nullptr;
        {
            AllocatedChunkDescriptor* chunkDescriptor = reinterpret_cast<AllocatedChunkDescriptor*>(ptr) - 1;

            allocatedChunkHeader =
                reinterpret_cast<MemoryChunkHeader*>(Subtract(ptr, chunkDescriptor->m_Offset, sizeof(MemoryChunkHeader)));
            allocatedChunkHeader->m_Size = chunkDescriptor->m_Size;
        }

        MemoryChunkHeader* previousChunkHeader = nullptr;
        MemoryChunkHeader* currentChunkHeader = m_Head;
        while (currentChunkHeader != nullptr)
        {
            if (reinterpret_cast<uintptr>(currentChunkHeader) > reinterpret_cast<uintptr>(allocatedChunkHeader))
            {
                break;
            }

            previousChunkHeader = currentChunkHeader;
            currentChunkHeader = currentChunkHeader->m_Next;
        }

        if (previousChunkHeader == nullptr)
        {
            previousChunkHeader = allocatedChunkHeader;
            allocatedChunkHeader->m_Next = m_Head;
            m_Head = allocatedChunkHeader;
        }

        if (reinterpret_cast<uintptr>(previousChunkHeader) + previousChunkHeader->m_Size == reinterpret_cast<uintptr>(allocatedChunkHeader))
        {
            previousChunkHeader->m_Size += allocatedChunkHeader->m_Size;
        }
        else
        {
            previousChunkHeader->m_Next = allocatedChunkHeader;
            allocatedChunkHeader->m_Next = currentChunkHeader;
        }

        m_AllocatedBytes -= allocatedChunkHeader->m_Size;
    }
} // namespace Savanna
