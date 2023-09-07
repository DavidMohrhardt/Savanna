/**
 * @file EmbeddedSingleton.h
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
    class EmbeddedSingleton
    {
    public:
        static T& Get()
        {
            static T instance = T();
            return instance;
        }
    };
} // namespace Savanna
