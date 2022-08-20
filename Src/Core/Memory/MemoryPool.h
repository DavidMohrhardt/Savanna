#pragma once

#include "MemoryBlocks.h"
#include "MemoryArena.h"
#include "Allocators/CoreAllocatorWrapper.h"

namespace Savanna
{
    class MemoryManager;

    class MemoryPool
    {
        friend class MemoryManager;

    private:
        inline static size_t k_DefaultAlignment = alignof(intptr_t);

    private:
        MemoryArena m_Arena;
        void* m_Root;
        size_t m_Size;
        size_t m_Allocated;
        CoreAllocatorWrapper m_Allocator;

    public:
        MemoryPool();
        MemoryPool(void* root, const size_t& size, AllocatorType allocatorType);
        MemoryPool(MemoryPool& other) = delete;
        MemoryPool(MemoryPool&& other);

        ~MemoryPool();

        MemoryPool& operator=(MemoryPool& other) = delete;
        MemoryPool& operator=(MemoryPool&& other);

        CoreAllocatorWrapper* GetAllocator() { return &m_Allocator; }

        void* AcquireMemory(size_t& size);
        void ReleaseMemory(void* ptr);

    private:
        void* GetRoot() const { return m_Root; }
    };

} // namespace Savanna
