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
#include "Memory/Allocators/CoreStlAllocatorAdaptor.h"

namespace Savanna
{
    // Specialization for std::vector
    template<typename T>
    using vector = std::vector<T, CoreStlAllocatorAdaptor<T>>;
} // namespace Savanna
