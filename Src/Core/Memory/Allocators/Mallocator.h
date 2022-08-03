#pragma once

#include "Types/Exceptions/SavannaException.h"
#include "Types/Primitive/PrimitiveTypes.h"
#include "Types/Primitive/PrimitiveUnions.h"
#include "Utilities/Macros/CppCommonDefs.h"

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

        Mallocator() SAVANNA_NO_EXCEPT
        {}

        // A converting copy constructor:
        template<class U> Mallocator(const Mallocator<U>&) SAVANNA_NO_EXCEPT {}
        template<class U> bool operator==(const Mallocator<U>&) const SAVANNA_NO_EXCEPT
        {
            return true;
        }

        template<class U> bool operator!=(const Mallocator<U>&) const SAVANNA_NO_EXCEPT
        {
            return false;
        }

        SAVANNA_NO_DISCARD inline T* allocate(const size_t n) const;

        inline void deallocate(T* const p, size_t) const SAVANNA_NO_EXCEPT;
    };
} // namespace Savanna
