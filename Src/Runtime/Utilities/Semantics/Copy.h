/**
 * @file Copy.h
 * @author David Mohrhardt (https://github.com/DavidMohrhardt/Savanna)
 * @brief TODO @David.Mohrhardt Document
 * @version 0.1
 * @date 2023-05-16
 *
 * @copyright Copyright (c) 2023
 *
 */
#pragma once

#include <concepts>
#include <type_traits>

namespace Savanna
{
    // POD types are trivially copyable
    template <typename T>
    concept Copyable = std::is_copy_constructible_v<T> && std::is_copy_assignable_v<T>;

    template <typename T>
    requires Copyable<T>
    inline void Copy(T& destination, const T& source)
    {
        destination = source;
    }

    // Array copy
    template <typename T, size_t N, size_t M>
    requires Copyable<T>
    inline void Copy(T (&destination)[N], const T (&source)[M])
    {
        static_assert(N >= M, "Destination array is smaller than source array");
        for (size_t i = 0; i < M; i++)
            destination[i] = source[i];
    }

#   define SAVANNA_COPY(a, b) Savanna::Copy(a, b)

} // namespace Savanna
