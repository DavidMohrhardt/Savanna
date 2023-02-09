/**
 * @file ExpandableBlockAllocator.cpp
 * @author David Mohrhardt (https://github.com/DavidMohrhardt/Savanna)
 * @brief
 * @version 0.1
 * @date 2023-02-03
 *
 * @copyright Copyright (c) 2023
 *
 */
#include "ExpandableBlockAllocator.h"

#include "Types/Exceptions/SavannaException.h"

#include "Profiling/Profiler.h"

namespace Savanna
{
    inline static void CreateBufferAndInitHeaders(
        const size_t bufferSize,
        MemoryBuffer& outBuffer,
        MemoryChunkDescriptor** ppOutHead,
        MemoryChunkDescriptor** ppOutTail)
    {
        SAVANNA_ASSERT(ppOutHead != nullptr, "Head is null!");
        SAVANNA_ASSERT(ppOutTail != nullptr, "Tail is null!");

        outBuffer = std::move(MemoryBuffer(bufferSize));
        if (outBuffer.GetBuffer() == nullptr)
        {
            throw BadAllocationException();
        }

        size_t bufferBlockSize = outBuffer.GetSize();

        MemoryChunkDescriptor* pBufferHead = reinterpret_cast<MemoryChunkDescriptor*>(outBuffer.GetBuffer());
        MemoryChunkDescriptor* pBufferTail = GetBackwardAlignedPtr<MemoryChunkDescriptor, MemoryChunkDescriptor>(
            reinterpret_cast<MemoryChunkDescriptor*>(
                (reinterpret_cast<uint8_t*>(outBuffer.GetBuffer()) + bufferBlockSize) - sizeof(MemoryChunkDescriptor)
            ),
            alignof(MemoryChunkDescriptor));


        pBufferHead->m_Size = GetPointerDifference(pBufferTail, (&pBufferHead[1]));
        pBufferHead->m_Next = pBufferTail;
        *ppOutHead = pBufferHead;

        pBufferTail->m_Size = 0;
        pBufferTail->m_Next = nullptr;
        *ppOutTail = pBufferTail;
    }

    ExpandableBlockAllocator::ExpandableBlockAllocator(size_t initialBufferCount, size_t bufferBlockSize)
        : m_BufferBlockSize(bufferBlockSize)
        , m_MemoryPoolContainer(initialBufferCount)
        , m_Head(nullptr)
        , m_Tail(nullptr)
    {
        for (int i = 0; i < initialBufferCount; ++i)
        {
            MemoryChunkDescriptor* pBufferHead, *pBufferTail;
            MemoryBuffer buffer;
            CreateBufferAndInitHeaders(m_BufferBlockSize, buffer, &pBufferHead, &pBufferTail);

            if (i == 0)
            {
                m_Head = pBufferHead;
            }
            else
            {
                m_Tail->m_Next = pBufferHead;
            }

            m_Tail = pBufferTail;
        }
    }

    ExpandableBlockAllocator::~ExpandableBlockAllocator()
    {
        for (MemoryBuffer& buffer : m_MemoryPoolContainer)
        {
            buffer.~MemoryBuffer();
        }
    }

    void* ExpandableBlockAllocator::alloc(const size_t& size, const size_t& alignment)
    {
        SAVANNA_INSERT_SCOPED_PROFILER(ExpandableBlockAllocator::alloc);

        if (m_Head == nullptr)
        {
            m_Head = AllocateAdditionalBuffers(m_BufferBlockSize);
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

            if (current->m_Next == nullptr) SAVANNA_BRANCH_HINT(unlikely)
            {
                current->m_Next = AllocateAdditionalBuffers(requiredSize);
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

    void ExpandableBlockAllocator::free(void* const ptr, const size_t& alignment)
    {
        SAVANNA_INSERT_SCOPED_PROFILER("ExpandableBlockAllocator::free");
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

    MemoryChunkDescriptor* ExpandableBlockAllocator::AllocateAdditionalBuffers(size_t minimumSize)
    {
        if (minimumSize < m_BufferBlockSize)
        {
            m_BufferBlockSize = NextPowerOfTwo(minimumSize);
        }

        MemoryChunkDescriptor* pBufferHead, *pBufferTail;
        m_MemoryPoolContainer.emplace_back(MemoryBuffer());
        MemoryBuffer& buffer = m_MemoryPoolContainer.back();
        CreateBufferAndInitHeaders(m_BufferBlockSize, buffer, &pBufferHead, &pBufferTail);

        m_Tail->m_Next = pBufferHead;
        m_Tail = pBufferTail;

        return pBufferHead;
    }
} // namespace Savanna
