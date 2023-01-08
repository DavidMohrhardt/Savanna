/**
 * @file EngineArenas.h
 * @author David Mohrhardt (https://github.com/DavidMohrhardt/Savanna)
 * @brief TODO @DavidMohrhardt Document
 * @version 0.1
 * @date 2022-08-02
 *
 */
#pragma once

#include <SavannaEngine.h>
#include <Utilities/SavannaCoding.h>

/**
 * @brief TODO @DavidMohrhardt Document
 *
 */
typedef enum se_MemoryArenaKind_t
{
    /**
     * @brief Represents Core Engine Memory Arena. Not to be used by users.
     */
    kMemoryArenaCore,

    /**
     * @brief Rerpresents a general use long-term Memory Arena
     */
    kMemoryArenaPersistent,

    /**
     * @brief Value indicates that a temporary allocator is used. Memory allocated via this allocator
     * is only available until the end of a frame. TODO @DavidMohrhardt Document additional details.
     */
    kMemoryArenaTemporary,

    /**
     * @brief Value indicates that a thread-safe temporary allocator will be used. Thread safe temporary
     * allocations are longer lived than typical temporary allocations.
     */
    kMemoryArenaThreadTemp,

    /**
     * @brief Value indicates that a thread-specific Memory Arenas will be used. Thread specific Memory
     * Arenas are longer lived than typical temporary allocations.
     */
    kMemoryArenaThreadSpecific,

    /**
     * @brief Represents the Scripting Engine Memory Arena
     */
    // kMemoryArenaScripting,

    /**
     * @brief Provides a count of the number engine specific memory arenas. Does not provide the full count of
     * all memory arenas known to the engine. Query the MemoryManager for the full count.
     */
    kMemoryArenaEngineArenaCount,
} se_MemoryArenaKind_t;
DECLARE_NAMESPACED_ENUM_WRAPPER(Savanna, se_MemoryArenaKind_t, MemoryArenaKind, se_uint8);
