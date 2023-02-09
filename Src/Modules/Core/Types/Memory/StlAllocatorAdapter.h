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
    template<typename T, typename Allocator>
    requires std::is_base_of_v<Allocator, Allocator>
    class StlAllocatorAdapter
    {
    private:
        Allocator m_Allocator;

    public:
        typedef T value_type;

        StlAllocatorAdapter() = delete;

        StlAllocatorAdapter(Allocator& allocator)
            : m_Allocator(other.m_Allocator)
        {}

        StlAllocatorAdapter(StlAllocatorAdapter& other)
            : m_Allocator(other.m_Allocator)
        {}

        ~StlAllocatorAdapter() {}

        SAVANNA_NO_DISCARD constexpr T* allocate(size_t n)
        {
            size_t size = n * sizeof(T);
            size_t alignment = alignof(T);
            return reinterpret_cast<T*>(m_Allocator.alloc(size, alignment));
        }

        constexpr void deallocate(T* p, size_t n) SAVANNA_NOEXCEPT
        {
            m_Allocator.Free(p, alignof(T));
        }

        size_t MaxAllocationSize() const SAVANNA_NOEXCEPT
        {
            return m_Allocator.GetSize();
        }

        bool operator==(const StlAllocatorAdapter<T, Allocator>& rhs) const SAVANNA_NOEXCEPT
        {
            return m_Allocator == rhs.m_Allocator;
        }

        bool operator!=(const StlAllocatorAdapter<T, Allocator>& rhs) const SAVANNA_NOEXCEPT
        {
            return !(*this == rhs);
        }
    };
} // namespace Savanna
