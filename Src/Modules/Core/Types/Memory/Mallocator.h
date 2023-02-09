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

namespace Savanna
{
    /**
     * @brief Extended from https://docs.microsoft.com/en-us/cpp/standard-library/allocators?view=msvc-170.
     * Implements a custom allocator for accessing system memory.
     *
     * @tparam T
     */
    template <class T>
    struct Mallocator
    {
        typedef T value_type;

        Mallocator()
        {}

        // A converting copy constructor:
        template<class U> Mallocator(const Mallocator<U>&) SAVANNA_NOEXCEPT {}
        template<class U> bool operator==(const Mallocator<U>&) const SAVANNA_NOEXCEPT
        {
            return true;
        }

        template<class U> bool operator!=(const Mallocator<U>&) const SAVANNA_NOEXCEPT
        {
            return false;
        }

        SAVANNA_NO_DISCARD inline T* allocate(const size_t n) const;

        inline void deallocate(T* const p, size_t) const SAVANNA_NOEXCEPT;
    };
} // namespace Savanna
