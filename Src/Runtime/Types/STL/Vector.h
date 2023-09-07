#pragma once

#include "Types/Memory/StlAllocatorAdapter.h"
#include "Types/Memory/Mallocator.h"

#include <type_traits>
#include <vector>

namespace Savanna
{
    template<typename T, class TAlloc>
    requires std::is_base_of_v<IAllocator, TAlloc>
    using vector = std::vector<T, StlAllocatorAdapter<T, TAlloc>>;

    template <typename T>
    using heap_vector = vector<T, Mallocator>;
} // namespace Savanna
