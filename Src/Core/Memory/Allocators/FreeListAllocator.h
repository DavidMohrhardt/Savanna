/**
 * @file FreeListAllocator.h
 * @author David Mohrhardt (https://github.com/DavidMohrhardt/Savanna)
 * @brief TODO @DavidMohrhardt Document
 * @version 0.1
 * @date 2022-08-02
 *
 */

#pragma once

#include "CoreAllocatorBase.h"
#include "Memory/CacheLine.h"
#include "Utilities/SavannaCoding.h"

namespace Savanna
{
    class MemoryPool;

    typedef struct alignas(8) MemoryChunkHeader
    {
        MemoryChunkHeader* m_Next;
        int32 m_Size;
    } MemoryChunkHeader;

    typedef struct alignas(8) AllocatedChunkDescriptor
    {
        size_t m_Offset;
        int32 m_Size;
    } AllocatedChunkDescriptor;

    static_assert(sizeof(MemoryChunkHeader) == sizeof(AllocatedChunkDescriptor) && "MemoryChunkHeader and AllocatedChunkDescriptor must be the same size");

    /**
     * @brief
     */
    class FreeListAllocator final : public CoreAllocatorBase
    {
    private:
        MemoryChunkHeader* m_Head;

    public:
        FreeListAllocator();
        FreeListAllocator(void* root, size_t size);
        FreeListAllocator(FreeListAllocator& other) = delete;
        FreeListAllocator(FreeListAllocator&& other);

        ~FreeListAllocator();

    public:
        FreeListAllocator& operator=(FreeListAllocator& other) = delete;
        FreeListAllocator& operator=(FreeListAllocator&& other);

    public:
        SAVANNA_NO_DISCARD void* Allocate(const size_t& size, const size_t& alignment) override;
        void Deallocate(void* const ptr, const size_t& alignment) override;
    };
} // namespace Savanna
