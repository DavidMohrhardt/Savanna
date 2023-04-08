/**
 * @file Singleton.h
 * @author David Mohrhardt (https://github.com/DavidMohrhardt/Savanna)
 * @brief
 * @version 0.1
 * @date 2022-08-17
 *
 */
#pragma once

#include <utility>
#include <memory>

namespace Savanna
{
    template <typename T>
    class EmbeddedSingleton
    {
    private:
        inline static T s_Instance;

    public:
        static T& Get()
        {
            return s_Instance;
        }

        static void Register(T& instance)
        {
            s_Instance = instance;
        }

        template <typename... Args>
        static T& Construct(Args&&... args)
        {
            s_Instance = T(std::forward<Args>(args)...);
            return Get();
        }

        static void Destroy()
        {
            s_Instance = T();
        }
    };
} // namespace Savanna
