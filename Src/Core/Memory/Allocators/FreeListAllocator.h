/**
 * @file FreeListAllocator.h
 * @author David Mohrhardt (https://github.com/DavidMohrhardt/Savanna)
 * @brief TODO @DavidMohrhardt Document
 * @version 0.1
 * @date 2022-08-02
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

        template<typename T>
        SAVANNA_NO_DISCARD T* Allocate(size_t count = 1)
        {
            return reinterpret_cast<T*>(Allocate(sizeof(T) * count, alignof(T)));
        }

        template<typename T>
        void Deallocate(T* const ptr)
        {
            Deallocate(ptr, alignof(T));
        }

        SAVANNA_NO_DISCARD size_t GetAllocatedBytes() const { return m_AllocatedBytes; };
        SAVANNA_NO_DISCARD size_t GetSize() const { return m_Size; };

        bool operator==(const FreeListAllocator& other) const
        {
            return m_Root == other.m_Root && m_Size == other.m_Size && m_Head == other.m_Head && m_AllocatedBytes == other.m_AllocatedBytes && m_NumberOfBlockLinks == other.m_NumberOfBlockLinks;
        }
    };
} // namespace Savanna
