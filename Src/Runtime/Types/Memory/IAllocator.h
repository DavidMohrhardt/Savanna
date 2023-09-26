/**
 * @file IAllocator.h
 * @author David Mohrhardt (https://github.com/DavidMohrhardt/Savanna)
 * @brief
 * @version 0.1
 * @date 2022-08-19
 *
 */
#pragma once

#include "Utilities/SavannaCoding.h"

namespace Savanna
{
    struct IAllocator
    {
        virtual ~IAllocator() {}

        SAVANNA_NO_DISCARD virtual void* alloc(const size_t& size, const size_t& alignment) = 0;
        virtual void free(void* const ptr, const size_t& alignment) = 0;

        template<typename T>
        SAVANNA_NO_DISCARD T* AllocateAs(const size_t& count = 1)
        {
            return reinterpret_cast<T*>(alloc(sizeof(T) * count, alignof(T)));
        }

        template <typename T, typename... Args>
        SAVANNA_NO_DISCARD T* New(Args&&... args)
        {
            T* pObject = AllocateAs<T>();
            new (pObject) T(std::forward<Args>(args)...);
            return pObject;
        }

        template <typename T, typename... Args>
        SAVANNA_NO_DISCARD T* NewArray(const size_t& count, Args&&... args)
        {
            if constexpr (std::is_trivially_constructible_v<T>)
            {
                auto pBuffer = AllocateAs<T>(count);
                memset(pBuffer, 0, sizeof(T) * count);
                return pBuffer;
            }

            T* pArray = AllocateAs<T>(count);
            for (size_t i = 0; i < count; i++)
            {
                new (&pArray[i]) T(std::forward<Args>(args)...);
            }
            return pArray;
        }

        template <typename T>
        void Delete(T* pObject)
        {
            if (pObject)
            {
                pObject->~T();
                free(pObject, alignof(T));
            }
        }

        template <typename T>
        void DeleteArray(T* pArray)
        {
            if (pArray)
            {
                for (size_t i = 0; i < sizeof(pArray); i++)
                {
                    pArray[i].~T();
                }
                free(pArray, alignof(T));
            }
        }
    };
}
