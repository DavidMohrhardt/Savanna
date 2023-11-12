/**
 * @file Allocator.h
 * @author David Mohrhardt (https://github.com/DavidMohrhardt/Savanna)
 * @brief TODO @David.Mohrhardt Document
 * @version 0.1
 * @date 2022-08-19
 *
 */
#pragma once

#include <Utilities/SavannaCoding.h>

#include "AllocatorUtils.h"
#include "IAllocator.h"

namespace Savanna
{
    class Allocator : public IAllocator, NonCopyable
    {
    public:
        Allocator() = default;
        virtual ~Allocator() {}

        template<typename T>
        SAVANNA_NO_DISCARD T* AllocateAs(const size_t& count = 1)
        {
            constexpr size_t alignment = alignof(T);
            constexpr size_t size = sizeof(T);
            return reinterpret_cast<T*>(alloc(size * count, alignment));
        }

        template <typename T, typename... Args>
        SAVANNA_NO_DISCARD T* New(Args&&... args)
        {
            return new (AllocateAs<T>()) T(std::forward<Args>(args)...);
        }

        template <typename T, typename... Args>
        SAVANNA_NO_DISCARD T* NewArray(const size_t& count, Args&&... args)
        {
            if constexpr (std::is_trivially_constructible_v<T>)
            {
                auto pBuffer = AllocateAs<T>(count);
                if (!pBuffer)
                {
                    throw std::bad_alloc();
                }

                memset(pBuffer, 0, sizeof(T) * count);
                return pBuffer;
            }

            T* pArray = AllocateAs<T>(count);
            if (!pArray)
            {
                throw std::bad_alloc();
            }

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
} // namespace Savanna
