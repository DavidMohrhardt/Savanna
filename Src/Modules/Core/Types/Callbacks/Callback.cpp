#include "Callback.h"

namespace Savanna
{
    template<typename TReturn, typename ...Args>
    Callback<TReturn, Args...>::Callback()
        : m_CallbackFuncs()
    {}

    template<typename TReturn, typename ...Args>
    Callback<TReturn, Args...>::~Callback()
    {}

    template<typename TReturn, typename ...Args>
    void Callback<TReturn, Args...>::Invoke(Args... arguments)
    {
        SAVANNA_INSERT_SCOPED_PROFILER("Callback::Invoke");
        for (const auto& function : m_CallbackFuncs)
        {
            if (function != nullptr)
            {
                function(&this, arguments);
            }
        }
    }
} // namespace Savanna
