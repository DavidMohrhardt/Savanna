/**
 * @file Context.h
 * @author David Mohrhardt (https://github.com/DavidMohrhardt/Savanna)
 * @brief
 * @version 0.1
 * @date 2022-08-14
 *
 */

#pragma once

#include "Memory/MemoryArena.h"
#include "Memory/Allocators/CoreStlAllocatorAdaptor.h"
#include "Types/Containers/STL/Vector.h"
#include "Types/Containers/STL/UnorderedMap.h"

namespace Savanna
{
    class Context
    {
    private:
        MemoryArena m_DefaultMemoryArena;
        unordered_map<HashString, MemoryArena> m_MemoryArenas;

    public:
        Context();
        Context(size_t coreMemorySize, size_t memoryArenaSize);

        ~Context();

        MemoryArena& GetDefaultMemoryArena() { return m_DefaultMemoryArena; }
        MemoryArena& GetMemoryArena(const HashString& name) { return m_MemoryArenas.at(name); }

        bool TryCreateMemoryArena(const HashString& name, size_t size);
    };
} // namespace Savanna
