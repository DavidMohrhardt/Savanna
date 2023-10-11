/**
 * @file FreeListAllocator.cpp
 * @author David Mohrhardt (https://github.com/DavidMohrhardt/Savanna)
 * @brief TODO @DavidMohrhardt Document
 * @version 0.1
 * @date 2022-08-02
 *
 */
#include "FreeListAllocator.h"

#include "Memory/MemoryManager.h"

#include "Math/MathHelpers.h"
#include "Math/PointerMath.h"
#include "Profiling/Profiler.h"
#include "Utilities/Assertions/SavannaAssertions.h"

#include "Utilities/Semantics/Move.h"

namespace Savanna
{
    FreeListAllocator::FreeListAllocator(size_t size, MemoryLabel label /*= k_SavannaMemoryLabelHeap*/)
        : m_MemoryLabel { label }
        , m_Head { nullptr }
        , m_Size { size }
        , m_AllocatedBytes { 0 }
    {
        SAVANNA_ASSERT(label != k_SavannaMemoryLabelNone, "Invalid memory label for memory owning allocator");
        SAVANNA_ASSERT(size > 0, "Invalid size for memory owning allocator");

        m_Head = reinterpret_cast<MemoryChunkDescriptor*>(
            MemoryManager::Get().Allocate(
                size + sizeof(MemoryChunkDescriptor),
                alignof(MemoryChunkDescriptor),
                label)
            );
        m_Head->m_Size = size;
        m_Head->m_Next = nullptr;
    }

    FreeListAllocator::FreeListAllocator(void* root, size_t size)
        : Allocator(root)
        , m_MemoryLabel { k_SavannaMemoryLabelNone }
        , m_Head(reinterpret_cast<MemoryChunkDescriptor*>(root))
        , m_Size(root != nullptr ? size : 0)
        , m_AllocatedBytes(0)
    {
        if (m_Head != nullptr)
        {
            m_Head->m_Next = nullptr;
            m_Size = size;
            m_Head->m_Size = static_cast<int32>(size);
            m_AllocatedBytes = sizeof(MemoryChunkDescriptor);
        }
    }

    FreeListAllocator::FreeListAllocator(FreeListAllocator&& other)
    {
        *this = std::move(other);
    }

    /**
     * @brief FreeListAllocator do not own their memory and therefore have nothing to do
     *       with the destructor.
     */
    FreeListAllocator::~FreeListAllocator()
    {
        if (m_MemoryLabel != k_SavannaMemoryLabelNone)
        {
            MemoryManager::Get().Free(m_Head, m_MemoryLabel);
            m_Head = nullptr;
            m_Size = 0;
            m_AllocatedBytes = 0;
        }
    }

    FreeListAllocator& FreeListAllocator::operator=(FreeListAllocator&& other)
    {
        if (this != &other)
        {
            Allocator::operator=(static_cast<Allocator&&>(other));
            SAVANNA_MOVE_MEMBER(m_MemoryLabel, other);
            SAVANNA_MOVE_MEMBER(m_Head, other);
            SAVANNA_MOVE_MEMBER(m_Size, other);
            SAVANNA_MOVE_MEMBER(m_AllocatedBytes, other);
        }
        return *this;
    }

    void* FreeListAllocator::alloc(const size_t& size, const size_t& alignment)
    {
        SAVANNA_INSERT_SCOPED_PROFILER(FreeListAllocator::alloc);

        if (size > m_Size - m_AllocatedBytes)
        {
            return nullptr;
        }

        MemoryChunkDescriptor* current = m_Head;
        MemoryChunkDescriptor* previous = nullptr;

        MemoryChunkDescriptor* bestFit = nullptr;
        MemoryChunkDescriptor* prevBestFit = nullptr;

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

        if (bestFit->m_Size - requiredSize > sizeof(MemoryChunkDescriptor))
        {
            MemoryChunkDescriptor* newChunk = Add(bestFit + 1, requiredSize);
            newChunk->m_Size = bestFit->m_Size - requiredSize - sizeof(MemoryChunkDescriptor);
            newChunk->m_Next = bestFit->m_Next;

            if (prevBestFit != nullptr)
            {
                prevBestFit->m_Next = newChunk;
            }
            else
            {
                m_Head = newChunk;
            }

            descPtr->m_Size = requiredSize + sizeof(MemoryChunkDescriptor);
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

    void FreeListAllocator::free(void* const ptr, const size_t& alignment)
    {
        SAVANNA_INSERT_SCOPED_PROFILER("FreeListAllocator::free");
        SAVANNA_ASSERT(ptr != nullptr && alignment > 0, "Invalid arguments");
        SAVANNA_ASSERT(m_AllocatedBytes > sizeof(MemoryChunkDescriptor), "Attempting to deallocate pointer from an empty allocator");

        AllocatedChunkDescriptor allocationDescriptor =
            reinterpret_cast<AllocatedChunkDescriptor*>(ptr)[-1];

        // Subtract the stored alignment adjustment to get the end of the actual header index into the pointer one place backwards to
        MemoryChunkDescriptor* allocatedHeader =
            &reinterpret_cast<MemoryChunkDescriptor*>(Subtract(ptr, allocationDescriptor.m_Offset))[-1];
        allocatedHeader->m_Size = allocationDescriptor.m_Size;

        MemoryChunkDescriptor* previousHeader = nullptr;
        MemoryChunkDescriptor* currentHeader = m_Head;
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

} // namespace Savanna

