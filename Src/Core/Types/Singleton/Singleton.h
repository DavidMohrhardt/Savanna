/**
 * @file Singleton.h
 * @author David Mohrhardt (https://github.com/DavidMohrhardt/Savanna)
 * @brief
 * @version 0.1
 * @date 2022-08-17
 *
 */
#pragma once

namespace Savanna
{
    template <typename T>
    class Singleton
    {
    public:
        static T& GetInstance()
        {
            static T instance;
            return instance;
        }
    };

    template <typename T>
    class ContextSingleton
    {
        static T* s_Instance;
        static Context& m_OwnerContextRef;
    public:
        static T* Construct(Context& context)
        {
            if (m_Instance == nullptr)
            {
                // m_Instance = new T(context);
                m_OwnerContextRef = context;
            }
            return m_Instance;
        }

        static T* Get()
        {
            return m_Instance;
        }

        static void Destroy()
        {
            if (m_Instance != nullptr)
            {
                // delete m_Instance;
                m_Instance = nullptr;
            }
        }

    }
} // namespace Savanna
