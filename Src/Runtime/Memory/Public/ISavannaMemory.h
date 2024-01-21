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

typedef uint16_t seAllocatorKindBackingInt;

typedef enum seAllocatorKind : seAllocatorKindBackingInt
{
    /**
     * @brief The Allocator type indicating the memory should come from the OS.
     * Will use malloc, and free to handle memory.
     *
     */
    kSavannaAllocatorKindHeap,

    /**
     * @brief The allocator type for memory that is likely to last for the life cycle of
     * Savanna. Typically is implemented using an thread-safe expandable memory pool allocator.
     *
     */
    kSavannaAllocatorKindPersistent,

    /**
     * @brief The allocator type memory allocations with varyable lifespans.
     * Typically will be allocated using a thread-safe Free List allocator.
     *
     */
    kSavannaAllocatorKindGeneral,

    /**
     * @brief The allocator type for memory that is extremely short lived (IE. a single frame or less).
     * Implemented as a StackAllocator that is freed by the memory manager at the beginning of each frame.
     *
     */
    kSavannaAllocatorKindTemp,

    /**
     * @brief The allocator type for memory that is extremely short lived (IE. a single frame or less) but
     * may need some amount of allocation synchronization. Implemented as a StackAllocator that is freed by the memory
     * manager at the beginning of each frame.
     *
     */
    kSavannaAllocatorKindThreadSafeTemp,

    /**
     * @brief The total number of allocator kinds.
     *
     */
    kSavannaAllocatorKindCount,

    /**
     * @brief The allocator type indicating there is no backing allocator. Provided to structures
     * that should not own their memory.
     *
     */
    kSavannaAllocatorKindNone = kSavannaAllocatorKindCount,
} seAllocatorKind;

/**
 * @brief TODO @David.Mohrhardt Document
 *
 */
typedef void* (*sepfnAlloc)(
    size_t size,
    void* pUserData);

/**
 * @brief TODO @David.Mohrhardt Document
 *
 */
typedef void* (*sepfnAllocAligned)(
    size_t size,
    size_t alignment,
    void* pUserData);

/**
 * @brief TODO @David.Mohrhardt Document
 *
 */
typedef void* (*sepfnRealloc)(
    void* ptr,
    const size_t& __newSize,
    void* pUserData);

/**
 * @brief TODO @David.Mohrhardt Document
 *
 */
typedef void* (*sepfnReallocAligned)(
    void* ptr,
    size_t alignment,
    const size_t& __newSize,
    void* pUserData);

/**
 * @brief TODO @David.Mohrhardt Document
 *
 */
typedef void (*sepfnFree)(
    void* ptr,
    void* pUserData);

/**
 * @brief Defines an interface for an allocator.
 *
 */
typedef struct seAllocatorInterface
{
    sepfnAlloc m_AllocFunc;
    sepfnAllocAligned m_AllocAlignedFunc;
    sepfnRealloc m_ReallocFunc;
    sepfnReallocAligned m_ReallocAlignedFunc;
    sepfnFree m_FreeFunc;
} seAllocatorInterface;

/**
 * @brief Provides a helper macro for allocating memory using an allocator interface pointer
 * to allocate using m_AllocFunc.
 * @param __allocatorPtr The allocator interface pointer to use for allocation.
 * @param __size The size of the allocation.
 * @param __userDataPtr The user data pointer to pass to the allocator interface.
 */
#define SAVANNA_INTERFACE_ALLOCATE(__allocatorPtr, __size, __userDataPtr) \
    (*(seAllocatorInterface*)(__allocatorPtr)).m_AllocFunc(__size, __userDataPtr)

/**
 * @brief Provides a helper macro for allocating memory using an allocator interface pointer
 * to allocate using m_AllocAlignedFunc.
 * @param __allocatorPtr The allocator interface pointer to use for allocation.
 * @param __size The size of the allocation.
 * @param __alignment The alignment of the allocation.
 * @param __userDataPtr The user data pointer to pass to the allocator interface.
 */
#define SAVANNA_INTERFACE_ALLOCATE_ALIGNED(__allocatorPtr, __size, __alignment, __userDataPtr) \
    (*(seAllocatorInterface*)(__allocatorPtr)).m_AllocAlignedFunc(__size, __alignment, __userDataPtr)

/**
 * @brief TODO @David.Mohrhardt Document
 *
 */
#define SAVANNA_INTERFACE_FREE(__allocatorPtr, __ptr, __userDataPtr) \
    (*(seAllocatorInterface*)(__allocatorPtr)).m_FreeFunc(__ptr, __userDataPtr)

