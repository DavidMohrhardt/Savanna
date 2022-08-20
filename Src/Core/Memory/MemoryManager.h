/**
 * @file MemoryManager.h
 * @author David Mohrhardt (https://github.com/DavidMohrhardt/Savanna)
 * @brief
 * @version 0.1
 * @date 2022-08-18
 *
 */
#pragma once

#include "MemoryArena.h"
#include "MemoryPool.h"

#include "Types/Containers/STL/Vector.h"

namespace Savanna
{
    class MemoryManager
    {
        friend class Context;

    private:
        static constexpr MemoryArena k_DefaultMemoryArena = MemoryArena::Persistent;

    private:
        MemoryPool m_CoreMemoryPool;
        MemoryPool m_MemoryArenas[MemoryArena::EngineArenaCount - 1];

    public:
        MemoryManager(size_t coreMemorySize = sizeof(UnifiedMemoryBlock64KiB));
        ~MemoryManager();

    public:
        const MemoryPool* AcquireMemoryPool(size_t poolSize, MemoryArena arena = k_DefaultMemoryArena);
        void ReleaseMemoryPool(const MemoryPool* pool, MemoryArena arena = k_DefaultMemoryArena);

    private:
        MemoryPool* AcquireMemoryPoolInternal(size_t poolSize, MemoryArena arena);
    };

    MemoryManager::MemoryManager(size_t coreMemorySize)
    {
        size_t totalSize = NextPowerOfTwo(coreMemorySize);
        m_CoreMemoryPool = MemoryPool(
            malloc(totalSize),
            totalSize,
            AllocatorType::k_FreeList);

        size_t arenaSize = coreMemorySize / MemoryArena::EngineArenaCount / 2;

        for (int i = 0; i < MemoryArena::EngineArenaCount; i++)
        {
            MemoryArena arena = static_cast<MemoryArena>(i);
            AllocatorType type;
            switch (arena)
            {
                case MemoryArena::Core:
                    break;
                case MemoryArena::Persistent:
                case MemoryArena::ThreadLocal:
                    type = AllocatorType::k_FreeList;
                    break;
                case MemoryArena::Temporary:
                case MemoryArena::ThreadTemp:
                    type = AllocatorType::k_Linear;
                    break;
                default:
                    type = AllocatorType::k_FreeList;
                    break;
            }
            m_MemoryArenas[i] = MemoryPool(m_CoreMemoryPool.AcquireMemory(arenaSize), arenaSize, type);
        }
    }

    MemoryManager::~MemoryManager()
    {
        for (int i = 0; i < MemoryArena::EngineArenaCount; i++)
        {
            MemoryArena arena = static_cast<MemoryArena>(i);
            m_CoreMemoryPool.ReleaseMemory(m_MemoryArenas[i].GetRoot());
        }

        free(m_CoreMemoryPool.GetRoot());
    }
} // namespace Savanna
