/**
 * @file CoreStlAllocatorAdaptor.h
 * @author David Mohrhardt (https://github.com/DavidMohrhardt/Savanna)
 * @brief
 * @version 0.1
 * @date 2022-08-15
 *
 * @reference https://indiegamedev.net/2022/03/27/custom-c20-memory-allocators-for-stl-containers/
 *
 */

#pragma once

#include "CoreAllocatorWrapper.h"

namespace Savanna
{
    template<typename T>
    class CoreStlAllocatorAdaptor
    {
    private:
        CoreAllocatorWrapper& m_CoreAllocatorWrapper;

    public:
        typedef T value_type;

        CoreStlAllocatorAdaptor() = delete;

        CoreStlAllocatorAdaptor(CoreAllocatorWrapper& allocator)
            : m_CoreAllocatorWrapper(other.m_CoreAllocatorWrapper)
        {}

        CoreStlAllocatorAdaptor(CoreStlAllocatorAdaptor& other)
            : m_CoreAllocatorWrapper(other.m_CoreAllocatorWrapper)
        {}

        ~CoreStlAllocatorAdaptor() {}

        SAVANNA_NO_DISCARD constexpr T* allocate(size_t n)
        {
            size_t size = n * sizeof(T);
            size_t alignment = alignof(T);
            return reinterpret_cast<T*>(m_CoreAllocatorWrapper.Allocate(size, alignment));
        }

        constexpr void deallocate(T* p, size_t n) SAVANNA_NO_EXCEPT
        {
            m_CoreAllocatorWrapper.Free(p, alignof(T));
        }

        size_t MaxAllocationSize() const SAVANNA_NO_EXCEPT
        {
            return m_CoreAllocatorWrapper.GetSize();
        }

        bool operator==(const CoreStlAllocatorAdaptor<T>& rhs) const SAVANNA_NO_EXCEPT
        {
            return m_CoreAllocatorWrapper == rhs.m_CoreAllocatorWrapper;
        }

        bool operator!=(const CoreStlAllocatorAdaptor<T>& rhs) const SAVANNA_NO_EXCEPT
        {
            return !(*this == rhs);
        }
    };
} // namespace Savanna
