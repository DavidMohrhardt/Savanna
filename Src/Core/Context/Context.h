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
        // ListAllocator m_SystemAllocator;

        // unordered_map<HashString, MemoryArena> m_MemoryArenas;
        std::unordered_map<HashString, MemoryArena> m_MemoryArenas;

    public:
        Context();
        Context(const size_t& coreMemorySize);

        ~Context();

        MemoryArena& GetDefaultMemoryArena() { return m_DefaultMemoryArena; }
        MemoryArena& GetMemoryArena(const HashString& name) { return m_MemoryArenas.at(name); }

        bool TryCreateMemoryArena(const HashString& name, size_t size);
    };
} // namespace Savanna
