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
#include "Types/Memory/CacheLine.h"

#include "Allocator.h"
#include "MemoryChunkDescriptors.h"

#include "AtomicAllocatorWrapper.h"

#include "MemoryBuffer.h"

namespace Savanna
{
    /**
     * @brief
     */
    class FreeListAllocator : public Allocator
    {
    private:
        MemoryLabel m_MemoryLabel;
        MemoryChunkDescriptor* m_Head;
        size_t m_Size;
        size_t m_AllocatedBytes;

    public:
        FreeListAllocator(size_t size, MemoryLabel label = k_SavannaMemoryLabelHeap);
        FreeListAllocator(void* root, size_t size);
        FreeListAllocator(FreeListAllocator&& other);

        ~FreeListAllocator();

    public:
        FreeListAllocator& operator=(FreeListAllocator&& other);

    public:
        SAVANNA_NO_DISCARD void* alloc(const size_t& size, const size_t& alignment) SAVANNA_OVERRIDE;
        void free(void* const ptr, const size_t& alignment) SAVANNA_OVERRIDE;

        SAVANNA_NO_DISCARD size_t GetAllocatedBytes() const { return m_AllocatedBytes; }
        SAVANNA_NO_DISCARD size_t GetSize() const { return m_Size; }
    };

    /**
     * @brief Provides a thread safe wrapper around the FreeListAllocator.
     *
    */
    using AtomicFreeListAllocator = AtomicAllocatorWrapper<FreeListAllocator>;

} // namespace Savanna
