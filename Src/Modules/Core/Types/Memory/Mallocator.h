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

namespace Savanna
{
    /**
     * @brief Extended from https://docs.microsoft.com/en-us/cpp/standard-library/allocators?view=msvc-170.
     * Implements a custom allocator for accessing system memory.
     *
     * @tparam T
     */
    template <class T>
    struct Mallocator : public Allocator
    {
        typedef T value_type;

        Mallocator() = default;
        Mallocator(const Mallocator&) = default;


        SAVANNA_NO_DISCARD void* alloc(const size_t& size, const size_t& alignment) SAVANNA_OVERRIDE;
        void free(void* const ptr, const size_t& alignment) SAVANNA_OVERRIDE;
    };
} // namespace Savanna
