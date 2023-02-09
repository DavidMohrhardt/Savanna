/**
 * @file VirtualMemoryPool.h
 * @author David Mohrhardt (https://github.com/DavidMohrhardt/Savanna)
 * @brief
 * @version 0.1
 * @date 2023-02-03
 *
 * @copyright Copyright (c) 2023
 *
 */
#pragma once

#include <SavannaEngine.h>
#include <Utilities/SavannaCoding.h>

#include <Types/Memory/Allocators.h>
#include <Types/Memory/CacheLine.h>
#include <Types/Memory/MemoryBlocks.h>

#include "MemoryBuffer.h"

namespace Savanna::Memory
{
    class VirtualMemoryPool
    {
    private:
        ExpandableBlockListAllocator m_Allocator;

    public:
        VirtualMemoryPool() = default;
        VirtualMemoryPool(void* memory, size_t size);
        ~VirtualMemoryPool() = default;

    public:
        SAVANNA_NO_DISCARD MemoryBuffer AcquireMemoryBuffer(size_t size, const size_t& alignment);
        void ReleaseMemoryBuffer(MemoryBuffer& memoryBuffer);
    };
} // namespace Savanna::Memory
