/**
 * @file Mallocator.cpp
 * @author David Mohrhardt (DavidMohrhardt@github.com)
 * @brief TODO @DavidMohrhardt Document
 * @version 0.1
 * @date 2022-08-02
 *
 * @copyright Copyright (c) 2022
 *
 */

#include "Mallocator.h"

namespace Savanna
{
    template <class T>
    SAVANNA_NO_DISCARD inline T* Mallocator<T>::allocate(const size_t size) const
    {
        if (size == 0)
        {
            return nullptr;
        }

        if (size > static_cast<size_t>(-1) / sizeof(T))
        {
            throw BadArrayNewLengthException;
        }

        void* const dataPtr = malloc(size * sizeof(T));

        if (dataPtr == nullptr)
        {
            throw BadAllocationException();
        }

        return static_cast<T*>(dataPtr);
    }

    template<class T>
    inline void Mallocator<T>::deallocate(T* const p, size_t) const SAVANNA_NO_EXCEPT
    {
        free(p);
    }
} // namespace Savanna
