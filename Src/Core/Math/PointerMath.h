/**
 * @file PointerMath.h
 * @author David Mohrhardt (https://github.com/DavidMohrhardt/Savanna)
 * @brief
 * @version 0.1
 * @date 2022-08-12
 *
 */

#pragma once

#include "Types/Primitive/PrimitiveTypes.h"
#include "Utilities/SavannaCoding.h"

inline size_t savanna_get_pointer_difference(const void* lhs, const void* rhs)
{
    return ((s_uintptr)lhs) > ((s_uintptr)rhs)
        ? (size_t)((s_uintptr)lhs - (s_uintptr)rhs)
        : (size_t)((s_uintptr)rhs - (s_uintptr)lhs);
}

inline size_t savanna_ptr_add(const void* const ptr, const size_t& byteOffset)
{
    return (size_t)((s_uintptr)ptr + byteOffset);
}

inline size_t savanna_ptr_sub(const void* const ptr, const size_t& byteOffset)
{
    return (size_t)((s_uintptr)ptr - byteOffset);
}

#if __cplusplus

namespace Savanna
{
    inline size_t GetPointerDifference(const void* const lhs, const void* const rhs)
    {
        return reinterpret_cast<uintptr>(lhs) > reinterpret_cast<uintptr>(rhs)
            ? static_cast<size_t>(reinterpret_cast<uintptr>(lhs) - reinterpret_cast<uintptr>(rhs))
            : static_cast<size_t>(reinterpret_cast<uintptr>(rhs) - reinterpret_cast<uintptr>(lhs));
    }

    template<typename T, typename ...size_t>
    inline T* Add(const T* const ptr, size_t... args)
    {
        return reinterpret_cast<T*>(reinterpret_cast<uintptr>(ptr) + (args + ...));
    }

    template<typename T, typename ...size_t>
    inline T* Subtract(const T* const ptr, size_t... args)
    {
        return reinterpret_cast<T*>(reinterpret_cast<uintptr>(ptr) - (args - ...));
    }
} // namespace Savanna

#endif
