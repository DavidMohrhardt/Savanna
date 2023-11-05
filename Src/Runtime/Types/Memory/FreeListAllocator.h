/**
 * @file FreeListAllocator.h
 * @author David Mohrhardt (https://github.com/DavidMohrhardt/Savanna)
 * @brief TODO @DavidMohrhardt Document
 * @version 0.1
 * @date 2022-08-02
 *
 */

#pragma once

#include "Utilities/SavannaCoding.h"

#include "Allocator.h"
#include "AtomicAllocatorWrapper.h"
#include "MemoryBuffer.h"
#include "MemoryChunkDescriptors.h"

#if SAVANNA_ENABLE_RUNTIME_MEMORY_VALIDATION
#include <unordered_map>
#endif // SAVANNA_ENABLE_RUNTIME_MEMORY_VALIDATION

namespace Savanna
{
    struct MemoryChunkDescriptor;

    /**
     * @brief
     */
    class FreeListAllocator : public Allocator
    {
    private:
        // Some other implementations use this allocator as a
        // member variable, allow those allocators to query the
        // information for allocations made by these allocators.
        friend struct MultiListAllocator;

        MemoryLabel m_MemoryLabel;
        void* m_Root;
        MemoryChunkDescriptor* m_Head;
        size_t m_Size;
        size_t m_AllocatedBytes;

#if SAVANNA_ENABLE_RUNTIME_MEMORY_VALIDATION
        std::unordered_map<void*, size_t> m_Allocations;
#endif // SAVANNA_ENABLE_RUNTIME_MEMORY_VALIDATION

        AllocationDescriptor GetAllocationDescriptor(void* const ptr) const;
        MemoryChunkDescriptor* FindBestFit(
            const size_t& size, const size_t& alignment, AllocationDescriptor& outAllocationDescriptor, MemoryChunkDescriptor*& pPrevious);

    public:
        FreeListAllocator(size_t size, MemoryLabel label = k_SavannaMemoryLabelHeap);
        FreeListAllocator(void* root, size_t size);
        FreeListAllocator(FreeListAllocator&& other);

        ~FreeListAllocator();

        FreeListAllocator& operator=(FreeListAllocator&& other);

        SAVANNA_NO_DISCARD void* alloc(const size_t& size, const size_t& alignment) SAVANNA_OVERRIDE;
        void free(void* const ptr, const size_t& alignment) SAVANNA_OVERRIDE;
        SAVANNA_NO_DISCARD void* realloc(void* const ptr, const size_t& newSize, const size_t& alignment) SAVANNA_OVERRIDE;

        SAVANNA_NO_DISCARD size_t GetAllocatedBytes() const { return m_AllocatedBytes; }
        SAVANNA_NO_DISCARD size_t GetSize() const { return m_Size; }
    };

    /**
     * @brief Provides a thread safe wrapper around the FreeListAllocator.
     *
    */
    using AtomicFreeListAllocator = AtomicAllocatorWrapper<FreeListAllocator>;

} // namespace Savanna
