/**
 * @file Singleton.h
 * @author David Mohrhardt (https://github.com/DavidMohrhardt/Savanna)
 * @brief
 * @version 0.1
 * @date 2022-08-17
 *
 */
#pragma once

#include "Public/ISavannaEngine.hpp"

#include "Types/Classes/Immovable.h"
#include "Types/Classes/NonCopyable.h"

#include <utility>

#define DECLARE_FRIENDS_FOR_SINGLETON(__className) \
    friend class Singleton<__className>; \
    /*friend class std::shared_ptr<__className>;*/

namespace Savanna
{
    template <typename T>
    class Singleton : public NonCopyable, public Immovable
    {
    private:
        using TYPE = T;
        // inline static std::shared_ptr<TYPE> s_pInstance = nullptr;
        inline static TYPE* s_pInstance = nullptr;

    public:
        static TYPE* Get();

        template <typename... Args>
        static TYPE* Construct(Args&&... args);

        static void Destroy();
    };

    template <typename T> inline
    Singleton<T>::TYPE *Singleton<T>::Get()
    {
        if (s_pInstance == nullptr)
        {
            throw RuntimeErrorException("[Singleton] Not yet constructed!");
        }
        return s_pInstance;
    }

    template <typename T>
    template <typename... Args>
    inline Singleton<T>::TYPE* Singleton<T>::Construct(Args &&...args)
    {
        if (s_pInstance == nullptr)
        {
            // Singletons are one time constructs, so we can use new here
            // over SAVANNA_NEW or an se_AllocatorInterface_t.
            s_pInstance = ::new TYPE(std::forward<Args>(args)...);
        }
        return Get();
    }

    template <typename T>
    inline void Singleton<T>::Destroy()
    {
        if (s_pInstance != nullptr)
        {
            ::delete s_pInstance;
        }
    }

} // namespace Savanna
