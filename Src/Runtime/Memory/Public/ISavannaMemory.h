/**
 * @file SavannaMemory.h
 * @author David Mohrhardt (https://github.com/DavidMohrhardt/Savanna)
 * @brief TODO @David.Mohrhardt Document
 * @version 0.1
 * @date 2023-09-19
 *
 * @copyright Copyright (c) 2023
 *
 */
#ifndef I_SAVANNA_MEMORY_H
#define I_SAVANNA_MEMORY_H

#include "Public/ISavannaEngine.h"

typedef uint16_t se_AllocatorKindBackingInt_t;

typedef enum se_AllocatorKind_t : se_AllocatorKindBackingInt_t
{
    /**
     * @brief The Allocator type indicating the memory should come from the OS.
     * Will use malloc, and free to handle memory.
     *
     */
    k_SavannaAllocatorKindHeap,

    /**
     * @brief The allocator type for memory that is likely to last for the life cycle of
     * Savanna. Typically is implemented using an thread-safe expandable memory pool allocator.
     *
     */
    k_SavannaAllocatorKindPersistent,

    /**
     * @brief The allocator type memory allocations with varyable lifespans.
     * Typically will be allocated using a thread-safe Free List allocator.
     *
     */
    k_SavannaAllocatorKindGeneral,

    /**
     * @brief The allocator type for memory that is extremely short lived (IE. a single frame or less).
     * Implemented as a StackAllocator that is freed by the memory manager at the beginning of each frame.
     *
     */
    k_SavannaAllocatorKindTemp,

    /**
     * @brief The allocator type for memory that is extremely short lived (IE. a single frame or less) but
     * may need some amount of allocation synchronization. Implemented as a StackAllocator that is freed by the memory
     * manager at the beginning of each frame.
     *
     */
    k_SavannaAllocatorKindThreadSafeTemp,

    /**
     * @brief The total number of allocator kinds.
     *
     */
    k_SavannaAllocatorKindCount,

    /**
     * @brief The allocator type indicating there is no backing allocator. Provided to structures
     * that should not own their memory.
     *
     */
    k_SavannaAllocatorKindNone = k_SavannaAllocatorKindCount,
} se_AllocatorKind_t;

/**
 * @brief TODO @David.Mohrhardt Document
 *
 */
typedef void* (*se_pfnAlloc_t)(
    size_t size,
    void* pUserData);

typedef void* (*se_pfnAllocAligned_t)(
    size_t size,
    size_t alignment,
    void* pUserData);

typedef void* (*se_pfnRealloc_t)(
    void* ptr,
    const size_t& __newSize,
    void* pUserData);

typedef void* (*se_pfnReallocAligned_t)(
    void* ptr,
    size_t alignment,
    const size_t& __newSize,
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

#define SAVANNA_INTERFACE_REALLOCATE(__allocatorPtr, __ptr, ____newSize, __userDataPtr) \
    (*(se_AllocatorInterface_t*)(__allocatorPtr)).m_ReallocFunc(__ptr, ____newSize, __userDataPtr)

#define SAVANNA_INTERFACE_REALLOCATE_ALIGNED(__allocatorPtr, __ptr, __alignment, ____newSize, __userDataPtr) \
    (*(se_AllocatorInterface_t*)(__allocatorPtr)).m_ReallocAlignedFunc(__ptr, __alignment, ____newSize, __userDataPtr)

SAVANNA_EXPORT(const se_AllocatorInterface_t) SavannaMemoryGetHeapAllocatorInterface();
SAVANNA_EXPORT(const se_AllocatorInterface_t) SavannaMemoryManagerGetDefaultAllocatorInterface();
SAVANNA_EXPORT(const se_AllocatorInterface_t) SavannaMemoryManagerGetAllocatorInterfaceForAllocatorKind(const se_AllocatorKindBackingInt_t& allocatorKind);
SAVANNA_EXPORT(bool) SavannaMemoryManagerTryGetAllocatorInterfaceForAllocatorKind(const se_uint32& allocatorKind, se_AllocatorInterface_t& outAllocatorKindInterface);

SAVANNA_EXPORT(void*) SavannaMemoryManagerAllocateAligned(
    size_t size,
    size_t alignment,
    se_AllocatorKind_t allocatorKind,
    const char* fileName = nullptr,
    const int lineNo = -1);

SAVANNA_EXPORT(void) SavannaMemoryManagerFree(void* ptr, se_AllocatorKind_t allocatorKind);

SAVANNA_EXPORT(void*) SavannaMemoryManagerReallocateAligned(
    void* ptr,
    size_t size,
    size_t alignment,
    se_AllocatorKind_t allocatorKind,
    const char* fileName = nullptr,
    const int lineNo = -1);

#define SAVANNA_MALLOC(__allocKind, __size) \
    SavannaMemoryManagerAllocateAligned(__size, alignof(se_byte), __allocKind, __FILE__, __LINE__)

#define SAVANNA_MALLOC_ALIGNED(__allocKind, __size, alignment) \
    SavannaMemoryManagerAllocateAligned(__size, alignment, __allocKind, __FILE__, __LINE__)

#define SAVANNA_REALLOC(__allocKind, __ptr, __newSize) \
    SavannaMemoryManagerReallocateAligned(__ptr, __newSize, alignof(se_byte), __allocKind, __FILE__, __LINE__)

#define SAVANNA_REALLOC_ALIGNED(__allocKind, __ptr, __newSize, alignment) \
    SavannaMemoryManagerReallocateAligned(__ptr, __newSize, alignment, __allocKind, __FILE__, __LINE__)

#define SAVANNA_FREE(__allocKind, __ptr) \
    SavannaMemoryManagerFree(__ptr, __allocKind)

#endif // !I_SAVANNA_MEMORY_H