/**
 * @brief TODO @David.Mohrhardt Document
 *
 */
#define SAVANNA_INTERFACE_REALLOCATE(__allocatorPtr, __ptr, ____newSize, __userDataPtr) \
    (*(seAllocatorInterface*)(__allocatorPtr)).m_ReallocFunc(__ptr, ____newSize, __userDataPtr)

/**
 * @brief TODO @David.Mohrhardt Document
 *
 */
#define SAVANNA_INTERFACE_REALLOCATE_ALIGNED(__allocatorPtr, __ptr, __alignment, ____newSize, __userDataPtr) \
    (*(seAllocatorInterface*)(__allocatorPtr)).m_ReallocAlignedFunc(__ptr, __alignment, ____newSize, __userDataPtr)

/**
 * @brief Gets the heap allocator from Savanna's Memory Manager.
 * @note This allocator is always valid regardless of SavannaStart
 * or SavannaStop.
 *
 */
SAVANNA_EXPORT(const seAllocatorInterface) SavannaMemoryManagerGetHeapAllocatorInterface();

/**
 * @brief Gets the default allocator from Savanna's Memory Manager.
 * @note Corresponds to the allocator kind k_Savanna
 *
 */
SAVANNA_EXPORT(const seAllocatorInterface) SavannaMemoryManagerGetDefaultAllocatorInterface();

/**
 * @brief TODO @David.Mohrhardt Document
 *
 */
SAVANNA_EXPORT(const seAllocatorInterface) SavannaMemoryManagerGetAllocatorInterfaceForAllocatorKind(const seAllocatorKindBackingInt& allocatorKind);

/**
 * @brief TODO @David.Mohrhardt Document
 *
 */
SAVANNA_EXPORT(bool) SavannaMemoryManagerTryGetAllocatorInterfaceForAllocatorKind(const se_uint32& allocatorKind, seAllocatorInterface& outAllocatorKindInterface);

/**
 * @brief Provides the interface for memory management.
 *
 * @note The usage of this struct is to allow external
 * plugins to provide their own memory managers to Savanna
 * subsystems.
 *
 */
typedef struct seMemoryManagerInterface
{
    // TODO @David.Mohrhardt: Fill out to override the weird allocator interface idea that's used in Gfx. This will allow hot swapping of Management schemes
} seMemoryManagerInterface;

/**
 * @brief TODO @David.Mohrhardt Document
 *
 */
SAVANNA_EXPORT(void*) SavannaMemoryManagerAllocateAligned(
    size_t size,
    size_t alignment,
    seAllocatorKind allocatorKind,
    const char* fileName = nullptr,
    const int lineNo = -1);

/**
 * @brief TODO @David.Mohrhardt Document
 *
 */
SAVANNA_EXPORT(void) SavannaMemoryManagerFree(void* ptr, seAllocatorKind allocatorKind);

/**
 * @brief TODO @David.Mohrhardt Document
 *
 */
SAVANNA_EXPORT(void*) SavannaMemoryManagerReallocateAligned(
    void* ptr,
    size_t size,
    size_t alignment,
    seAllocatorKind allocatorKind,
    const char* fileName = nullptr,
    const int lineNo = -1);

/**
 * @brief TODO @David.Mohrhardt Document
 *
 */
#define SAVANNA_MALLOC(__allocKind, __size) \
    SavannaMemoryManagerAllocateAligned(__size, alignof(se_byte), __allocKind, __FILE__, __LINE__)

/**
 * @brief TODO @David.Mohrhardt Document
 *
 */
#define SAVANNA_MALLOC_ALIGNED(__allocKind, __size, alignment) \
    SavannaMemoryManagerAllocateAligned(__size, alignment, __allocKind, __FILE__, __LINE__)

/**
 * @brief TODO @David.Mohrhardt Document
 *
 */
#define SAVANNA_REALLOC(__allocKind, __ptr, __newSize) \
    SavannaMemoryManagerReallocateAligned(__ptr, __newSize, alignof(se_byte), __allocKind, __FILE__, __LINE__)

/**
 * @brief TODO @David.Mohrhardt Document
 *
 */
#define SAVANNA_REALLOC_ALIGNED(__allocKind, __ptr, __newSize, alignment) \
    SavannaMemoryManagerReallocateAligned(__ptr, __newSize, alignment, __allocKind, __FILE__, __LINE__)

/**
 * @brief TODO @David.Mohrhardt Document
 *
 */
#define SAVANNA_FREE(__allocKind, __ptr) \
    SavannaMemoryManagerFree(__ptr, __allocKind)

#endif // !I_SAVANNA_MEMORY_H
