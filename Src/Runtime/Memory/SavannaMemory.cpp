/**
 * @file SavannaMemory.cpp
 * @author David Mohrhardt (https://github.com/DavidMohrhardt/Savanna)
 * @brief
 * @version 0.1
 * @date 2023-09-25
 *
 * @copyright Copyright (c) 2023
 *
 */
#include "Memory/SavannaMemory.h"
#include "MemoryManager.h"

#include <cstdlib>
#include "SavannaMemory.h"

extern "C"
{
    void* SavannaHeapAllocatorAllocateAligned(
        size_t size,
        size_t alignment,
        void* /*pUserData*/)
    {
        return malloc(size);
    }

    void* SavannaHeapAllocatorAllocate(
        size_t size,
        void* /*pUserData*/)
    {
        return malloc(size);
    }

    void* SavannaHeapAllocatorReallocAligned(
        void* ptr,
        size_t alignment,
        const size_t& newSize,
        void* /*pUserData*/)
    {
        return realloc(ptr, newSize);
    }

    void* SavannaHeapAllocatorRealloc(
        void* ptr,
        const size_t& newSize,
        void* /*pUserData*/)
    {
        return realloc(ptr, newSize);
    }

    void SavannaHeapAllocatorFree(void* ptr, void* /*pUserData*/)
    {
        if (ptr)
        {
            free(ptr);
        }
    }
}

SAVANNA_EXPORT(const se_AllocatorInterface_t) SavannaMemoryGetHeapAllocatorInterface()
{
    return Savanna::k_HeapAllocatorInterface;
}

SAVANNA_EXPORT(const se_AllocatorInterface_t) SavannaMemoryGetDefaultAllocatorInterfaceForLabel(const se_uint32& label)
{
    return Savanna::MemoryManager::GetAllocatorInterfaceForLabel(label);
}

bool Savanna::IsAllocatorInterfaceValid(const se_AllocatorInterface_t &allocatorInterface)
{
    return allocatorInterface.m_AllocAlignedFunc != nullptr
        && allocatorInterface.m_FreeFunc != nullptr
        && allocatorInterface.m_ReallocAlignedFunc != nullptr;
}

bool Savanna::IsAllocatorInterfaceValid(const se_AllocatorInterface_t *pAllocatorInterface)
{
    return pAllocatorInterface != nullptr
        && IsAllocatorInterfaceValid(*pAllocatorInterface);
}
