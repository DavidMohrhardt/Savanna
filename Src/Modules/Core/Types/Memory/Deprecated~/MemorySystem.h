/**
 * @file MemoryManager.h
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

#include <Types/Singleton/Singleton.h>
#include "MemoryManager.h"

namespace Savanna::Memory
{
    class MemoryManager : public Singleton<MemoryManager>, NonCopyable, Immovable
    {
    private:
        std::list<MemoryBuffer> m_Buffers;
        ListAllocator m_Allocator;
        mutable std::mutex m_CoreMemoryOperationsMutex;

    public:
        MemoryManager() = default;
        ~MemoryManager();

      public:
        void Initialize(size_t size);
        void Shutdown();

        SAVANNA_NO_DISCARD MemoryBuffer AcquireMemoryBuffer(size_t size, const size_t& alignment);
        void ReleaseMemoryBuffer(MemoryBuffer& memoryBuffer);

        SAVANNA_NO_DISCARD size_t GetAllocatedBytes() const { return m_Allocator.GetAllocatedBytes(); };

        SAVANNA_NO_DISCARD size_t GetSize() const { return m_Allocator.GetSize(); };

    private:
        // Memory Optimization operations
        void DefragmentMemory();
        void CombineUnusedBuffers();

        void ReallocateMemoryBuffer(MemoryBuffer& memoryBuffer, size_t size, const size_t& alignment);
    };
} // namespace Savanna::Memory

