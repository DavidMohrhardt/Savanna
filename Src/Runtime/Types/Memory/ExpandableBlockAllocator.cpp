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

#include "Memory/MemoryManager.h"

#include "Types/Exceptions/SavannaException.h"

#include "Profiling/Profiler.h"

namespace Savanna
{
#if TODO_SPLIT_CONTIGUOUS_BUFFERS
    inline static void SplitBufferIntoSubBuffers(
        MemoryBuffer& buffer,
        const size_t bufferSize,
        const size_t subBufferSize)
    {
        SAVANNA_ASSERT(buffer.GetBuffer() != nullptr, "Buffer is null!");
        SAVANNA_ASSERT(bufferSize > 0, "Buffer size is 0!");
        SAVANNA_ASSERT(subBufferSize > 0, "Sub buffer size is 0!");
        SAVANNA_ASSERT(subBufferSize % bufferSize == 0, "Sub buffer size is not a multiple of buffer size!");

        size_t bufferBlockSize = buffer.GetSize();
        SAVANNA_ASSERT(subBufferSize >= bufferSize, "Buffer size is smaller than requested size!");

        size_t subBufferCount = bufferBlockSize / subBufferSize;
        SAVANNA_ASSERT(subBufferCount > 0, "Sub buffer count is 0!");

        uint8_t* pBuffer = reinterpret_cast<uint8_t*>(buffer.GetBuffer());
        for (size_t i = 0; i < subBufferCount; ++i)
        {
            MemoryChunkDescriptor* pBufferHead = reinterpret_cast<MemoryChunkDescriptor*>(pBuffer);
            MemoryChunkDescriptor* pBufferTail = GetBackwardAlignedPtr<MemoryChunkDescriptor, MemoryChunkDescriptor>(
                reinterpret_cast<MemoryChunkDescriptor*>(
                    (pBuffer + subBufferSize) - sizeof(MemoryChunkDescriptor)
                ),
                alignof(MemoryChunkDescriptor));

            pBufferHead->m_Size = GetPointerDifference(pBufferTail, (&pBufferHead[1]));
            pBufferHead->m_Next = pBufferTail;

            pBufferTail->m_Size = 0;
            pBufferTail->m_Next = nullptr;

            pBuffer += subBufferSize;
        }
    }
#endif
    ExpandableBlockAllocator::ExpandableBlockAllocator(
        bool contiguous /*= true*/,
        const MemoryLabel label /*= k_SavannaMemoryLabelHeap*/)
        : ExpandableBlockAllocator(0, SAVANNA_DEFAULT_MEMORY_POOL_SIZE, contiguous, label)
    {}

    ExpandableBlockAllocator::ExpandableBlockAllocator(
        size_t initialBufferCount,
        size_t bufferBlockSize,
        bool contiguous /*= true*/,
        const MemoryLabel label /*= k_SavannaMemoryLabelHeap*/)
        : Allocator(nullptr)
        , m_MemoryLabel {label}
        , m_Head {nullptr}
        , m_Tail {nullptr}
        , m_BufferBlockSize{bufferBlockSize}
        , m_AllocatedBytes{initialBufferCount > 0 ? initialBufferCount : 1}
        , m_Size{0}
        , m_MemoryPoolContainer(initialBufferCount > 0 ? initialBufferCount : 1, MemoryManager::GetAllocatorInterfaceForLabel(label))
    {
        SAVANNA_INSERT_SCOPED_PROFILER(ExpandableBlockAllocator::ctor);
        // If the buffers are initially contiguous, we need to allocate a single buffer that is large enough to hold all the buffers
        if (contiguous)
        {
            bufferBlockSize *= initialBufferCount;
            initialBufferCount = 1;
        }

        MemoryChunkDescriptor *pBufferHead, *pBufferTail;
        for (int i = 0; i < initialBufferCount; ++i)
        {
            CreateBufferWithMemoryChunkDescs(bufferBlockSize, &pBufferHead, &pBufferTail);

            if (i == 0)
            {
                m_Head = pBufferHead;
            }
            else
            {
                m_Tail->m_Next = pBufferHead;
            }

            m_Tail = pBufferTail;
            m_AllocatedBytes += sizeof(MemoryChunkDescriptor) * 2;
        }

#if TODO_SPLIT_CONTIGUOUS_BUFFERS
        if (contiguous)
        {
            // Split the buffer into the individual buffers of size m_BufferBlockSize
            MemoryChunkDescriptor* current = m_Head;
            MemoryChunkDescriptor* previous = nullptr;
            MemoryChunkDescriptor* tail = m_Head->m_Next;
        }
#endif

        m_Root = m_Head;
    }

    ExpandableBlockAllocator::~ExpandableBlockAllocator()
    {
        SAVANNA_INSERT_SCOPED_PROFILER(ExpandableBlockAllocator::dtor);
        // Memory is in the form of a MemoryBuffer class which will automatically free the memory
        // Since it's in a vector, it will be freed when the vector goes out of scope

#if SAVANNA_ENABLE_RIGOROUS_MEMORY_VALIDATION
        SAVANNA_ASSERT(m_AllocationCount == m_FreeCount, "Memory leak detected!");
#endif
    }

