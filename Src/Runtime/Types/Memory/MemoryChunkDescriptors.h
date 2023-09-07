/**
 * @file MemoryChunkDescriptors.h
 * @author David Mohrhardt (https://github.com/DavidMohrhardt/Savanna)
 * @brief
 * @version 0.1
 * @date 2023-02-03
 *
 * @copyright Copyright (c) 2023
 *
 */
#pragma once

#include "Utilities/SavannaCoding.h"

#define DEPRECATE_MEMORY_CHUNK_DESCRIPTOR_POINTER 0

namespace Savanna
{
    struct alignas(8) MemoryChunkDescriptor
    {
#if DEPRECATE_MEMORY_CHUNK_DESCRIPTOR_POINTER
        uint32 m_NextOffset;
#else
        // TODO: Instead of a pointer make this a uint32 offset from the start of the chunk
        MemoryChunkDescriptor* m_Next;
#endif // DEPRECATE_MEMORY_CHUNK_DESCRIPTOR_POINTER
        uint32 m_Size;
    };

    struct alignas(8) AllocatedChunkDescriptor
    {
#if DEPRECATE_MEMORY_CHUNK_DESCRIPTOR_POINTER
        uint32 m_Offset;
#else
        // TODO: When MemoryChunkDescriptor is changed to a uint32 offset, change this to a uint32
        size_t m_Offset;
#endif // DEPRECATE_MEMORY_CHUNK_DESCRIPTOR_POINTER
        uint32 m_Size;
    };

    static_assert(sizeof(MemoryChunkDescriptor) == sizeof(AllocatedChunkDescriptor), "MemoryChunkDescriptor and AllocatedChunkDescriptor must be the same size");
}
