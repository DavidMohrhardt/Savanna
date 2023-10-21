#include "VkEngineAllocator.h"

#include <Memory/MemoryManager.h>

using namespace Savanna;

void* savanna_gfx_vk_engine_allocator_alloc(void *pUserData, size_t size, size_t alignment, VkSystemAllocationScope allocationScope)
{
    return MemoryManager::Get()->Allocate(size, alignment, k_SavannaMemoryLabelGfx);
}

void* savanna_gfx_vk_engine_allocator_realloc(void *pUserData, void *pOriginal, size_t size, size_t alignment, VkSystemAllocationScope allocationScope)
{
    if (pOriginal != nullptr)
        MemoryManager::Get()->Free(pOriginal);
    return MemoryManager::Get()->Allocate(size, alignment, k_SavannaMemoryLabelGfx);
}

void savanna_gfx_vk_engine_allocator_free(void *pUserData, void *pMemory)
{
    if (pMemory != nullptr)
        MemoryManager::Get()->Free(pMemory, k_SavannaMemoryLabelGfx);
}
