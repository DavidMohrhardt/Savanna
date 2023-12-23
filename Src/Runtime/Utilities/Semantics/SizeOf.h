#pragma once

#include <type_traits>

namespace savanna
{
    /**
     * @brief Returns the total size of the types passed in.
    */
    template <typename T, typename... Ts>
    struct TotalSizeOfTypes
    {
        static constexpr size_t value = sizeof(T) + TotalSizeOfTypes<Ts...>::value;
    };

    /**
     * @brief Returns the total size of the types passed in.
    */
    template <typename T>
    struct TotalSizeOfTypes<T>
    {
        static constexpr size_t value = sizeof(T);
    };

    /**
     * @brief Returns the total size of the types passed in.
    */
    template <typename T, typename... Ts>
    struct LargestType
    {
        using type = std::conditional_t<(sizeof(T) > sizeof(LargestType<Ts...>::type)), T, typename LargestType<Ts...>::type>;
        static constexpr size_t size = sizeof(type);
        static constexpr size_t alignment = alignof(type);
    };

    /**
     * @brief Returns the total size of the types passed in.
    */
    template <typename T>
    struct LargestType<T>
    {
        using type = T;
        static constexpr size_t size = sizeof(type);
        static constexpr size_t alignment = alignof(type);
    };

    // template <typename ...Ts>
    // constexpr auto largest_type_v = typename LargestType<Ts...>::type;
} // namespace Savanna
