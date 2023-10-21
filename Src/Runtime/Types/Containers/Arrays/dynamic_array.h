/**
 * @file dynamic_array.h
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

#include "Memory/Public/ISavannaMemory.h"
#include "Memory/MemoryLabel.h"

#include <initializer_list>

namespace Savanna
{
    class MemoryManager;

    // A class that conforms to the std::vector interface, but uses a custom allocator
    template <typename T>
    class dynamic_array
    {
    private:
        T* m_Buffer = nullptr;
        size_t m_Size = 0;
        size_t m_Capacity = 0;
        MemoryLabel m_ProviderLabel = k_SavannaMemoryLabelHeap;

    public:
        dynamic_array() = default;
        dynamic_array(const size_t size, const MemoryLabel providerLabel = k_SavannaMemoryLabelHeap);
        dynamic_array(const std::initializer_list<T>& list, const MemoryLabel providerLabel = k_SavannaMemoryLabelHeap);
        dynamic_array(const dynamic_array& other);
        dynamic_array(dynamic_array&& other) noexcept;
        ~dynamic_array();

        dynamic_array& operator=(const dynamic_array& other);
        dynamic_array& operator=(dynamic_array&& other) noexcept;

        T& operator[](const size_t index);
        const T& operator[](const size_t index) const;

        void push_back(const T& value);
        void push_back(T&& value);

        void pop_back();

        void resize(const size_t size);
        void resizeUninitialized(const size_t size);

        void clear();

        size_t size() const { return m_Size; }
        size_t capacity() const { return m_Capacity; }

        T* data() { return m_Buffer; }
        const T* data() const { return m_Buffer; }

        T* begin() { return m_Buffer; }
        const T* begin() const { return m_Buffer; }

        T* end() { return m_Buffer + m_Size; }
        const T* end() const { return m_Buffer + m_Size; }

    private:
        void Reserve(const size_t newCapacity);
    };

    template <typename T>
    inline dynamic_array<T>::dynamic_array(const size_t size, const MemoryLabel providerLabel)
        : m_Capacity(size)
        , m_Size(0)
        , m_ProviderLabel(providerLabel)
    {
        if (size == 0)
        {
            return;
        }

        auto interface = MemoryManager::Get()->GetAllocatorInterfaceForLabel(m_ProviderLabel);
        m_Buffer = reinterpret_cast<T*>(SAVANNA_INTERFACE_ALLOCATE_ALIGNED(&interface, sizeof(T) * m_Capacity, alignof(T), nullptr));
    }

    template <typename T>
    inline dynamic_array<T>::dynamic_array(const std::initializer_list<T>& list, const MemoryLabel providerLabel)
        : m_Capacity(list.size())
        , m_Size(0)
        , m_ProviderLabel(providerLabel)
    {
        if (list.size() == 0)
        {
            return;
        }

        auto interface = MemoryManager::Get()->GetAllocatorInterfaceForLabel(m_ProviderLabel);
        m_Buffer = reinterpret_cast<T*>(SAVANNA_INTERFACE_ALLOCATE_ALIGNED(&interface, sizeof(T) * m_Capacity, alignof(T), nullptr));

        if constexpr (std::is_trivially_copyable_v<T>)
        {
            memcpy(m_Buffer, list.begin(), sizeof(T) * m_Capacity);
        }
        else if constexpr (std::is_copy_constructible_v<T>)
        {
            for (auto& item : list)
            {
                push_back(item);
            }
        }
        else
        {
            for (auto& item : list)
            {
                push_back(std::move(item));
            }
        }
    }

    template <typename T>
    inline dynamic_array<T>::dynamic_array(const dynamic_array &other)
        : m_Capacity(other.m_Capacity)
        , m_Size(other.m_Size)
        , m_ProviderLabel(k_SavannaMemoryLabelHeap)
    {
        if (other.m_Size == 0)
        {
            return;
        }

        auto interface = MemoryManager::Get()->GetAllocatorInterfaceForLabel(m_ProviderLabel);
        m_Buffer = reinterpret_cast<T*>(SAVANNA_INTERFACE_ALLOCATE_ALIGNED(&interface, sizeof(T) * m_Capacity, alignof(T), nullptr));

        if constexpr (std::is_trivially_copyable_v<T>)
        {
            memcpy(m_Buffer, other.m_Buffer, sizeof(T) * m_Size);
        }
        else
        {
            for (size_t i = 0; i < m_Size; ++i)
            {
                new (m_Buffer + i) T(other[i]);
            }
        }
    }

    template <typename T>
    inline dynamic_array<T>::dynamic_array(dynamic_array &&other) noexcept
        : m_Buffer(other.m_Buffer)
        , m_Size(other.m_Size)
        , m_Capacity(other.m_Capacity)
        , m_ProviderLabel(other.m_ProviderLabel)
    {
        other.m_Buffer = nullptr;
        other.m_Size = 0;
        other.m_Capacity = 0;
    }

    template <typename T>
    inline dynamic_array<T>::~dynamic_array()
    {
        if (m_Buffer)
        {
            if constexpr (!std::is_trivially_destructible_v<T>)
            {
                for (size_t i = 0; i < m_Size; ++i)
                {
                    m_Buffer[i].~T();
                }
            }

            auto interface = MemoryManager::Get()->GetAllocatorInterfaceForLabel(m_ProviderLabel);
            SAVANNA_INTERFACE_FREE(&interface, m_Buffer, nullptr);
        }
    }

    template <typename T>
    inline dynamic_array<T>& dynamic_array<T>::operator=(const dynamic_array &other)
    {
        if (this == &other)
        {
            return *this;
        }

        if (m_ProviderLabel == k_SavannaMemoryLabelNone)
        {
            m_ProviderLabel = other.m_ProviderLabel;
        }

        this->Reserve(other.m_Capacity);

        m_Size = other.m_Size;
        if constexpr (std::is_trivially_copyable_v<T>)
        {
            memcpy(m_Buffer, other.m_Buffer, sizeof(T) * m_Size);
        }
        else
        {
            for (size_t i = 0; i < m_Size; ++i)
            {
                new (m_Buffer + i) T(other[i]);
            }
        }
    }

    template <typename T>
    inline dynamic_array<T>& dynamic_array<T>::operator=(dynamic_array &&other) noexcept
    {
        if (this == &other)
        {
            return *this;
        }

        if (m_Buffer)
        {
            if constexpr (!std::is_trivially_destructible_v<T>)
            {
                for (size_t i = 0; i < m_Size; ++i)
                {
                    m_Buffer[i].~T();
                }
            }

            auto interface = MemoryManager::Get()->GetAllocatorInterfaceForLabel(m_ProviderLabel);
            SAVANNA_INTERFACE_FREE(&interface, m_Buffer, nullptr);
        }

        SAVANNA_MOVE_ASSIGN(m_Buffer, other.m_Buffer);
        SAVANNA_MOVE_ASSIGN(m_Size, other.m_Size);
        SAVANNA_MOVE_ASSIGN(m_Capacity, other.m_Capacity);
        // SAVANNA_MOVE_ASSIGN(m_ProviderLabel, other.m_ProviderLabel);
        this->m_ProviderLabel = other.m_ProviderLabel;
        return *this;
    }

    template <typename T>
    inline T& dynamic_array<T>::operator[](const size_t index)
    {
        SAVANNA_DEBUG_ASSERT(index < m_Size, "Index out of bounds");
        return m_Buffer[index];
    }

    template <typename T>
    inline const T& dynamic_array<T>::operator[](const size_t index) const
    {
        SAVANNA_DEBUG_ASSERT(index < m_Size, "Index out of bounds");
        return m_Buffer[index];
    }

    // The rest of the vector interface:

    template <typename T>
    inline void dynamic_array<T>::push_back(const T& value)
    {
        if (m_Size == m_Capacity || m_Buffer == nullptr)
        {
            Reserve((m_Capacity + 1) * 2);
        }

        new (m_Buffer + m_Size) T(value);
        m_Size++;
    }

    template <typename T>
    inline void dynamic_array<T>::push_back(T &&value)
    {
        if (m_Size == m_Capacity || m_Buffer == nullptr)
        {
            Reserve((m_Capacity + 1) * 2);
        }

        new (m_Buffer + m_Size) T(std::move(value));
        m_Size++;
    }

    template <typename T>
    inline void dynamic_array<T>::pop_back()
    {
        SAVANNA_DEBUG_ASSERT(m_Size > 0, "Can't pop back on empty array");
        m_Buffer[m_Size - 1].~T();
        --m_Size;
    }

    template <typename T>
    inline void dynamic_array<T>::resize(const size_t size)
    {
        if (size == m_Size)
        {
            return;
        }

        if (size == 0)
        {
            clear();
            return;
        }

        if (size > m_Capacity)
        {
            Reserve(size);
        }

        if constexpr (std::is_trivially_constructible_v<T>)
        {
            memset(m_Buffer + m_Size, 0, sizeof(T) * (size - m_Size));
        }
        else
        {
            for (size_t i = m_Size; i < size; ++i)
            {
                new (m_Buffer + i) T();
            }
        }

        for (size_t i = size; i < m_Size; ++i)
        {
            m_Buffer[i].~T();
        }
        m_Size = size;
    }

    template <typename T>
    inline void dynamic_array<T>::resizeUninitialized(const size_t size)
    {
        if (size == m_Size)
        {
            return;
        }

        if (size > m_Capacity)
        {
            Reserve(size);
        }

        m_Size = size;
    }

    template <typename T> inline void dynamic_array<T>::clear()
    {
        if constexpr (!std::is_trivially_destructible_v<T>)
        {
            for (size_t i = 0; i < m_Size; ++i)
            {
                m_Buffer[i].~T();
            }
        }

        m_Size = 0;
    }

    template <typename T>
    inline void dynamic_array<T>::Reserve(const size_t newCapacity)
    {
        if (newCapacity == 0)
        {
            return;
        }

        auto interface = MemoryManager::Get()->GetAllocatorInterfaceForLabel(m_ProviderLabel);
        T* newBuffer = reinterpret_cast<T*>(SAVANNA_INTERFACE_ALLOCATE_ALIGNED(&interface, sizeof(T) * newCapacity, alignof(T), nullptr));
        if (m_Buffer)
        {
            if constexpr (std::is_trivially_copyable_v<T>)
            {
                memcpy(newBuffer, m_Buffer, sizeof(T) * m_Size);
            }
            else
            {
                for (size_t i = 0; i < m_Size; ++i)
                {
                    new (newBuffer + i) T(std::move(m_Buffer[i]));
                }
            }

            SAVANNA_INTERFACE_FREE(&interface, m_Buffer, nullptr);
        }

        m_Buffer = newBuffer;
        m_Capacity = newCapacity;
    }

} // namespace Savanna
