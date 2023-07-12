/**
 * @file StlAllocatorAdapter.h
 * @author David Mohrhardt (https://github.com/DavidMohrhardt/Savanna)
 * @brief
 * @version 0.1
 * @date 2022-08-15
 *
 * @reference https://indiegamedev.net/2022/03/27/custom-c20-memory-allocators-for-stl-containers/
 *
 */

#pragma once

#include "Allocator.h"

#include <concepts>
#include <type_traits>

namespace Savanna
{
    template<typename T, typename TAlloc>
    requires std::is_base_of_v<TAlloc, Allocator>
    class StlAllocatorAdapter
    {
    public:
        using allocator_type = TAlloc;
        using value_type = T;

    private:
        allocator_type m_Allocator;

    public:
        StlAllocatorAdapter() = delete;

        StlAllocatorAdapter(allocator_type& allocator)
            : m_Allocator(other.m_Allocator)
        {}

        StlAllocatorAdapter(StlAllocatorAdapter& other)
            : m_Allocator(other.m_Allocator)
        {}

        ~StlAllocatorAdapter() {}

        SAVANNA_NO_DISCARD constexpr value_type* allocate(size_t n)
        {
            size_t size = n * sizeof(value_type);
            size_t alignment = alignof(value_type);
            return reinterpret_cast<T*>(m_Allocator.alloc(size, alignment));
        }

        constexpr void deallocate(value_type* p, size_t n) SAVANNA_NOEXCEPT
        {
            m_Allocator.Free(p, alignof(T));
        }

        size_t MaxAllocationSize() const SAVANNA_NOEXCEPT
        {
            return m_Allocator.GetSize();
        }

        bool operator==(const StlAllocatorAdapter<value_type, allocator_type>& rhs) const SAVANNA_NOEXCEPT
        {
            return m_Allocator == rhs.m_Allocator;
        }

        bool operator!=(const StlAllocatorAdapter<value_type, allocator_type>& rhs) const SAVANNA_NOEXCEPT
        {
            return !(*this == rhs);
        }
    };
} // namespace Savanna
