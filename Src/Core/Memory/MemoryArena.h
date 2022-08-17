#pragma once

#include "MemoryBlocks.h"

namespace Savanna
{
    class MemoryArena
    {
    public:
        static constexpr size_t k_DefaultMemoryArenaSize = sizeof(MemoryBlock32KiB);
    private:
        /* data */
        size_t m_Size;
        size_t m_Allocated;
        void* m_Root;
        void* m_Head;

    public:
        MemoryArena(const size_t& size);
        ~MemoryArena();

        SAVANNA_NO_DISCARD size_t GetSize() const { return m_Size; }
        SAVANNA_NO_DISCARD size_t GetAllocated() const { return m_Allocated; }
        SAVANNA_NO_DISCARD size_t GetMaxSize() const { return m_Size - m_Allocated; }

        SAVANNA_NO_DISCARD void* AcquireMemory(size_t size);
        void ReleaseMemory(void* ptr);
    };

} // namespace Savanna
