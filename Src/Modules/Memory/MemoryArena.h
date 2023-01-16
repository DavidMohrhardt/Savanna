/**
 * @file MemoryArena.h
 * @author David Mohrhardt (https://github.com/DavidMohrhardt/Savanna)
 * @brief TODO @DavidMohrhardt Document
 * @version 0.1
 * @date 2022-08-02
 *
 */

#pragma once

#include <Utilities/SavannaCoding.h>

#include "Handle.h"

// #include <Types/ManagedObject.h>
#include <Types/Memory/Allocators.h>

#if defined(__cplusplus)

#define SAVANNA_ALLOW_MEMORY_EXPANSION 1

#include <memory>
#include <vector>

namespace Savanna
{
    class MemoryArena
    {
    private:
#if SAVANNA_ALLOW_MEMORY_EXPANSION
        // How can we expand the memory of the arena if we go over budget
        std::vector<std::unique_ptr<void*>> m_Buffers;
#else
        std::unique_ptr<void*> m_Root;
#endif // SAVANNA_ALLOW_MEMORY_EXPANSION
        size_t m_Size;
        uint8 m_ArenaId;
        bool m_MemoryOwner;

    public:
        MemoryArena(void* pBuffer, const size_t &size, const uint8 &arenaId, bool ownsMemory)

#if SAVANNA_ALLOW_MEMORY_EXPANSION
            : m_Buffers(2)
#else
            : m_Root(std::make_unique<T>(pBuffer))
#endif // SAVANNA_ALLOW_MEMORY_EXPANSION
            , m_Size(size)
            , m_ArenaId(arenaId)
            , m_MemoryOwner(ownsMemory)
        {
            m_Buffers[0] = std::make_unique<void*>(pBuffer);
        }

        MemoryArena(const MemoryArena &other) = delete;
        MemoryArena(MemoryArena &&other) = delete;

        ~MemoryArena();

    public:
        MemoryArena& operator=(const MemoryArena &other) = delete;
        MemoryArena& operator=(MemoryArena &&other) = delete;

    public:
        void* Allocate(const size_t &requiredSize, const size_t &alignment);
        void Deallocate(void *const ptr, const size_t alignment);

        inline const uint8 GetArenaId() const
        {
            return m_ArenaId;
        }
    };
}

#endif // defined(__cplusplus)
