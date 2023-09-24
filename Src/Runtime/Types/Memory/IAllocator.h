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

        template <typename T>
        SAVANNA_NO_DISCARD T* New()
        {
            return new (AllocateAs<T>()) T();
        }
    };
}
