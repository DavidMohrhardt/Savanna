/**
 * @file Mallocator.h
 * @author David Mohrhardt (https://github.com/DavidMohrhardt/Savanna)
 * @brief TODO @DavidMohrhardt Document
 * @version 0.1
 * @date 2022-08-02
 *
 */

#pragma once

#include "Utilities/SavannaCoding.h"

#include "Allocator.h"
#include "Memory/SystemMemoryUtils.h"

#include <cstdlib>

namespace Savanna
{
    /**
     * @brief Extended from https://docs.microsoft.com/en-us/cpp/standard-library/allocators?view=msvc-170.
     * Implements a custom allocator for accessing system heap memory. This is thread safe.
     */
    struct Mallocator
        : public Allocator
    {
        Mallocator() = default;

        SAVANNA_NO_DISCARD void* alloc(const size_t& size, const size_t& alignment) SAVANNA_OVERRIDE { return std::malloc(size); }
        void free(void* const ptr, const size_t& alignment) SAVANNA_OVERRIDE { std::free(ptr); }
        SAVANNA_NO_DISCARD virtual size_t GetAllocatedBytes() const { return 0; };
        SAVANNA_NO_DISCARD virtual size_t GetSize() const
        {
            static size_t s_TotalSystemMemorySize = GetTotalSystemMemory();
            return s_TotalSystemMemorySize;
        }
    };
} // namespace Savanna
