#include "MemoryArena.h"

namespace Savanna
{
    MemoryArena::~MemoryArena() {}

    void* MemoryArena::Allocate(const size_t &requiredSize, const size_t &alignment)
    {
        // for (const auto& pAllocator : m_upAllocators)
        // {
        //     void* ptr = pAllocator->Allocate(requiredSize, alignment);
        //     if (ptr != nullptr)
        //     {
        //         return ptr;
        //     }
        // }

        // If we get here, we need to allocate more memory
        // void* pBuffer = MemoryManager::Get()->RegisterArena(m_ArenaSize, m_ArenaAlignment, m_ArenaKind);
        // void* pAdditionalBuffer = MemoryManager::Get()->ExpandArena(, m_ArenaId);
        return nullptr;
    }

    void MemoryArena::Deallocate(void *const ptr, const size_t alignment)
    {
        // m_upAllocator->free(ptr, alignment);
    }

} // namespace Savanna
