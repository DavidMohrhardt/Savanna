/**
 * @file Context.cpp
 * @author David Mohrhardt (https://github.com/DavidMohrhardt/Savanna)
 * @brief
 * @version 0.1
 * @date 2022-08-16
 *
 * @copyright Copyright (c) 2022
 *
 */

#include "Context/Context.h"

#include "Memory/MemoryArena.h"
#include "Memory/Allocators/CoreStlAllocatorAdaptor.h"
#include "Types/Containers/STL/Vector.h"
#include "Types/Containers/STL/UnorderedMap.h"

namespace Savanna
{
    Context::Context()
        : m_DefaultMemoryArena(MemoryArena(MemoryArena::k_DefaultMemoryArenaSize))
        , m_MemoryArenas(std::unordered_map<HashString, MemoryArena>())
    {
    }

    Context::Context(const size_t& coreMemorySize)
        : m_DefaultMemoryArena(MemoryArena(coreMemorySize))
        , m_MemoryArenas(std::unordered_map<HashString, MemoryArena>())
    {
    }

    Context::~Context() {}

    bool Context::TryCreateMemoryArena(const HashString& name, size_t size)
    {
        if (m_MemoryArenas.find(name) != m_MemoryArenas.end())
        {
            return false;
        }
        else
        {
            m_MemoryArenas.emplace(name, MemoryArena(size));
            return true;
        }
    }
} // namespace Savanna
