#include "VkAllocationCallback.h"

#include <Memory/MemoryManager.h>

using namespace Savanna;

void* SavannaGfxVkAllocatorCallbackAlloc(void *pUserData, size_t size, size_t alignment, VkSystemAllocationScope allocationScope)
{
    return MemoryManager::Get().Allocate(size, alignment, SE_MEMORY_LABEL_GRAPHICS);
}

void* SavannaGfxVkAllocatorCallbackRealloc(void *pUserData, void *pOriginal, size_t size, size_t alignment, VkSystemAllocationScope allocationScope)
{
    if (pOriginal != nullptr)
        MemoryManager::Get().Free(pOriginal);
    return MemoryManager::Get().Allocate(size, alignment, SE_MEMORY_LABEL_GRAPHICS);
}

void SavannaGfxVkAllocatorCallbackFree(void *pUserData, void *pMemory)
{
    if (pMemory != nullptr)
        MemoryManager::Get().Free(pMemory);
}
