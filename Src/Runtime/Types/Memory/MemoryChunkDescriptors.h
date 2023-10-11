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

namespace Savanna
{
    struct alignas(8) MemoryChunkDescriptor
    {
        MemoryChunkDescriptor* m_Next;
        uint32 m_Size;
    };

    struct alignas(8) AllocatedChunkDescriptor
    {
        size_t m_Offset;
        uint32 m_Size;
    };

    static_assert(sizeof(MemoryChunkDescriptor) == sizeof(AllocatedChunkDescriptor), "MemoryChunkDescriptor and AllocatedChunkDescriptor must be the same size");
}
