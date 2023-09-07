/**
 * @file DynamicArray.h
 * @author David Mohrhardt (https://github.com/DavidMohrhardt/Savanna)
 * @brief
 * @version 0.1
 * @date 2023-05-16
 *
 * @copyright Copyright (c) 2023
 *
 */
#pragma once

#include "Utilities/SavannaCoding.h"

#include "Memory/MemoryManager.h"

namespace Savanna
{
    template <typename T>
    class DynamicArray
    {
    private:
        static constexpr size_t DEFAULT_CAPACITY = 8;

        using value_type = T;

        using iterator = value_type*;
        using const_iterator = const value_type*;

        value_type* m_Data;
        size_t m_Size;
        size_t m_Capacity;

    public:
        DynamicArray(size_t capacity, bool initialize = false)
            : m_Data(nullptr)
            , m_Size(0)
            , m_Capacity(capacity)
        {
            if (initialize)
            {
                m_Data = SAVANNA_NEW_ARRAY(value_type, capacity);
            }
            else
            {
                m_Data = malloc(sizeof(value_type) * capacity);
            }
        }

        DynamicArray() : m_Data(nullptr), m_Size(0), m_Capacity(0) {}
        ~DynamicArray()
        {
            if (m_Data != nullptr)
                delete[] m_Data;
        }

        DynamicArray(const DynamicArray& other) = delete;

        DynamicArray(DynamicArray&& other) noexcept
        {
            *this = std::move(other);
        }

        DynamicArray& operator=(DynamicArray&& other) SAVANNA_NOEXCEPT
        {
            if (this != &other)
            {
                SAVANNA_MOVE_MEMBER(m_Data, other);
                SAVANNA_MOVE_MEMBER(m_Size, other);
                SAVANNA_MOVE_MEMBER(m_Capacity, other);
            }

            return *this;
        }

        void Append(const value_type& value)
        {
            if (m_Size == m_Capacity)
                Reserve(m_Capacity * 2);

            m_Data[m_Size++] = value;
        }

        void Append(value_type&& value)
        {
            if (m_Size == m_Capacity)
                Reserve(m_Capacity * 2);

            m_Data[m_Size++] = std::move(value);
        }

        void Reserve(size_t capacity)
        {
            if (capacity <= m_Capacity)
                return;

            value_type* newData = SAVANNA_NEW_ARRAY(value_type, capacity);
            for (size_t i = 0; i < m_Size; i++)
                newData[i] = std::move(m_Data[i]);

            SAVANNA_DELETE_ARRAY(m_Data);
            m_Data = newData;
            m_Capacity = capacity;
        }

        void Resize(size_t size)
        {
            if (size > m_Capacity)
                Reserve(size);

            m_Size = size;
        }

        void Clear()
        {
            m_Size = 0;
        }

        size_t Size() const
        {
            return m_Size;
        }

        size_t Capacity() const
        {
            return m_Capacity;
        }

        bool Empty() const
        {
            return m_Size == 0;
        }

#define SAVANNA_DEF_CONST_AND_NON_VARIANT_FUNC(__decl, __def) \
        const __decl const { __def } \
        __decl { __def }

        SAVANNA_DEF_CONST_AND_NON_VARIANT_FUNC(
            value_type& At(size_t index),
            {
                if (index >= m_Size)
                    throw std::out_of_range("Index out of range");
                return m_Data[index];
            });

        SAVANNA_DEF_CONST_AND_NON_VARIANT_FUNC(
            value_type& operator[](size_t index),
            {
                return At(index);
            })

#undef SAVANNA_DEF_CONST_AND_NON_VARIANT_FUNC
    };
} // namespace Savanna
