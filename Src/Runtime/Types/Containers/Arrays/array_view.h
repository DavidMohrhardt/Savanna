/**
 * @file array_view.h
 * @author David Mohrhardt (https://github.com/DavidMohrhardt/Savanna)
 * @brief Provides an opaque view into a slice of contiguous memory.
 * @version 0.1
 * @date 2022-08-28
 *
 * @copyright Copyright (c) 2022
 */

#pragma once

#include "Utilities/SavannaCoding.h"

namespace Savanna
{
    template<typename T>
    class array_view
    {
    private:
        const T* m_DataPtr = nullptr;
        const size_t m_Size = 0;

    public:
        array_view(const T* pData, size_t size)
            : m_DataPtr(pData)
            , m_Size(size)
        {
            SAVANNA_ASSERT(pData != nullptr, "array_view: pData must not be null.");
            SAVANNA_ASSERT(size > 0, "array_view: size must be greater than 0.");
        }

    public:
        void CopyTo(T* pDestination, const size_t& start = 0, const size_t& length = 1) const
        {
            SAVANNA_DEBUG_ASSERT(length < m_Size, "array_view: length must be less than the size of the array.");
            SAVANNA_DEBUG_ASSERT(pDestination != nullptr, "array_view: pDestination must not be null.");
            memcpy(pDestination, m_DataPtr, m_Size * sizeof(T));
        }

    public:
        inline size_t GetSize() const { return m_Size; }
        inline const T& operator[](const size_t& index) const { return m_DataPtr[index]; }
    };
} // namespace Savanna
