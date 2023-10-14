/**
 * @file SavannaMemory.h
 * @author David Mohrhardt (https://github.com/DavidMohrhardt/Savanna)
 * @brief
 * @version 0.1
 * @date 2023-09-19
 *
 * @copyright Copyright (c) 2023
 *
 */
#ifndef I_SAVANNA_MEMORY_H
#define I_SAVANNA_MEMORY_H

#include "Public/ISavannaEngine.h"

typedef void* (*se_pfnAlloc_t)(
    size_t size,
    void* pUserData);

typedef void* (*se_pfnAllocAligned_t)(
    size_t size,
    size_t alignment,
    void* pUserData);

typedef void* (*se_pfnRealloc_t)(
    void* ptr,
    const size_t& newSize,
    void* pUserData);

typedef void* (*se_pfnReallocAligned_t)(
    void* ptr,
    size_t alignment,
    const size_t& newSize,
    void* pUserData);

typedef void (*se_pfnFree_t)(
    void* ptr,
    void* pUserData);

typedef struct se_AllocatorInterface_t
{
    se_pfnAlloc_t m_AllocFunc;
    se_pfnAllocAligned_t m_AllocAlignedFunc;
    se_pfnRealloc_t m_ReallocFunc;
    se_pfnReallocAligned_t m_ReallocAlignedFunc;
    se_pfnFree_t m_FreeFunc;
} se_AllocatorInterface_t;

/**
 * @brief Provides a helper macro for allocating memory using an allocator interface pointer
 * to allocate using m_AllocFunc.
 * @param __allocatorPtr The allocator interface pointer to use for allocation.
 * @param __size The size of the allocation.
 * @param __userDataPtr The user data pointer to pass to the allocator interface.
 */
#define SAVANNA_INTERFACE_ALLOCATE(__allocatorPtr, __size, __userDataPtr) \
    (*(se_AllocatorInterface_t*)(__allocatorPtr)).m_AllocFunc(__size, __userDataPtr)

/**
 * @brief Provides a helper macro for allocating memory using an allocator interface pointer
 * to allocate using m_AllocAlignedFunc.
 * @param __allocatorPtr The allocator interface pointer to use for allocation.
 * @param __size The size of the allocation.
 * @param __alignment The alignment of the allocation.
 * @param __userDataPtr The user data pointer to pass to the allocator interface.
 */
#define SAVANNA_INTERFACE_ALLOCATE_ALIGNED(__allocatorPtr, __size, __alignment, __userDataPtr) \
    (*(se_AllocatorInterface_t*)(__allocatorPtr)).m_AllocAlignedFunc(__size, __alignment, __userDataPtr)

#define SAVANNA_INTERFACE_FREE(__allocatorPtr, __ptr, __userDataPtr) \
    (*(se_AllocatorInterface_t*)(__allocatorPtr)).m_FreeFunc(__ptr, __userDataPtr)

#define SAVANNA_INTERFACE_REALLOCATE(__allocatorPtr, __ptr, __newSize, __userDataPtr) \
    (*(se_AllocatorInterface_t*)(__allocatorPtr)).m_ReallocFunc(__ptr, __newSize, __userDataPtr)

#define SAVANNA_INTERFACE_REALLOCATE_ALIGNED(__allocatorPtr, __ptr, __alignment, __newSize, __userDataPtr) \
    (*(se_AllocatorInterface_t*)(__allocatorPtr)).m_ReallocAlignedFunc(__ptr, __alignment, __newSize, __userDataPtr)

SAVANNA_EXPORT(const se_AllocatorInterface_t) SavannaMemoryGetHeapAllocatorInterface();
SAVANNA_EXPORT(const se_AllocatorInterface_t) SavannaMemoryManagerGetDefaultAllocatorInterface();
SAVANNA_EXPORT(bool) SavannaMemoryManagerTryGetAllocatorInterfaceForLabel(const se_uint32& label, se_AllocatorInterface_t& outLabelInterface);

#endif // !I_SAVANNA_MEMORY_H
