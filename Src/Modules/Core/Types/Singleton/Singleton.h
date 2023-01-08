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
    class Singleton
    {
    private:
        inline static std::shared_ptr<T> s_pInstance = std::shared_ptr<T>(nullptr);

    public:
        static void Register(std::shared_ptr<T> spInstance)
        {
            if (s_pInstance == nullptr)
            {
                s_pInstance = spInstance;
            }
        }

        template <typename... Args>
        static T* Construct(Args&&... args)
        {
            if (s_pInstance == nullptr)
            {
                s_pInstance = std::shared_ptr<T>(new T(std::forward<Args>(args)...));
            }
            return s_pInstance.get();
        }

        static std::shared_ptr<T> Get()
        {
            if (s_pInstance == nullptr)
            {
                throw RuntimeErrorException("Singleton not constructed!");
            }
            return s_pInstance;
        }

        static void Destroy()
        {
            if (s_pInstance != nullptr)
            {
                s_pInstance = nullptr;
            }
        }
    };
} // namespace Savanna
