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

#include <memory>

#if defined(__cplusplus)

namespace Savanna
{
    class MemoryArena
    {
    private:
#if defined(SAVANNA_MEMORY_ALLOW_ARENA_RESIZE)
        // TODO @DavidMohrhardt: Implement arena resizing
        std::vector<Allocator> m_upAllocators;
#else
        std::unique_ptr<Allocator> m_upAllocator;
#endif // SAVANNA_MEMORY_ALLOW_ARENA_RESIZE
        uint8 m_ArenaId;

    public:
        template <typename T>
        requires std::is_base_of_v<Allocator, T>
        MemoryArena(const size_t &arenaSize, const size_t &arenaAlignment)
            : m_upAllocator(nullptr)
            , m_ArenaId(UINT8_MAX)
        {
            void* pBuffer = MemoryManager::Get()->RegisterArena(arenaSize, arenaAlignment, m_ArenaKind);
            if (pBuffer == nullptr)
            {
                return;
            }

            m_upAllocator = std::make_unique<T>(pBuffer, arenaSize, arenaAlignment);
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

        inline const Allocator* GetAllocator() const
        {
            // Get the weak pointer to the allocator
            return reinterpret_cast<Allocator*>(m_upAllocator.get());
        }

        inline const uint8 GetArenaId() const
        {
            return m_ArenaId;
        }
    };
}

#endif // defined(__cplusplus)
