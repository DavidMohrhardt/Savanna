/**
 * @file fixed_array.h
 * @author David Mohrhardt (https://github.com/DavidMohrhardt/Savanna)
 * @brief TODO @David.Mohrhardt Document
 * @version 0.1
 * @date 2023-10-19
 *
 * @copyright Copyright (c) 2023
 *
 */
#pragma once

#include "Utilities/SavannaCoding.h"

#include "dynamic_array.h"

#include <initializer_list>

namespace savanna
{
    class MemoryManager;

    // A class that conforms to the std::vector interface but disallows expansion, uses a custom allocator
    template <typename T>
    class fixed_array
    {
    private:
        void* m_Buffer = nullptr;
        size_t m_Size = 0;
        size_t m_Capacity = 0;
        const AllocatorKind m_ProviderAllocatorKind = kSavannaAllocatorKindHeap;

    public:
        fixed_array() = default;
        fixed_array(const size_t size, const AllocatorKind providerAllocatorKind = kSavannaAllocatorKindHeap);
        fixed_array(const std::initializer_list<T>& list);
        fixed_array(const fixed_array& other);
        fixed_array(const dynamic_array<T>& other);
        fixed_array(fixed_array&& other) noexcept;
        ~fixed_array();

        fixed_array& operator=(const fixed_array& other) = default;
        fixed_array& operator=(fixed_array&& other) noexcept = default;

        T& operator[](const size_t index);
        const T& operator[](const size_t index) const;

        void push_back(const T& value);
        void push_back(T&& value);

        void pop_back();

        void resize(const size_t size);

        void clear();

        size_t size() const { return m_Size; }

        T* data() { return reinterpret_cast<T*>(m_Buffer); }
        const T* data() const { return reinterpret_cast<T*>(m_Buffer); }
    };

    template <typename T>
    inline fixed_array<T>::fixed_array(const size_t size, const AllocatorKind providerAllocatorKind)
        : m_Capacity(size)
        , m_Size(size)
        , m_ProviderAllocatorKind(kSavannaAllocatorKindHeap)
    {
        SAVANNA_DEBUG_ASSERT(m_Capacity, "Can't create empty fixed array");

        auto interface = MemoryManager::Get()->GetAllocatorInterfaceForAllocatorKind(m_ProviderAllocatorKind);
        m_Buffer = SAVANNA_INTERFACE_ALLOCATE_ALIGNED(&interface, sizeof(T) * m_Capacity, alignof(T), nullptr);

        if constexpr (std::is_trivially_constructible_v<T>)
        {
            memset(m_Buffer, 0, sizeof(T) * m_Size);
        }
        else if constexpr (std::is_default_constructible_v<T>)
        {
            for (size_t i = 0; i < m_Size; ++i)
            {
                new (reinterpret_cast<T*>(m_Buffer) + i) T();
            }
        }
        else
        {
            throw std::runtime_error("Can't create fixed array of non-trivially constructible type");
        }
    }

    template <typename T>
    inline fixed_array<T>::fixed_array(const std::initializer_list<T>& list)
        : m_Capacity(list.size())
        , m_Size(list.size())
        , m_ProviderAllocatorKind(kSavannaAllocatorKindHeap)
    {
        SAVANNA_DEBUG_ASSERT(m_Capacity > 0, "Can't create empty fixed array");

        auto interface = MemoryManager::Get()->GetAllocatorInterfaceForAllocatorKind(m_ProviderAllocatorKind);
        m_Buffer = SAVANNA_INTERFACE_ALLOCATE_ALIGNED(&interface, sizeof(T) * m_Capacity, alignof(T), nullptr);

        size_t i = 0;
        for (auto& item : list)
        {
            new (reinterpret_cast<T*>(m_Buffer) + i) T(item);
            ++i;
        }
    }

    template <typename T>
    inline fixed_array<T>::fixed_array(const fixed_array &other)
        : m_Capacity(other.m_Capacity)
        , m_Size(other.m_Size)
        , m_ProviderAllocatorKind(kSavannaAllocatorKindHeap)
    {
        SAVANNA_DEBUG_ASSERT(m_Capacity > 0, "This shouldn't even be possible! Can't create empty fixed array");

        auto interface = MemoryManager::Get()->GetAllocatorInterfaceForAllocatorKind(m_ProviderAllocatorKind);
        m_Buffer = SAVANNA_INTERFACE_ALLOCATE_ALIGNED(&interface, sizeof(T) * m_Capacity, alignof(T), nullptr);

        if constexpr (std::is_trivially_copyable_v<T>)
        {
            memcpy(m_Buffer, other.m_Buffer, sizeof(T) * m_Size);
        }
        else
        {
            for (size_t i = 0; i < m_Size; ++i)
            {
                new (reinterpret_cast<T*>(m_Buffer) + i) T(other[i]);
            }
        }
    }

