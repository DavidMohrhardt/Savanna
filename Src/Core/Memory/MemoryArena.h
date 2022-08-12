#pragma once

#include "MemoryBlocks.h"

namespace Savanna
{
    class MemoryArena
    {
    private:
        /* data */
        size_t m_Size;
        size_t m_Allocated;
        void* m_Root;
        void* m_Head;

    public:
        MemoryArena(size_t initialMemoryRequest);
        ~MemoryArena();

        SAVANNA_NO_DISCARD size_t GetSize() const { return m_Size; }
        SAVANNA_NO_DISCARD size_t GetAllocated() const { return m_Allocated; }
        SAVANNA_NO_DISCARD size_t GetMaxSize() const { return m_Size - m_Allocated; }

        SAVANNA_NO_DISCARD void* AcquireMemory(size_t size);
        void ReleaseMemory(void* ptr);
    };

} // namespace Savanna
