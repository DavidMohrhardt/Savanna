/**
 * @file SavannaMemory.h
 * @author David Mohrhardt (https://github.com/DavidMohrhardt/Savanna)
 * @brief
 * @version 0.1
 * @date 2023-09-25
 *
 * @copyright Copyright (c) 2023
 *
 */
#pragma once

#include "Public/ISavannaMemory.h"

extern "C"
{
    void* SavannaHeapAllocatorAllocateAligned(size_t size, size_t alignment, void* pUserData);
    void* SavannaHeapAllocatorAllocate(size_t size, void* pUserData);
    void* SavannaHeapAllocatorReallocAligned(void* ptr, size_t alignment, const size_t& newSize, void* pUserData);
    void* SavannaHeapAllocatorRealloc(void* ptr, const size_t& newSize, void* pUserData);
    void SavannaHeapAllocatorFree(void* ptr, void* pUserData);
}

namespace Savanna
{
    constexpr se_AllocatorInterface_t k_HeapAllocatorInterface
    {
        .m_AllocFunc            = SavannaHeapAllocatorAllocate,
        .m_AllocAlignedFunc     = SavannaHeapAllocatorAllocateAligned,
        .m_ReallocFunc          = SavannaHeapAllocatorRealloc,
        .m_ReallocAlignedFunc   = SavannaHeapAllocatorReallocAligned,
        .m_FreeFunc             = SavannaHeapAllocatorFree
    };

    bool IsAllocatorInterfaceValid(const se_AllocatorInterface_t& allocatorInterface);
    bool IsAllocatorInterfaceValid(const se_AllocatorInterface_t* pAllocatorInterface);
} // namespace Savanna::Memory

#define SAVANNA_ASSERT_VALID_ALLOCATOR_INTERFACE(allocatorInterface) \
    SAVANNA_ASSERT(Savanna::IsAllocatorInterfaceValid(allocatorInterface), "Invalid allocator interface!")

#if __cplusplus

#define SAVANNA_NEW(label, type, ...) \
    Savanna::MemoryManager::Get()->New<type>(label, __VA_ARGS__)

#define SAVANNA_NEW_ARRAY(label, type, count) \
    Savanna::MemoryManager::Get()->NewArray<type>(count, label)

#define SAVANNA_DELETE(label, ptr) \
    Savanna::MemoryManager::Get()->Delete(ptr, label)

#define SAVANNA_DELETE_ARRAY(label, ptr, count) \
    Savanna::MemoryManager::Get()->DeleteArray(ptr, count, label)

#define SAVANNA_INPLACE_NEW(label, type, ptr, ...) \
    new (ptr) type(__VA_ARGS__)

#define SAVANNA_INPLACE_NEW_ARRAY(label, type, count, ptr) \
    new (ptr) type[count]

#define SAVANNA_MALLOC(label, size) \
    Savanna::MemoryManager::Get()->Allocate(size, label)

#define SAVANNA_MALLOC_ALIGNED(label, size, alignment) \
    Savanna::MemoryManager::Get()->AllocateAligned(size, alignment, label)

#define SAVANNA_REALLOC(label, ptr, newSize) \
    Savanna::MemoryManager::Get()->Reallocate(ptr, newSize, label)

#define SAVANNA_REALLOC_ALIGNED(label, ptr, newSize, alignment) \
    Savanna::MemoryManager::Get()->ReallocateAligned(ptr, newSize, alignment, label)

#define SAVANNA_FREE(label, ptr) \
    Savanna::MemoryManager::Get()->Free(ptr, label)

#define DECLARE_SAVANNA_MEMORY_CLASS_FRIENDS() \
    friend class Savanna::MemoryManager; \
    friend class Savanna::Allocator; \

#endif // __cplusplus
