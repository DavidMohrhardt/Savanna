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

    MemoryManager::MemoryManager(const size_t &initialSize)
        : m_CoreAllocator()
    {
        SAVANNA_MEMORY_SAFETY_ASSERT(initialSize > sizeof(Allocator) * k_MaxArenaCount, "MemoryManager: Not enough memory to allocate arena allocators.");
        SAVANNA_MEMORY_SAFETY_ASSERT(IsPowerOfTwo(initialSize), "MemoryManager: Initial memory size must be a power of two.");

        m_pCoreBuffer = malloc(initialSize);
        SAVANNA_MEMORY_SAFETY_ASSERT(m_pCoreBuffer != nullptr, "MemoryManager: Failed to allocate memory.");

        m_CoreAllocator = FreeListAllocator(m_pCoreBuffer, initialSize);

        m_pArenas = m_CoreAllocator.Allocate<MemoryArena>(k_MaxArenaCount);
    }

    MemoryManager::~MemoryManager()
    {
        for (uint8 i = 0; i < m_ArenaCount; ++i)
        {
            m_pArenas[i].~MemoryArena();
        }

        free(m_pCoreBuffer);
    }
} // namespace Savanna
