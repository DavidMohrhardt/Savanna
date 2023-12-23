#pragma once

#if __cplusplus

#include <concepts>
#include <type_traits>

namespace savanna
{
    /**
     * @brief Moves the contents of b into a and sets b to its default value. Must be a POD type.
     *
     * @tparam T The type to move. Must be a POD type.
     * @param a The destination data.
     * @param b  The source data.
     */
    template <typename T>
    requires std::is_pod_v<T>
    inline constexpr void Move(T& a, T& b)
    {
        a = b;
        b = {0};
    }

    template <typename T>
    requires std::is_pod_v<T>
    inline constexpr void MoveAndAssign(T& a, T& b)
    {
        a = b;
        b = {0};
    }

    /**
     * @brief Calls the move assignment operator on a with b and sets b to its default value. Must be a non-POD type.
     *
     * @tparam T The type to move. Must be a non-POD type.
     * @param a The destination data.
     * @param b The source data.
     */
    template <class T>
    requires std::is_class_v<T> && std::is_move_assignable_v<T> && (!std::is_pod_v<T>)
    inline constexpr void Move(T& a, T& b)
    {
        a = std::move(b);
    }

    template <class T>
    requires std::is_class_v<T> && std::is_move_assignable_v<T> && (!std::is_pod_v<T>)
    inline constexpr void MoveAndAssign(T& a, T& b)
    {
        a = std::move(b);
        b = decltype(b)();
    }

#   define SAVANNA_MOVE(a, b) savanna::Move(a, b)
#   define SAVANNA_MOVE_ASSIGN(a, b) savanna::MoveAndAssign(a, b)

#   define SAVANNA_MOVE_MEMBER(__name, __other) SAVANNA_MOVE(this->__name, __other.__name)
#   define SAVANNA_MOVE_MEMBER_ASSIGN(__name, __other) SAVANNA_MOVE_ASSIGN(this->__name, __other.__name)

} // namespace Savanna

#endif
