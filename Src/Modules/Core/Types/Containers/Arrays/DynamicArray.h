#pragma once

#include "Utilities/SavannaCoding.h"

namespace Savanna
{
    template<class T>
    class DynamicArray
    {
    private:
        T* m_Data;
        size_t m_Size;
        size_t m_Capacity;

    public:
        DynamicArray();
        DynamicArray(const size_t& size);
        DynamicArray(const size_t& size, const T& value);
        DynamicArray(const DynamicArray& other);
        DynamicArray(DynamicArray&& other);
        ~DynamicArray();

    public:
        DynamicArray& operator=(const DynamicArray& other);
        DynamicArray& operator=(DynamicArray&& other);

    public:
        SAVANNA_NO_DISCARD T& operator[](const size_t& index);
        SAVANNA_NO_DISCARD const T& operator[](const size_t& index) const;

    public:
        SAVANNA_NO_DISCARD size_t GetSize() const { return m_Size; };
        SAVANNA_NO_DISCARD size_t GetCapacity() const { return m_Capacity; };
        SAVANNA_NO_DISCARD bool IsEmpty() const { return m_Size == 0; };
        SAVANNA_NO_DISCARD bool IsValid() const { return m_Data != nullptr; };

    public:
        void Push(const T& value);
        void Push(T&& value);
        void Pop();
        void Clear();
        void Resize(const size_t& size);
        void Reserve(const size_t& capacity);

    private:
        void Reallocate(const size_t& capacity);

    private:
        void Copy(const DynamicArray& other);
    };

    template <class T>
    inline DynamicArray<T>::DynamicArray() {}

    template <class T>
    inline DynamicArray<T>::DynamicArray(const size_t &size)
        : m_Size(size)
        , m_Capacity(size)
        , m_Data(reinterpret_cast<T*>(malloc(sizeof(T) * size)))
    {}

    template <class T>
    inline DynamicArray<T>::DynamicArray(const size_t &size, const T &value)
        : m_Size(size)
        , m_Capacity(size)
        , m_Data(reinterpret_cast<T*>(malloc(sizeof(T) * size)))
    {
        for (size_t i = 0; i < size; i++)
        {
            m_Data[i] = value;
        }
    }

    template <class T>
    inline DynamicArray<T>::DynamicArray(const DynamicArray &other)
    {
        Copy(other);
    }

    template <class T>
    inline DynamicArray<T>::DynamicArray(DynamicArray &&other)
    {
        DynamicArray::Move(other);
    }

    template <class T>
    inline DynamicArray<T>::~DynamicArray()
    {
        Clear();
        free(m_Data);
    }

    template <class T>
    inline DynamicArray<T> &DynamicArray<T>::operator=(const DynamicArray &other)
    {
        Copy(other);
        return *this;
    }

    template <class T>
    inline DynamicArray<T> &DynamicArray<T>::operator=(DynamicArray &&other)
    {
        m_Size = other.m_Size;
        m_Capacity = other.m_Capacity;
        m_Data = other.m_Data;

        other.m_Size = 0;
        other.m_Capacity = 0;
        other.m_Data = nullptr;
        return *this;
    }

    template <class T>
    inline T &DynamicArray<T>::operator[](const size_t &index)
    {
        return m_Data[index];
    }

    template <class T>
    inline const T &DynamicArray<T>::operator[](const size_t &index) const
    {
        return m_Data[index];
    }

    template <class T>
    inline void DynamicArray<T>::Push(const T &value)
    {
        if (m_Size >= m_Capacity)
        {
            Reallocate(m_Capacity * 2);
        }

        m_Data[m_Size++] = value;
    }

    template <class T>
    inline void DynamicArray<T>::Push(T &&value)
    {
        if (m_Size >= m_Capacity)
        {
            Reallocate(m_Capacity * 2);
        }

        m_Data[m_Size++] = std::move(value);
    }

    template <class T>
    inline void DynamicArray<T>::Pop()
    {
        if (m_Size > 0)
        {
            m_Size--;
        }
    }

    template <class T>
    inline void DynamicArray<T>::Clear()
    {
        for (size_t i = 0; i < m_Size; i++)
        {
            m_Data[i].~T();
        }
        m_Size = 0;
    }

    template <class T>
    inline void DynamicArray<T>::Resize(const size_t &size)
    {
        if (size > m_Capacity)
        {
            Reallocate(size);
        }

        if (size > m_Size)
        {
            for (size_t i = m_Size; i < size; i++)
            {
                m_Data[i] = T();
            }
        }
        else if (size < m_Size)
        {
            for (size_t i = size; i < m_Size; i++)
            {
                m_Data[i].~T();
            }
        }

        m_Size = size;
    }

    template <class T>
    inline void DynamicArray<T>::Reserve(const size_t &capacity)
    {
        if (capacity > m_Capacity)
        {
            Reallocate(capacity);
        }
    }

    template <class T>
    inline void DynamicArray<T>::Reallocate(const size_t &capacity)
    {
        T* newData = reinterpret_cast<T*>(malloc(sizeof(T) * capacity));

        for (size_t i = 0; i < m_Size; i++)
        {
            newData[i] = std::move(m_Data[i]);
        }

        free(m_Data);
        m_Data = newData;
        m_Capacity = capacity;
    }

    template <class T>
    inline void DynamicArray<T>::Copy(const DynamicArray &other)
    {
        m_Size = other.m_Size;
        m_Capacity = other.m_Capacity;
        m_Data = reinterpret_cast<T*>(malloc(sizeof(T) * m_Capacity));

        for (size_t i = 0; i < m_Size; i++)
        {
            m_Data[i] = other.m_Data[i];
        }
    }
} // namespace Savanna