    template <typename T>
    inline fixed_array<T>::fixed_array(const dynamic_array<T> &other)
    {
        // Allow copies from dynamic arrays
        for (size_t i = 0; i < other.size(); ++i)
        {
            push_back(other[i]);
        }
    }

    template <typename T>
    inline fixed_array<T>::fixed_array(fixed_array &&other) noexcept
        : m_Buffer(other.m_Buffer)
        , m_Size(other.m_Size)
        , m_Capacity(other.m_Capacity)
        , m_ProviderAllocatorKind(other.m_ProviderAllocatorKind)
    {
        // This is fine because we're going to destroy the other object anyway
        other.m_Buffer = nullptr;
        other.m_Size = 0;
        other.m_Capacity = 0;
    }

    template <typename T>
    inline fixed_array<T>::~fixed_array()
    {
        if (m_Buffer)
        {
            if constexpr (!std::is_trivially_destructible_v<T>)
            {
                for (size_t i = 0; i < m_Size; ++i)
                {
                    reinterpret_cast<T*>(m_Buffer)[i].~T();
                }
            }

            auto interface = MemoryManager::Get()->GetAllocatorInterfaceForAllocatorKind(m_ProviderAllocatorKind);
            SAVANNA_INTERFACE_FREE(&interface, m_Buffer, nullptr);
        }
    }

    template <typename T>
    inline T &fixed_array<T>::operator[](const size_t index)
    {
        SAVANNA_DEBUG_ASSERT(index < m_Size, "Index out of bounds");
        return reinterpret_cast<T*>(m_Buffer)[index];
    }

    template <typename T>
    inline const T &fixed_array<T>::operator[](const size_t index) const
    {
        SAVANNA_DEBUG_ASSERT(index < m_Size, "Index out of bounds");
        return reinterpret_cast<T*>(m_Buffer)[index];
    }

    // The rest of the vector interface:

    template <typename T>
    inline void fixed_array<T>::push_back(const T& value)
    {
        if (m_Size == m_Capacity || m_Buffer == nullptr)
        {
            throw std::runtime_error("Can't push back on full array");
        }

        new (reinterpret_cast<T*>(m_Buffer) + m_Size) T(value);
    }

    template <typename T>
    inline void fixed_array<T>::push_back(T &&value)
    {
        if (m_Size == m_Capacity || m_Buffer == nullptr)
        {
            throw std::runtime_error("Can't push back on full array");
        }

        new (reinterpret_cast<T*>(m_Buffer) + m_Size) T(std::move(value));
    }

    template <typename T>
    inline void fixed_array<T>::pop_back()
    {
        SAVANNA_DEBUG_ASSERT(m_Size > 0, "Can't pop back on empty array");
        reinterpret_cast<T*>(m_Buffer)[m_Size - 1].~T();
        --m_Size;
    }

    template <typename T>
    inline void fixed_array<T>::resize(const size_t size)
    {
        // Not allowed to resize
        SAVANNA_DEBUG_ASSERT(size <= m_Capacity, "Can't resize fixed array");

        if (size == m_Size)
        {
            return;
        }

        if (size == 0)
        {
            clear();
            return;
        }

        for (size_t i = m_Size; i < size; ++i)
        {
            new (reinterpret_cast<T*>(m_Buffer) + i) T(value);
        }

        for (size_t i = size; i < m_Size; ++i)
        {
            reinterpret_cast<T*>(m_Buffer)[i].~T();
        }
    }

    template <typename T> inline void fixed_array<T>::clear()
    {
        if constexpr (!std::is_trivially_destructible_v<T>)
        {
            for (size_t i = 0; i < m_Size; ++i)
            {
                reinterpret_cast<T*>(m_Buffer)[i].~T();
            }
        }

        m_Size = 0;
    }
} // namespace Savanna
