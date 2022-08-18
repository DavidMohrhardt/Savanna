/**
 * @file FreeListAllocator.cpp
 * @author David Mohrhardt (https://github.com/DavidMohrhardt/Savanna)
 * @brief TODO @DavidMohrhardt Document
 * @version 0.1
 * @date 2022-08-02
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
        : m_Root(nullptr)
        , m_Head(nullptr)
        , m_Size(0)
        , m_AllocatedBytes(0)
    {}

    FreeListAllocator::FreeListAllocator(void* bufferPtr, size_t size)
        : m_Root(bufferPtr)
        , m_Head(reinterpret_cast<MemoryChunkHeader*>(bufferPtr))
        , m_Size(size)
        , m_AllocatedBytes(sizeof(MemoryChunkHeader))
    {
        m_Head->m_Next = nullptr;
        m_Head->m_Size = static_cast<int32>(size - sizeof(MemoryChunkHeader));
    }

    FreeListAllocator::~FreeListAllocator() {}

    void* FreeListAllocator::Allocate(size_t size, const size_t& alignment)
    {
        SAVANNA_INSERT_SCOPED_PROFILER("FreeListAllocator::Allocate");
        MemoryChunkHeader* current = m_Head;
        MemoryChunkHeader* previous = nullptr;

        MemoryChunkHeader* bestFit = nullptr;
        MemoryChunkHeader* prevBestFit = nullptr;

        size_t bestFitSize, requiredSize, forwardAdjustment;

        while (current != nullptr)
        {
            forwardAdjustment = GetForwardAlignment(&current[1], alignment);
            requiredSize = size + forwardAdjustment;
            if (current->m_Size >= requiredSize
                && (bestFit == nullptr || current->m_Size < bestFitSize))
            {
                bestFit = current;
                bestFitSize = current->m_Size;
                prevBestFit = previous;

                if (current->m_Size == requiredSize)
                {
                    break;
                }
            }

            previous = current;
            current = current->m_Next;
        }

        if (bestFit == nullptr)
        {
            return nullptr;
        }

        void* outPtr = Add(&bestFit[1], forwardAdjustment);
        AllocatedChunkDescriptor* descPtr = &reinterpret_cast<AllocatedChunkDescriptor*>(outPtr)[-1];

        if (bestFit->m_Size - requiredSize > sizeof(MemoryChunkHeader))
        {
            MemoryChunkHeader* newChunk = Add(bestFit + 1, requiredSize);
            newChunk->m_Size = bestFit->m_Size - requiredSize - sizeof(MemoryChunkHeader);
            newChunk->m_Next = bestFit->m_Next;

            if (prevBestFit != nullptr)
            {
                prevBestFit->m_Next = newChunk;
            }
            else
            {
                m_Head = newChunk;
            }

            descPtr->m_Size = requiredSize + sizeof(MemoryChunkHeader);
        }
        else
        {
            if (prevBestFit != nullptr)
            {
                prevBestFit->m_Next = bestFit->m_Next;
            }
            else
            {
                m_Head = bestFit->m_Next;
            }

            descPtr->m_Size = bestFit->m_Size;
        }

        m_AllocatedBytes += descPtr->m_Size;

        descPtr->m_Offset = forwardAdjustment;

        return outPtr;
    }

    void FreeListAllocator::Deallocate(void* const ptr, const size_t alignment)
    {
        SAVANNA_INSERT_SCOPED_PROFILER("FreeListAllocator::Deallocate");
        SAVANNA_MEMORY_SAFETY_ASSERT(ptr != nullptr && alignment > 0, "Invalid arguments");
        SAVANNA_MEMORY_SAFETY_ASSERT(m_AllocatedBytes > sizeof(MemoryChunkHeader), "Attempting to deallocate pointer from an empty allocator");

        AllocatedChunkDescriptor allocationDescriptor =
            reinterpret_cast<AllocatedChunkDescriptor*>(ptr)[-1];

        // Subtract the stored alignment adjustment to get the end of the actual header index into the pointer one place backwards to
        MemoryChunkHeader* allocatedHeader =
            &reinterpret_cast<MemoryChunkHeader*>(Subtract(ptr, allocationDescriptor.m_Offset))[-1];
        allocatedHeader->m_Size = allocationDescriptor.m_Size;

        MemoryChunkHeader* previousHeader = nullptr;
        MemoryChunkHeader* currentHeader = m_Head;
        while (currentHeader != nullptr)
        {
            if (reinterpret_cast<uintptr>(currentHeader) >= reinterpret_cast<uintptr>(allocatedHeader) + allocatedHeader->m_Size)
            {
                break;
            }

            previousHeader = currentHeader;
            currentHeader = currentHeader->m_Next;
        }

        if (previousHeader == nullptr)
        {
            previousHeader = allocatedHeader;
            allocatedHeader->m_Next = m_Head;
            m_Head = allocatedHeader;
        }
        else if (reinterpret_cast<uintptr>(previousHeader) + previousHeader->m_Size == reinterpret_cast<uintptr>(allocatedHeader))
        {
            previousHeader->m_Size += allocatedHeader->m_Size;
        }
        else
        {
            previousHeader->m_Next = allocatedHeader;
            allocatedHeader->m_Next = currentHeader;
        }

        m_AllocatedBytes -= allocatedHeader->m_Size;

        if (Add(previousHeader, previousHeader->m_Size) == previousHeader->m_Next)
        {
            previousHeader->m_Size += currentHeader->m_Size;
            previousHeader->m_Next = previousHeader->m_Next->m_Next;
        }
    }

    FreeListAllocator& FreeListAllocator::operator=(FreeListAllocator&& other)
    {
        if (this != &other)
        {
            m_Root = other.m_Root;
            m_Head = other.m_Head;
            m_Size = other.m_Size;
            m_AllocatedBytes = other.m_AllocatedBytes;
        }
        return *this;
    }
} // namespace Savanna
