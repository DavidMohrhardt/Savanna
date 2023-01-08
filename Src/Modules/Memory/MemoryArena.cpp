#include "MemoryArena.h"

namespace Savanna
{
    MemoryArena::~MemoryArena() {}

    void* MemoryArena::Allocate(const size_t &requiredSize, const size_t &alignment)
    {
        return m_upAllocator->alloc(requiredSize, alignment);
    }

    void MemoryArena::Deallocate(void *const ptr, const size_t alignment)
    {
        m_upAllocator->free(ptr, alignment);
    }

} // namespace Savanna
