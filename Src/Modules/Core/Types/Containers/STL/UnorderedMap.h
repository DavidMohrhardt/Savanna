/**
 * @file UnorderedMap.h
 * @author David Mohrhardt (https://github.com/DavidMohrhardt/Savanna)
 * @brief
 * @version 0.1
 * @date 2022-08-15
 *
 * @reference https://indiegamedev.net/2022/03/27/custom-c20-memory-allocators-for-stl-containers/
 *
 */

#pragma once

#include <unordered_map>
#include <string>

#include "Memory/Allocators/StlAllocatorAdapter.h"
#include "Types/Strings/FixedString.h"
#include "Types/Strings/CRC32HashString"


namespace Savanna
{
    template<typename Key, typename T>
    using unordered_map = std::unordered_map<
        Key,
        T,
        std::hash<Key>,
        std::equal_to<Key>,
        StlAllocatorAdapter<std::pair<Key const, T>>>;
} // namespace Savanna

namespace std
{
    template<typename Key, typename T>
    struct hash<Savanna::unordered_map<Key, T>>
    {
        size_t operator()(Savanna::unordered_map<Key, T> const& map) const
        {
            size_t hash = 0;
            for (auto const& pair : map)
            {
                hash ^= hash_value(pair.first);
                hash ^= hash_value(pair.second);
            }
            return hash;
        }
    };
} // namespace std
