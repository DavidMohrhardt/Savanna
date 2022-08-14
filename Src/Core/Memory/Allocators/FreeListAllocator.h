/**
 * @file FreeListAllocator.h
 * @author David Mohrhardt (david.mohrhardt@gmail.com)
 * @brief TODO @DavidMohrhardt Document
 * @version 0.1
 * @date 2022-08-02
 *
 * @copyright Copyright (c) 2022
 *
 */

#pragma once

#include "AllocatorUtils.h"

#include "Memory/CacheLine.h"

#include "Utilities/SavannaCoding.h"

namespace Savanna
{
    class MemoryArena;

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

    static_assert(sizeof(MemoryChunkHeader) == sizeof(AllocatedChunkDescriptor));

    class FreeListAllocator
    {
    private:
        MemoryArena* m_OwnerMemoryArena;

        void* m_Root;
        MemoryChunkHeader* m_Head;
        size_t m_Size;
        size_t m_AllocatedBytes;
        uint32 m_NumberOfBlockLinks;

    public:
        FreeListAllocator();
        FreeListAllocator(MemoryArena* arena, size_t size);
        FreeListAllocator(void* m_Root, size_t size);
        ~FreeListAllocator();

        SAVANNA_NO_DISCARD void* Allocate(size_t size, const size_t& alignment);
        void Deallocate(void* const ptr, const size_t alignment);

        SAVANNA_NO_DISCARD size_t GetAllocatedBytes() const { return m_AllocatedBytes; };
        SAVANNA_NO_DISCARD size_t GetSize() const { return m_Size; };
    };
} // namespace Savanna
