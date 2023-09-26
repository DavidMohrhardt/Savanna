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

#include "Utilities/PreprocessorDefinitions/InterfaceDefinitions.h"

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

SAVANNA_EXPORT(const se_AllocatorInterface_t) SavannaMemoryGetHeapAllocatorInterface();
SAVANNA_EXPORT(const se_AllocatorInterface_t) SavannaMemoryManagerGetDefaultAllocatorInterface();

#endif // !I_SAVANNA_MEMORY_H
