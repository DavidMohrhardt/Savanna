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
    /**
     * @brief A virtual class that represents a program Context. Implementations of this class are responsible
     * for defining the required managers for the program. The base context will always contain a MemoryManager
     * and ThreadManaager.
     */
    class Context
    {
    /**
     * When building the context, allow users to register their own memory pools generated from various
     * memory arenas. For example a user could register a long lived Rendering memory pool and a
     * short lived frame memory pool that would use the Persistent and Temporary memory arenas respectively.
    */
    private:
        // MemoryArena m_DefaultMemoryArena;
        // std::unordered_map<HashString, MemoryPool> m_MemoryArenas;
        // MemoryManager m_MemoryManager;
        // ThreadManager m_ThreadManager;

    public:
        Context() = delete;
        Context(const size_t& coreMemorySize);

        virtual ~Context();

        // MemoryArena& GetDefaultMemoryArena() { return m_DefaultMemoryArena; }
        // MemoryManager* GetMemoryManager() { return &m_MemoryManager; }
        // MemoryArena& GetMemoryArena(const HashString& name) { return m_MemoryArenas.at(name); }

        // bool TryCreateMemoryArena(const HashString& name, MemoryArena& memoryArena, size_t size);
    };
} // namespace Savanna
