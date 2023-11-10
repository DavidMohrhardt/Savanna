/**
 * @file IAllocator.h
 * @author David Mohrhardt (https://github.com/DavidMohrhardt/Savanna)
 * @brief TODO @David.Mohrhardt Document
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
        SAVANNA_NO_DISCARD virtual void* realloc(void* const ptr, const size_t& newSize, const size_t& alignment) = 0;
    };
}
