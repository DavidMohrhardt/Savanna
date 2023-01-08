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

#include "Utilities/SavannaCoding.h"
#include "Types/Singleton/Singleton.h"

namespace Savanna
{
    class MemoryManager : public Singleton<MemoryManager>
    {
    private:
        static constexpr uint8 k_MaxArenaCount = 16;

    private:
        FreeListAllocator m_CoreAllocator;
        void* m_pCoreBuffer;
        MemoryArena* m_pArenas;

        uint8 m_ArenaCount;

    public:
        MemoryManager(const size_t &initialSize);
        ~MemoryManager();
    };
} // namespace Savanna
