/**
 * @file MemoryManager.cpp
 * @author David Mohrhardt (https://github.com/DavidMohrhardt/Savanna)
 * @brief
 * @version 0.1
 * @date 2023-01-08
 *
 * @copyright Copyright (c) 2023
 *
 */
#include "MemoryManager.h"

#include <Utilities/Assertions/SavannaAssertions.h>

#include <mutex>

namespace Savanna
{
    static std::mutex s_RootMutex;

    constexpr uint8 k_CoreArenaId = MemoryManager::k_MaxArenaCount + 1;

    MemoryManager::MemoryManager(const size_t &initialSize)
        : m_RootArena(malloc(initialSize), initialSize, k_CoreArenaId, true)
        , m_RegisteredArenas()
        , m_ArenaCount(0)
    {
        SAVANNA_MEMORY_SAFETY_ASSERT(initialSize > sizeof(Allocator) * k_MaxArenaCount, "MemoryManager: Not enough memory to allocate arena allocators.");
        SAVANNA_MEMORY_SAFETY_ASSERT(IsPowerOfTwo(initialSize), "MemoryManager: Initial memory size must be a power of two.");
    }

    MemoryManager::~MemoryManager()
    {
        for (uint8 i = 0; i < m_ArenaCount; ++i)
        {
            // Call the destructors manually since we own the memory.
            m_RegisteredArenas[i]->~MemoryArena();
        }
    }
} // namespace Savanna