    ExpandableBlockAllocator& ExpandableBlockAllocator::operator=(ExpandableBlockAllocator &&other)
    {
        if (this != &other)
        {
            Allocator::operator=(std::move(other));

            SAVANNA_MOVE_MEMBER(m_BufferBlockSize, other);
            SAVANNA_MOVE_MEMBER(m_MemoryPoolContainer, other);
            SAVANNA_MOVE_MEMBER(m_Head, other);
            SAVANNA_MOVE_MEMBER(m_Tail, other);
            SAVANNA_MOVE_MEMBER(m_AllocatedBytes, other);
            SAVANNA_MOVE_MEMBER(m_Size, other);
        }

        return *this;
    }

    void* ExpandableBlockAllocator::alloc(const size_t& size, const size_t& alignment)
    {
        SAVANNA_INSERT_SCOPED_PROFILER(ExpandableBlockAllocator::alloc);

        if (m_Head == nullptr)
        {
            m_Head = AllocateAdditionalBuffers(m_BufferBlockSize > size ? m_BufferBlockSize : size);
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

            if (bestFit == nullptr && current->m_Next == nullptr) SAVANNA_BRANCH_HINT(unlikely)
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
#if SAVANNA_ENABLE_RIGOROUS_MEMORY_VALIDATION || true
        m_AllocationCount++;
#endif

        return outPtr;
    }

    void ExpandableBlockAllocator::free(void* const ptr, const size_t& alignment)
    {
        SAVANNA_INSERT_SCOPED_PROFILER(ExpandableBlockAllocator::free);
        SAVANNA_ASSERT(ptr != nullptr && alignment > 0, "Invalid arguments");
        SAVANNA_ASSERT(m_AllocatedBytes > sizeof(MemoryChunkDescriptor), "Attempting to deallocate pointer from an empty allocator");

#if SAVANNA_ENABLE_RIGOROUS_MEMORY_VALIDATION
        {
            // Check if the pointer is within the bounds of the allocator
            bool found = false;
            uintptr_t ptrInt = reinterpret_cast<uintptr_t>(ptr);
            for (const auto& buffer : m_MemoryPoolContainer)
            {
                if (ptrInt - reinterpret_cast<uintptr_t>(buffer.GetBuffer()) < buffer.GetSize())
                {
                    found = true;
                    break;
                }
            }
            SAVANNA_ASSERT(found, "Attempting to deallocate pointer that was not allocated by this allocator");
        }
#endif

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

#if SAVANNA_ENABLE_RIGOROUS_MEMORY_VALIDATION || true
        m_FreeCount++;
#endif
    }

    MemoryChunkDescriptor* ExpandableBlockAllocator::AllocateAdditionalBuffers(size_t minimumSize)
    {
        SAVANNA_INSERT_SCOPED_PROFILER(ExpandableBlockAllocator::AllocateAdditionalBuffers);
        if (m_BufferBlockSize < minimumSize)
        {
            m_BufferBlockSize = NextPowerOfTwo(minimumSize);
        }

        MemoryChunkDescriptor* pBufferHead, *pBufferTail;
        // m_MemoryPoolContainer.push_back(MemoryBuffer());
        CreateBufferWithMemoryChunkDescs(m_BufferBlockSize, &pBufferHead, &pBufferTail);

        if (m_Head == nullptr) SAVANNA_BRANCH_HINT(unlikely)
        {
            m_Head = pBufferHead;
        }
        if (m_Tail != nullptr) SAVANNA_BRANCH_HINT(likely)
        {
            m_Tail->m_Next = pBufferHead;
        }
        m_Tail = pBufferTail;

        return pBufferHead;
    }

    inline void ExpandableBlockAllocator::CreateBufferWithMemoryChunkDescs(
        const size_t bufferSize,
        MemoryChunkDescriptor** ppOutHead,
        MemoryChunkDescriptor** ppOutTail)
    {
        SAVANNA_INSERT_SCOPED_PROFILER(ExpandableBlockAllocator::CreateBufferWithMemoryChunkDescs);
        SAVANNA_ASSERT(ppOutHead != nullptr, "Head is null!");
        SAVANNA_ASSERT(ppOutTail != nullptr, "Tail is null!");

        size_t newBufferSize = bufferSize + sizeof(MemoryChunkDescriptor) * 2;

        m_MemoryPoolContainer.Append(MemoryBuffer(newBufferSize, m_MemoryLabel));
        MemoryBuffer& buffer = m_MemoryPoolContainer[m_MemoryPoolContainer.Size() - 1];
        if (!buffer.IsValid())
        {
            throw BadAllocationException();
        }

        m_Size += buffer.GetSize();

        size_t bufferBlockSize = buffer.GetSize();

        MemoryChunkDescriptor* pBufferHead = reinterpret_cast<MemoryChunkDescriptor*>(buffer.GetBuffer());
        MemoryChunkDescriptor* pBufferTail = GetBackwardAlignedPtr<MemoryChunkDescriptor, MemoryChunkDescriptor>(
            reinterpret_cast<MemoryChunkDescriptor*>(
                (reinterpret_cast<uint8_t*>(buffer.GetBuffer()) + bufferBlockSize) - sizeof(MemoryChunkDescriptor)
            ),
            alignof(MemoryChunkDescriptor));


        pBufferHead->m_Size = GetPointerDifference(pBufferTail, (&pBufferHead[1]));
        pBufferHead->m_Next = pBufferTail;
        *ppOutHead = pBufferHead;

        pBufferTail->m_Size = 0;
        pBufferTail->m_Next = nullptr;
        *ppOutTail = pBufferTail;
    }

} // namespace Savanna
