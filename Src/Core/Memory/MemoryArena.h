/**
 * @file AllocatorKind.h
 * @author David Mohrhardt (https://github.com/DavidMohrhardt/Savanna)
 * @brief TODO @DavidMohrhardt Document
 * @version 0.1
 * @date 2022-08-02
 *
 */

#pragma once

#include "Types/Primitive/PrimitiveTypes.h"

#include "Utilities/Macros/CppTypeDefs.h"

/**
 * @brief TODO @DavidMohrhardt Document
 *
 */
enum SavannaMemoryArena : __se_int32
{
    /**
     * @brief Represents the lack of a memory arena. This is the default value and cannot be used to acquire
     * memory.
     */
    None,

    /**
     * @brief Represents Core Engine Memory Arena. Not to be used by users.
     */
    Core,

    /**
     * @brief Represents the Scripting Engine Memory Arena
     */
    Scripting,

    /**
     * @brief Rerpresents a general use long-term Memory Arena
     */
    Persistent,

    /**
     * @brief Value indicates that a temporary allocator is used. Memory allocated via this allocator
     * is only available until the end of a frame.
     */
    Temporary,

    /**
     * @brief Value indicates that a thread-safe temporary allocator will be used. Thread safe temporary
     * allocations are longer lived than typical temporary allocations.
     */
    ThreadTemp,

    /**
     * @brief Value indicates that a thread-specific Memory Arenas will be used. Thread specific Memory
     * Arena will only be accessible via the owner thread. When used on from the main thread, this memory will
     * be allocated from Temporary Memory Arena then copied once a thread has taken ownership.
     */
    ThreadLocal,

    /**
     * @brief Represents a Memory Arena that is unowned by the engine. Allocations using this arena will use
     * malloc and must be freed manually.
     */
    System,
};

/**
 * @brief If compiled by an C++ object, provides a simple "Savanna" Namespaced type definition
 * for the SavannaMemoryArena: Savanna::MemoryArena
 */
DECLARE_SAVANNA_NAMESPACED_CPP_TYPE_DEF(SavannaMemoryArena, MemoryArena);
