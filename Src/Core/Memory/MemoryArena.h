#pragma once

#include "MemoryBlocks.h"
#include "Allocators/Mallocator.h"

namespace Savanna
{
    class MemoryArena
    {
    private:
        /* data */
        size_t m_Size;
        byte* m_Root;


    public:
        MemoryArena(size_t initialMemoryRequest);
        ~MemoryArena();

        SAVANNA_NO_DISCARD void* AcquireMemory(size_t size);
        void ReleaseMemory(void* ptr, size_t size);
    };

} // namespace Savanna
