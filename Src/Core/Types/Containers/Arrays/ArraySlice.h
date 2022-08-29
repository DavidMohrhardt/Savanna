/**
 * @file ArraySlice.h
 * @author David Mohrhardt (https://github.com/DavidMohrhardt/Savanna)
 * @brief Provides an opaque view into a slice of contiguous memory.
 * @version 0.1
 * @date 2022-08-28
 *
 * @copyright Copyright (c) 2022
 */

#pragma once

#include <Utilities/SavannaCoding.h>

namespace Savanna
{
    template<typename T>
    class ArraySlice
    {
    private:
        const T* m_DataPtr = nullptr;
        const size_t m_Size = 0;

    public:
        ArraySlice(const T* pData, size_t size)
            : m_DataPtr(pData)
            , m_Size(size)
        {
            assert(pData != nullptr);
            assert(size > 0);
        }

        // TODO @DavidMohrhardt consider disallowing copy and move construction.
        ArraySlice(const ArraySlice&) = default;
        ArraySlice(ArraySlice&&) = default;
        ArraySlice& operator=(const ArraySlice&) = default;
        ArraySlice& operator=(ArraySlice&&) = default;
        ~ArraySlice() = default;

    public:
        void CopyTo(T* pDestination, const size_t& start = 0, const size_t& length = 1) const
        {
            assert(length < m_Size);
            assert(pDestination != nullptr);
            memcpy(pDestination, m_DataPtr, m_Size * sizeof(T));
        }

    public:
        inline size_t GetSize() const { return m_Size; }
        inline const T& operator[](const size_t& index) const { return m_DataPtr[index]; }
    };
} // namespace Savanna
