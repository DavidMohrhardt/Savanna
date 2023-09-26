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

#include "Types/Memory/InterfaceAllocator.h"

#include <initializer_list>

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
        InterfaceAllocator m_Allocator;

    public:
        DynamicArray(const size_t capacity = 0,
            const se_AllocatorInterface_t allocatorInterface = SavannaMemoryManagerGetDefaultAllocatorInterface())
            : m_Data(nullptr)
            , m_Size(0)
            , m_Capacity(capacity)
            , m_Allocator(allocatorInterface)
        {
            if (m_Capacity > 0)
            {
                m_Capacity = m_Capacity < DEFAULT_CAPACITY ? DEFAULT_CAPACITY : m_Capacity;
                m_Data = m_Allocator.NewArray<value_type>(m_Capacity);
            }
        }

        // Accept a std::initializer_list
        DynamicArray(
            std::initializer_list<value_type> list,
            const se_AllocatorInterface_t allocatorInterface = SavannaMemoryManagerGetDefaultAllocatorInterface())
            : DynamicArray(list.size(), allocatorInterface)
        {
            for (auto& item : list)
            {
                m_Data[m_Size++] = item;
            }
        }

        ~DynamicArray()
        {
            Clear();

            if (m_Data != nullptr)
            {
                m_Allocator.Free(m_Data);
                m_Data = nullptr;
            }
        }

        /**
         * @brief Copy the contents of another DynamicArray into this one using a different
         * allocator interface.
         *
         * @param other The other DynamicArray to copy from.
         * @param allocatorInterface The allocator interface to use for this DynamicArray.
         */
        DynamicArray(const DynamicArray& other, const se_AllocatorInterface_t allocatorInterface)
            : DynamicArray(0, allocatorInterface)
        {
            *this = other;
        }

        /**
         * @brief Copy the contents of another DynamicArray into this one.
         *
         * @param other The other DynamicArray to copy from.
         */
        DynamicArray(const DynamicArray& other)
            : DynamicArray(other, other.m_Allocator)
        {}

        /**
         * @brief Copy the contents of another DynamicArray into this one.
         *
         * @param other The other DynamicArray to copy from.
         */
        DynamicArray& operator=(const DynamicArray& other)
        {
            if (this != &other)
            {
                Clear();
                Reserve(other.m_Capacity);
                if (std::is_trivially_copyable_v<value_type>)
                {
                    std::memcpy(m_Data, other.m_Data, sizeof(value_type) * other.m_Size);
                }
                else
                {
                    for (size_t i = 0; i < other.m_Size; i++)
                    {
                        m_Data[i] = other.m_Data[i];
                    }
                }

                m_Size = other.m_Size;
            }

            return *this;
        }

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
                SAVANNA_MOVE_MEMBER(m_Allocator, other);
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

        value_type* Data()
        {
            return m_Data;
        }

        void Reserve(size_t capacity)
        {
            if (capacity <= m_Capacity)
                return;

            if (m_Data == nullptr)
            {
                m_Data = m_Allocator.AllocateAs<value_type>(capacity);
                m_Capacity = capacity;
                return;
            }

            if constexpr (std::is_trivially_copyable_v<value_type>)
            {
                value_type* previousBuffer = m_Data;
                m_Data = m_Allocator.AllocateAs<value_type>(capacity);
                memcpy(m_Data, previousBuffer, sizeof(value_type) * m_Size);
                m_Allocator.Free(previousBuffer);
            }
            else
            {
                value_type* previousBuffer = m_Data;
                m_Data = m_Allocator.AllocateAs<value_type>(capacity);
                for (size_t i = 0; i < m_Size; i++)
                {
                    m_Data[i] = std::move(previousBuffer[i]);
                }
                m_Allocator.Free(previousBuffer);
            }
        }

        void Resize(size_t size, bool initialize = false)
        {
            if (size > m_Capacity)
                Reserve(size);

            if (initialize)
            {
                for (size_t i = m_Size; i < size; i++)
                {
                    new (&m_Data[i]) value_type();
                }
            }
            m_Size = size;
        }

        void Clear()
        {
            if (m_Data == nullptr)
            {
                return;
            }

            if constexpr (!std::is_trivially_destructible_v<value_type>)
            {
                for (size_t i = 0; i < m_Size; i++)
                {
                    m_Data[i].~value_type();
                }
            }
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

        iterator begin()
        {
            return m_Data;
        }

        iterator end()
        {
            return m_Data + m_Size;
        }

#define SAVANNA_DEF_CONST_AND_NON_VARIANT_FUNC(__decl, __def) \
        const __decl const { __def } \
        __decl { __def }

        SAVANNA_DEF_CONST_AND_NON_VARIANT_FUNC(
            value_type& operator[](size_t index),
            {
                if (index >= m_Capacity)
                {
                    throw std::out_of_range("Index out of range.");
                }

                return m_Data[index];
            })

#undef SAVANNA_DEF_CONST_AND_NON_VARIANT_FUNC

        // STL compatibility
        inline void push_back(const value_type& value)
        {
            Append(value);
        }

        inline void push_back(value_type&& value)
        {
            Append(std::move(value));
        }

        inline void pop_back()
        {
            if (m_Size == 0)
            {
                throw std::out_of_range("Index out of range.");
            }

            m_Data[--m_Size].~value_type();
        }

        inline void erase(iterator position)
        {
            if (position < begin() || position >= end())
            {
                throw std::out_of_range("Index out of range.");
            }

            for (iterator it = position; it != end() - 1; it++)
            {
                *it = *(it + 1);
            }

            pop_back();
        }

        inline T* data()
        {
            return m_Data;
        }

        inline const T* data() const
        {
            return m_Data;
        }

        inline T& front()
        {
            if (m_Size == 0)
            {
                throw std::out_of_range("Index out of range.");
            }

            return m_Data[0];
        }

        inline const T& front() const
        {
            if (m_Size == 0)
            {
                throw std::out_of_range("Index out of range.");
            }

            return m_Data[0];
        }

        inline size_t size() const
        {
            return m_Size;
        }

        inline size_t capacity() const
        {
            return m_Capacity;
        }
    };
} // namespace Savanna
