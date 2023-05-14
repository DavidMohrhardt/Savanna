#pragma once

#if __cplusplus

#include <concepts>
#include <type_traits>

namespace Savanna
{
    template <typename T>
    concept MoveAssignable = requires(T a, T b)
    {
        { a = std::move(b) } -> std::same_as<T&>;
    };

    template <typename T>
    concept MoveConstructible = requires(T a, T b)
    {
        { T(std::move(a)) } -> std::same_as<T>;
    };

    template <typename T>
    concept MoveConstructibleAndAssignable = MoveConstructible<T> && MoveAssignable<T>;

    template <typename T>
    requires std::is_pod_v<T>
    inline constexpr void Move(T& a, T& b)
    {
        a = b;
        b = {};
    }

    template <typename T>
    requires std::is_class_v<T> && MoveConstructibleAndAssignable<T>
    inline constexpr void Move(T& a, T& b)
    {
        a = std::move(b);
    }

#   define SAVANNA_MOVE(a, b) Savanna::Move(a, b)
#   define SAVANNA_MOVE_ASSIGN(a, b) a = std::move(b); b = decltype(b)()

#   define SAVANNA_MOVE_MEMBER(__name, __other) SAVANNA_MOVE(this->__name, __other.__name)

} // namespace Savanna

#endif
