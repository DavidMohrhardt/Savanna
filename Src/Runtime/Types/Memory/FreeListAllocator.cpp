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
#if SAVANNA_ENABLE_RUNTIME_MEMORY_VALIDATION
    inline bool IsLinkedListCyclic(const MemoryChunkDescriptor* pHead)
    {
        // Find cycles in the linked list
        const MemoryChunkDescriptor* pSlow = pHead;
        const MemoryChunkDescriptor* pFast = pHead;
        while (pFast != nullptr && pFast->m_Next != nullptr)
        {
            pSlow = pSlow->m_Next;
            pFast = pFast->m_Next->m_Next;

            if (pSlow == pFast)
            {
                return true;
            }
        }
        return false;
    }

    inline bool IsPointerInList(MemoryChunkDescriptor* head, void* ptr)
    {
        MemoryChunkDescriptor* pCurrent = head;
        while (pCurrent != nullptr)
        {
            if (pCurrent == ptr)
            {
                return true;
            }
            pCurrent = pCurrent->m_Next;
        }
        return false;
    }
#endif

    AllocationDescriptor FreeListAllocator::GetAllocationDescriptor(void *const ptr) const
    {
        AllocationDescriptor allocationDescriptor = reinterpret_cast<AllocationDescriptor*>(ptr)[-1];
        return allocationDescriptor;
    }

    FreeListAllocator::FreeListAllocator(
        size_t size, AllocatorKind allocatorKind /*= k_SavannaAllocatorKindHeap*/)
        : m_AllocatorKind{allocatorKind}
        , m_Head{nullptr}
        , m_Size{size}
        , m_AllocatedBytes{0}
    {
        SAVANNA_ASSERT(allocatorKind != k_SavannaAllocatorKindNone, "Invalid memory allocatorKind for memory owning allocator");
        SAVANNA_ASSERT(size > 0, "Invalid size for memory owning allocator");

        m_Head = SAVANNA_NEW(allocatorKind, MemoryChunkDescriptor, size);
        m_Root = m_Head;
    }

    FreeListAllocator::FreeListAllocator(void* root, size_t size)
        : m_Root(root)
        , m_AllocatorKind { k_SavannaAllocatorKindNone }
        , m_Head(reinterpret_cast<MemoryChunkDescriptor*>(root))
        , m_Size(root != nullptr ? size : 0)
        , m_AllocatedBytes(0)
    {
        if (m_Head != nullptr)
        {
            *m_Head = MemoryChunkDescriptor(size);
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
#if SAVANNA_ENABLE_RUNTIME_MEMORY_VALIDATION
        if (m_Allocations.size() > 0)
        {
            SAVANNA_LOG("Memory leak detected");
            for (auto& [ptr, size] : m_Allocations)
            {
                SAVANNA_LOG("Leaked {} bytes at {}", size, ptr);
            }
        }
#endif // SAVANNA_ENABLE_RUNTIME_MEMORY_VALIDATION

        if (m_AllocatorKind != k_SavannaAllocatorKindNone)
        {
            SAVANNA_DELETE(m_AllocatorKind, m_Head);
        }

        m_Head = nullptr;
        m_Size = 0;
        m_AllocatedBytes = 0;
    }

    FreeListAllocator& FreeListAllocator::operator=(FreeListAllocator&& other)
    {
        if (this != &other)
        {
            SAVANNA_MOVE_MEMBER(m_Root, other);
            SAVANNA_MOVE_MEMBER(m_AllocatorKind, other);
            SAVANNA_MOVE_MEMBER(m_Head, other);
            SAVANNA_MOVE_MEMBER(m_Size, other);
            SAVANNA_MOVE_MEMBER(m_AllocatedBytes, other);
        }
        return *this;
    }

    MemoryChunkDescriptor* FreeListAllocator::FindBestFit(
        const size_t &size,
        const size_t &alignment,
        AllocationDescriptor &outAllocationDescriptor,
        MemoryChunkDescriptor*& pPrevious)
    {
        MemoryChunkDescriptor *pCurrent = m_Head;
        pPrevious = nullptr;
        MemoryChunkDescriptor *pBestFit = nullptr;
        while (pCurrent != nullptr)
        {
            // Manually calculate the forward alignment
            void* pBuffer = (void*)&pCurrent[1];
            size_t forwardAlignment = GetForwardAlignment(pBuffer, alignment);
            size_t requiredSize = size + forwardAlignment;

            if (pCurrent->IsPerfectFit(requiredSize))
            {
                outAllocationDescriptor.m_Offset = forwardAlignment;
                outAllocationDescriptor.m_Size = requiredSize;
                pBestFit = pCurrent;
                break;
            }
            else if (pCurrent->m_Size > requiredSize &&
                (pBestFit == nullptr || pBestFit->m_Size < pCurrent->m_Size))
            {
                outAllocationDescriptor.m_Offset = forwardAlignment;
                outAllocationDescriptor.m_Size = requiredSize;
                pBestFit = pCurrent;
            }

            if (pCurrent->m_Next == nullptr)
            {
                break;
            }

            pPrevious = pCurrent;
            pCurrent = pCurrent->m_Next;
        }

        return pBestFit;
    }

    void* FreeListAllocator::alloc(const size_t& size, const size_t& alignment)
    {
        SAVANNA_ASSERT(size > 0, "Invalid size for allocation");
        SAVANNA_ASSERT(alignment > 0, "Invalid alignment for allocation");
        SAVANNA_ASSERT(m_Head != nullptr, "Allocator is not initialized");

        AllocationDescriptor allocationDescriptor {};
        MemoryChunkDescriptor* pPrevious = nullptr;
        MemoryChunkDescriptor* pBestFit = FindBestFit(size, alignment, allocationDescriptor, pPrevious);

        if (pBestFit == nullptr)
        {
            return nullptr;
        }

        void *pAllocation = Add(&pBestFit[1], allocationDescriptor.m_Offset);

        if (pBestFit->m_Size < allocationDescriptor.m_Size + sizeof(MemoryChunkDescriptor))
        {
            // Can't write another header, so just allocate the whole chunk
            allocationDescriptor.m_Size = pBestFit->m_Size;
        }
        else
        {
            // Write a new header after the allocation
            MemoryChunkDescriptor *pNewHeader =
                reinterpret_cast<MemoryChunkDescriptor *>(Add(pAllocation, allocationDescriptor.m_Size));
            pNewHeader[0] = MemoryChunkDescriptor(pBestFit->m_Size - allocationDescriptor.m_Size, pBestFit->m_Next);
            pBestFit->m_Next = pNewHeader;

            // Only add the size of the header if we actually wrote a new header
            m_AllocatedBytes += sizeof(MemoryChunkDescriptor);
        }

        // Fixup the list
        if (pBestFit == m_Head)
        {
            m_Head = pBestFit->m_Next;
        }
        else if (pPrevious != pBestFit)
        {
            pPrevious->m_Next = pBestFit->m_Next;
        }
        else
        {
            SAVANNA_ASSERT(false, "This should never happen");
        }

        m_AllocatedBytes += allocationDescriptor.m_Size;

        // Write the allocation descriptor behind the allocation
        reinterpret_cast<AllocationDescriptor*>(pAllocation)[-1] = allocationDescriptor;

#if SAVANNA_ENABLE_RUNTIME_MEMORY_VALIDATION
        SAVANNA_ASSERT(m_Allocations.find(pAllocation) == m_Allocations.end(), "Pointer was already in the allocation list!");
        m_Allocations[pAllocation] = allocationDescriptor.m_Size;

        SAVANNA_ASSERT(!IsLinkedListCyclic(m_Head), "Linked list is cyclic, there is a bug in the allocator");
        SAVANNA_ASSERT(!IsPointerInList(m_Head, pBestFit), "Pointer was still in the header list!");
#endif

        return pAllocation;
    }

    void FreeListAllocator::free(void* const ptr, const size_t& alignment)
    {
        // SAVANNA_INSERT_SCOPED_PROFILER(FreeListAllocator::free);
        SAVANNA_ASSERT(ptr != nullptr && alignment > 0, "Invalid arguments");
        SAVANNA_ASSERT(m_AllocatedBytes > sizeof(MemoryChunkDescriptor), "Attempting to deallocate pointer from an empty allocator");
        SAVANNA_ASSERT(ptr >= m_Root && ptr < Add(m_Root, m_Size), "Attempting to deallocate pointer from an allocator that does not own it");

#if SAVANNA_ENABLE_RUNTIME_MEMORY_VALIDATION
        SAVANNA_ASSERT(m_Allocations.find(ptr) != m_Allocations.end(), "Pointer is not known to the allocator");
#endif

        AllocationDescriptor allocationDescriptor = GetAllocationDescriptor(ptr);

        MemoryChunkDescriptor* pCurrent = m_Head;
        MemoryChunkDescriptor* pPrevious = nullptr;
        while (pCurrent != nullptr)
        {
            if (pCurrent->IsGreaterOrEqual(ptr))
            {
                break;
            }
            pPrevious = pCurrent;
            pCurrent = pCurrent->m_Next;
        }

        MemoryChunkDescriptor* pChunk = reinterpret_cast<MemoryChunkDescriptor*>(
            Subtract(ptr, sizeof(AllocationDescriptor) + allocationDescriptor.m_Offset));

        if (pPrevious != nullptr)
        {
            SAVANNA_ASSERT(pPrevious != pChunk, "Pointer was still in the header list!");
            if (pPrevious->IsAdjacent(pChunk))
            {
                // Merge with previous chunk
                pPrevious->m_Size += allocationDescriptor.m_Size + sizeof(MemoryChunkDescriptor);
                m_AllocatedBytes -= sizeof(MemoryChunkDescriptor);
            }
            else
            {
                // Insert chunk before current
                pChunk[0] = MemoryChunkDescriptor(allocationDescriptor.m_Size, pCurrent);
                pPrevious->m_Next = pChunk;
            }
        }
        else
        {
            // Insert chunk before current
            pChunk[0] = MemoryChunkDescriptor(allocationDescriptor.m_Size, pCurrent);
            m_Head = pChunk;
        }

        m_AllocatedBytes -= allocationDescriptor.m_Size;

#if SAVANNA_ENABLE_RUNTIME_MEMORY_VALIDATION
        m_Allocations.erase(ptr);
        SAVANNA_ASSERT(!IsLinkedListCyclic(m_Head), "Linked list is cyclic, there is a bug in the allocator");
        SAVANNA_ASSERT(m_Allocations.find(ptr) == m_Allocations.end(), "Pointer was still in the allocation list!");
#endif
    }

    void* FreeListAllocator::realloc(void* const ptr, const size_t& newSize, const size_t& alignment)
    {
        // SAVANNA_INSERT_SCOPED_PROFILER(FreeListAllocator::realloc);
        SAVANNA_ASSERT(ptr != nullptr && newSize > 0 && alignment > 0, "Invalid arguments");
        SAVANNA_ASSERT(m_AllocatedBytes > sizeof(MemoryChunkDescriptor), "Attempting to reallocate pointer from an empty allocator");
        SAVANNA_ASSERT(ptr >= m_Root && ptr < Add(m_Root, m_Size), "Attempting to reallocate pointer from an allocator that does not own it");

#if SAVANNA_ENABLE_RUNTIME_MEMORY_VALIDATION
        SAVANNA_ASSERT(m_Allocations.find(ptr) != m_Allocations.end(), "Pointer is not known to the allocator");
#endif

        AllocationDescriptor allocationDescriptor = GetAllocationDescriptor(ptr);

        if (allocationDescriptor.m_Size == newSize)
        {
            // The allocation is already big enough
            return ptr;
        }
        else if (allocationDescriptor.m_Size > newSize)
        {
            // The original allocation is bigger, so we can just shrink it
            AllocationDescriptor* pAllocationDescriptor = reinterpret_cast<AllocationDescriptor*>(ptr) - 1;
            pAllocationDescriptor->m_Size = newSize;
            // Write a new header after the allocation
            MemoryChunkDescriptor* pNewHeader =
                reinterpret_cast<MemoryChunkDescriptor*>(Add(ptr, newSize));
            pNewHeader[0] = MemoryChunkDescriptor(allocationDescriptor.m_Size - newSize, nullptr);
            MemoryChunkDescriptor* pCurrent = m_Head;
            MemoryChunkDescriptor* pPrevious = nullptr;
            while (pCurrent != nullptr)
            {
                if (pCurrent->IsGreaterOrEqual(ptr))
                {
                    break;
                }
                pPrevious = pCurrent;
                pCurrent = pCurrent->m_Next;
            }
            if (pPrevious != nullptr)
            {
                SAVANNA_ASSERT(pPrevious != pNewHeader, "Pointer was still in the header list!");
                if (pPrevious->IsAdjacent(pNewHeader))
                {
                    // Merge with previous chunk
                    pPrevious->m_Size += allocationDescriptor.m_Size - newSize;
                    m_AllocatedBytes -= sizeof(MemoryChunkDescriptor);
                }
                else
                {
                    // Insert chunk before current
                    pNewHeader[0] = MemoryChunkDescriptor(allocationDescriptor.m_Size - newSize, pCurrent);
                    pPrevious->m_Next = pNewHeader;
                }
            }
            else
            {
                // Insert chunk before current
                pNewHeader[0] = MemoryChunkDescriptor(allocationDescriptor.m_Size - newSize, pCurrent);
                m_Head = pNewHeader;
            }
            return ptr;
        }

        // The allocation is too small, allocate a new one and copy the data
        void* pNewPtr = alloc(newSize, alignment);
        if (pNewPtr == nullptr)
        {
            return nullptr;
        }

        // Copy the data from the old allocation to the new one
        memcpy(pNewPtr, ptr, allocationDescriptor.m_Size);

        // Free the old allocation
        free(ptr, alignment);
        return pNewPtr;
    }

} // namespace Savanna

