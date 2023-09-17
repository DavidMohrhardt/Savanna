/**
 * @file Singleton.h
 * @author David Mohrhardt (https://github.com/DavidMohrhardt/Savanna)
 * @brief
 * @version 0.1
 * @date 2022-08-17
 *
 */
#pragma once

#include "Utilities/PreprocessorDefinitions/InterfaceDefinitions.h"

#include <utility>

#include "Memory/SmartPtrUtils.h"

#define DECLARE_FRIENDS_FOR_SINGLETON(__className) \
    friend class Singleton<__className>; \
    friend class std::shared_ptr<__className>;

namespace Savanna
{
    template <typename T>
    class Singleton
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
            // Can't use SAVANNA_NEW here due to the possibility of a private friend constructor
            // So instead use new placement syntax to provide a MemoryManager buffer, and then construct the
            // object
            // s_pInstance = CreateManagedSharedPtr(new (MemoryManager::Get().Allocate(sizeof(T), alignof(T))) TYPE(std::forward<Args>(args)...));
            // s_pInstance = SAVANNA_INPLACE_NEW(TYPE, std::forward<Args>(args)...);
            s_pInstance = new TYPE(std::forward<Args>(args)...);
        }
        return Get();
    }

    template <typename T>
    inline void Singleton<T>::Destroy()
    {
        if (s_pInstance != nullptr)
        {
            // SAVANNA_DELETE(s_pInstance);
            delete s_pInstance;
        }
    }

} // namespace Savanna
