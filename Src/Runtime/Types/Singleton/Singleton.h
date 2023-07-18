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

#define DECLARE_SINGLETON_CLASS(__className) \
    friend class Singleton<__className>; \
    friend class std::shared_ptr<__className>;

namespace Savanna
{
    template <typename T>
    class Singleton
    {
    private:
        inline static std::shared_ptr<T> s_pInstance = nullptr;

    public:
        static std::shared_ptr<T> Get()
        {
            if (s_pInstance == nullptr)
            {
                throw RuntimeErrorException("[Singleton] Not yet constructed!");
            }
            return s_pInstance;
        }

        template <typename... Args>
        static std::shared_ptr<T> Construct(Args&&... args)
        {
            if (s_pInstance == nullptr)
            {
                s_pInstance = std::shared_ptr<T>(new T(std::forward<Args>(args)...));
            }
            return Get();
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
