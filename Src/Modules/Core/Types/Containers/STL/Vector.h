/**
 * @file Vector.h
 * @author David Mohrhardt (https://github.com/DavidMohrhardt/Savanna)
 * @brief
 * @version 0.1
 * @date 2022-08-14
 *
 */
#pragma once

#include <vector>

#include "Types/Memory/Allocators.h"

namespace Savanna
{
    // Specialization for std::vector
    template<typename T>
    using vector = std::vector<T, StlAllocatorAdapter<T, FreeListAllocator>>;

    // Specialization for temporary vectors
    template<typename T>
    using temp_vector = std::vector<T, StlAllocatorAdapter<T, LinearAllocator>>;
} // namespace Savanna
