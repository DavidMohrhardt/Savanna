#pragma once
#include <concept>
#include <type_traits>

namespace Savanna
{
    template <int COUNT>
    class LoopUnroller
    {
    public:
        template <typename TReturn, typename TOperation>
        requires std::is_invocable_v<TOperation, int>
        static inline void Unroll(TOperation& func)
        {
            LoopUnroller<COUNT - 1>::Unroll(func);
            func(COUNT - 1);
        }
    };

    template <>
    class LoopUnroller<0>
    {
    public:
        template <typename TReturn, typename TOperation>
        requires std::is_invocable_v<TOperation, int>
        static inline void Unroll(TOperation& func) {}

        // Specialization for void return type
        template <typename TOperation>
        requires std::is_invocable_v<TOperation, int>
        static inline void Unroll(TOperation& func) {}
    };

#define SAVANNA_UNROLL(COUNT, operation) \
    LoopUnroller<COUNT>::Unroll(operation)
} // namespace Savanna
