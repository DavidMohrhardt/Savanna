#pragma once

#include <functional>

#include "Types/SavannaClass.h"

namespace Savanna
{
    template<typename TReturn = void, typename ...Args>
    class Callback
    {
        // DECLARE_SAVANNA_CLASS(Callback);
    public:
        typedef TReturn (*RawCallbackFunctionPtr)(const Callback*, Args...);
        typedef std::function<TReturn(const Callback*, Args...)> CallbackFunc;

        Callback();
        ~Callback();

        void Invoke(Args... arguments);

        void operator+=(CallbackFunc func)
        {
            m_CallbackFuncs.push_back(func);
        }

        void operator-=(CallbackFunc func)
        {
            if (m_CallbackFuncs.empty())
            {
                return;
            }

            CallbackFunc lastFunc = m_CallbackFuncs.end();
            for (int i = 0; i < m_CallbackFuncs.size(); ++i)
            {
                const auto& function = m_CallbackFuncs[i];
                if (func == function)
                {
                    for (; i < m_CallbackFuncs.size() - 1; ++i)
                    {
                        m_CallbackFuncs[i] = m_CallbackFuncs[i + 1];
                    }
                    m_CallbackFuncs.resize(m_CallbackFuncs.size() - 1);
                    break;
                }
            }
        }

        void operator+=(RawCallbackFunctionPtr funcPtr)
        {
            CallbackFunc asFunction = CallbackFunc(funcPtr);
            this += asFunction;
        }

        void operator-=(RawCallbackFunctionPtr funcPtr)
        {
            CallbackFunc asFunction = CallbackFunc(funcPtr);
            this -= asFunction;
        }

    private:
        std::vector<CallbackFunc> m_CallbackFuncs;
    };
} // namespace Savanna
