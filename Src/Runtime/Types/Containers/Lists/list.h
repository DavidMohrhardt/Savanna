/**
 * @file list.h
 * @author David Mohrhardt (https://github.com/DavidMohrhardt/Savanna)
 * @brief TODO @David.Mohrhardt Document
 * @version 0.1
 * @date 2023-10-31
 *
 * @copyright Copyright (c) 2023
 *
 */
#pragma once

#include "SavannaEngine.h"

#include "Utilities/SavannaCoding.h"

#include <algorithm>
#include <initializer_list>
#include <iterator>
#include <type_traits>

namespace Savanna
{
    // Implements the same interface as std::list except it accepts a AllocatorKind
    template <typename T>
    class list
    {
    public:
        using value_type = T;
        using allocator_type = AllocatorKind;
        using reference = T&;
        using const_reference = const T&;
        using pointer = T*;
        using const_pointer = const T*;

    private:
        AllocatorKind m_AllocatorKind;

        struct Node
        {
            Node* m_Next;
            Node* m_Prev;
            T m_Data;

            template <typename... Args>
            Node(Args&&... args) : m_Next(nullptr), m_Prev(nullptr), m_Data(std::forward<Args>(args)...) {}

            ~Node() = default;
        };

        Node* m_Head;
        Node* m_Tail;
        size m_Size;

        void InsertNode(Node* node, Node* prev, Node* next);
        void RemoveNode(Node* node);

    public:
        list(AllocatorKind allocatorKind = k_SavannaAllocatorKindHeap)
            : m_AllocatorKind(allocatorKind), m_Head(nullptr), m_Tail(nullptr)
        {}

        list(const list& other);
        list(list&& other) noexcept;

        list(std::initializer_list<T> init, AllocatorKind allocatorKind = k_SavannaAllocatorKindHeap)
            : m_AllocatorKind(allocatorKind), m_Head(nullptr), m_Tail(nullptr)
        {
            for (const auto& item : init)
            {
                push_back(item);
            }
        }

        ~list(){ clear(); }

        list& operator=(const list& other);
        list& operator=(list&& other) noexcept;

        void assign(std::initializer_list<T> init);
        void assign(size_t count, const T& value);

        void clear();

        void push_back(const T& value);
        void push_back(T&& value);

        void push_front(const T& value);
        void push_front(T&& value);

        template <typename... Args>
        T& emplace_back(Args&&... args);

        template <typename... Args>
        T& emplace_front(Args&&... args);

        void pop_back();
        void pop_front();

        void resize(size_t count);
        void resize(size_t count, const T& value);

        class iterator
        {
        private:
            Node* m_Node;

        public:
            using iterator_category = std::bidirectional_iterator_tag;
            using value_type = T;
            using difference_type = ptrdiff_t;
            using pointer = T*;
            using reference = T&;

            iterator(Node* node) : m_Node(node) {}
            iterator(const iterator& other) : m_Node(other.m_Node) {}

            iterator& operator=(const iterator& other)
            {
                m_Node = other.m_Node;
                return *this;
            }

            iterator& operator++()
            {
                m_Node = m_Node->m_Next;
                return *this;
            }

            iterator operator++(int)
            {
                iterator tmp = *this;
                ++(*this);
                return tmp;
            }

            iterator& operator--()
            {
                m_Node = m_Node->m_Prev;
                return *this;
            }

            iterator operator--(int)
            {
                iterator tmp = *this;
                --(*this);
                return tmp;
            }

            bool operator==(const iterator& rhs) const { return m_Node == rhs.m_Node; }
            bool operator!=(const iterator& rhs) const { return m_Node != rhs.m_Node; }

            reference operator*() const { return m_Node->m_Data; }
            pointer operator->() const { return &m_Node->m_Data; }
        };

        class const_iterator
        {
        private:
            const Node* m_Node;

        public:
            using iterator_category = std::bidirectional_iterator_tag;
            using value_type = T;
            using difference_type = ptrdiff_t;
            using pointer = T*;
            using reference = T&;

            const_iterator(const Node* node) : m_Node(node) {}
            const_iterator(const const_iterator& other) : m_Node(other.m_Node) {}
            const_iterator(const iterator& other) : m_Node(other.m_Node) {}

            const_iterator& operator=(const const_iterator& other)
            {
                m_Node = other.m_Node;
                return *this;
            }

            const_iterator& operator=(const iterator& other)
            {
                m_Node = other.m_Node;
                return *this;
            }

            const_iterator& operator++()
            {
                m_Node = m_Node->m_Next;
                return *this;
            }

            const_iterator operator++(int)
            {
                const_iterator tmp = *this;
                ++(*this);
                return tmp;
            }

            const_iterator& operator--()
            {
                m_Node = m_Node->m_Prev;
                return *this;
            }

            const_iterator operator--(int)
            {
                const_iterator tmp = *this;
                --(*this);
                return tmp;
            }

            bool operator==(const const_iterator& rhs) const { return m_Node == rhs.m_Node; }
            bool operator!=(const const_iterator& rhs) const { return m_Node != rhs.m_Node; }

            reference operator*() const { return m_Node->m_Data; }
            pointer operator->() const { return &m_Node->m_Data; }
        };

        iterator begin() { return iterator(m_Head); }
        iterator end() { return iterator(nullptr); }

        const_iterator begin() const { return const_iterator(m_Head); }
        const_iterator end() const { return const_iterator(nullptr); }

        bool empty() const { return m_Head == nullptr; }

        size_t size() const { return m_Size; }

        void swap(list& other);

        void remove(const T& value);
    };

    template <typename T>
    list<T>::list(const list& other)
        : m_AllocatorKind(other.m_AllocatorKind), m_Head(nullptr), m_Tail(nullptr)
    {
        for (const auto& item : other)
        {
            push_back(item);
        }
    }

    template <typename T>
    inline void list<T>::assign(size_t count, const T &value)
    {
        clear();
        for (size_t i = 0; i < count; ++i)
        {
            push_back(value);
        }
    }

    template <typename T>
    inline void list<T>::assign(std::initializer_list<T> init)
    {
        clear();
        for (const auto& item : init)
        {
            push_back(item);
        }
    }

    template <typename T>
    inline void list<T>::clear()
    {
        Node* node = m_Head;
        while (node != nullptr)
        {
            Node* next = node->m_Next;

            SAVANNA_DELETE(m_AllocatorKind, node);
            node = next;
        }
        m_Head = nullptr;
        m_Tail = nullptr;
        m_Size = 0;
    }

    template<typename T>
    inline void Savanna::list<T>::push_back(const T & value)
    {
        Node* node = SAVANNA_NEW(m_AllocatorKind, Node);
        *node = { nullptr, nullptr, value };

        if (m_Head == nullptr)
        {
            m_Head = node;
            m_Tail = node;
        }
        else
        {
            m_Tail->m_Next = node;
            node->m_Prev = m_Tail;
            m_Tail = node;
        }
        ++m_Size;
    }

    template<typename T>
    inline void Savanna::list<T>::push_back(T && value)
    {
        Node* node = SAVANNA_NEW(m_AllocatorKind, Node);
        *node = { nullptr, nullptr, std::move(value) };

        if (m_Head == nullptr)
        {
            m_Head = node;
            m_Tail = node;
        }
        else
        {
            m_Tail->m_Next = node;
            node->m_Prev = m_Tail;
            m_Tail = node;
        }
        ++m_Size;
    }

    template<typename T>
    inline void Savanna::list<T>::push_front(const T & value)
    {
        Node* node = SAVANNA_NEW(m_AllocatorKind, Node);
        *node = { nullptr, nullptr, value };

        if (m_Head == nullptr)
        {
            m_Head = node;
            m_Tail = node;
        }
        else
        {
            m_Head->m_Prev = node;
            node->m_Next = m_Head;
            m_Head = node;
        }
        ++m_Size;
    }

    template<typename T>
    inline void Savanna::list<T>::push_front(T && value)
    {
        Node* node = SAVANNA_NEW(m_AllocatorKind, Node);
        *node = { nullptr, nullptr, std::move(value) };

        if (m_Head == nullptr)
        {
            m_Head = node;
            m_Tail = node;
        }
        else
        {
            m_Head->m_Prev = node;
            node->m_Next = m_Head;
            m_Head = node;
        }
        ++m_Size;
    }

    template<typename T>
    template<typename ...Args>
    inline T & Savanna::list<T>::emplace_back(Args && ...args)
    {
        Node* node = SAVANNA_NEW(m_AllocatorKind, Node);
        *node = { nullptr, nullptr, std::forward<Args>(args)... };

        if (m_Head == nullptr)
        {
            m_Head = node;
            m_Tail = node;
        }
        else
        {
            m_Tail->m_Next = node;
            node->m_Prev = m_Tail;
            m_Tail = node;
        }
        ++m_Size;

        return node->m_Data;
    }

    template<typename T>
    template<typename ...Args>
    inline T & Savanna::list<T>::emplace_front(Args && ...args)
    {
        Node* node = SAVANNA_NEW(m_AllocatorKind, Node);
        *node = { nullptr, nullptr, std::forward<Args>(args)... };

        if (m_Head == nullptr)
        {
            m_Head = node;
            m_Tail = node;
        }
        else
        {
            m_Head->m_Prev = node;
            node->m_Next = m_Head;
            m_Head = node;
        }
        ++m_Size;

        return node->m_Data;
    }

    template<typename T>
    inline void list<T>::pop_back()
    {
        if (m_Tail == nullptr)
        {
            return;
        }

        Node* node = m_Tail;
        m_Tail = m_Tail->m_Prev;
        if (m_Tail != nullptr)
        {
            m_Tail->m_Next = nullptr;
        }
        else
        {
            m_Head = nullptr;
        }

        SAVANNA_DELETE(m_AllocatorKind, node);
        --m_Size;
    }

    template<typename T>
    inline void list<T>::pop_front()
    {
        if (m_Head == nullptr)
        {
            return;
        }

        Node* node = m_Head;
        m_Head = m_Head->m_Next;
        if (m_Head != nullptr)
        {
            m_Head->m_Prev = nullptr;
        }
        else
        {
            m_Tail = nullptr;
        }

        SAVANNA_DELETE(m_AllocatorKind, node);
        --m_Size;
    }

    template<typename T>
    inline void list<T>::resize(size_t count)
    {
        if (count == m_Size)
        {
            return;
        }

        if (count < m_Size)
        {
            while (m_Size > count)
            {
                pop_back();
            }
        }
        else
        {
            while (m_Size < count)
            {
                push_back(T());
            }
        }
    }

    template<typename T>
    inline void list<T>::resize(size_t count, const T & value)
    {
        if (count == m_Size)
        {
            return;
        }

        if (count < m_Size)
        {
            while (m_Size > count)
            {
                pop_back();
            }
        }
        else
        {
            while (m_Size < count)
            {
                push_back(value);
            }
        }
    }

    template<typename T>
    inline void list<T>::swap(list & other)
    {
        std::swap(m_AllocatorKind, other.m_AllocatorKind);
        std::swap(m_Head, other.m_Head);
        std::swap(m_Tail, other.m_Tail);
        std::swap(m_Size, other.m_Size);
    }

    template<typename T>
    inline void list<T>::remove(const T & value)
    {
        Node* node = m_Head;
        while (node != nullptr)
        {
            if (node->m_Data == value)
            {
                Node* next = node->m_Next;
                RemoveNode(node);
                node = next;
            }
            else
            {
                node = node->m_Next;
            }
        }
    }

    template<typename T>
    inline void list<T>::InsertNode(Node * node, Node * prev, Node * next)
    {
        if (prev != nullptr)
        {
            prev->m_Next = node;
        }
        else
        {
            m_Head = node;
        }

        if (next != nullptr)
        {
            next->m_Prev = node;
        }
        else
        {
            m_Tail = node;
        }

        node->m_Prev = prev;
        node->m_Next = next;
    }

    template<typename T>
    inline void list<T>::RemoveNode(Node * node)
    {
        Node* prev = node->m_Prev;
        Node* next = node->m_Next;

        if (prev != nullptr)
        {
            prev->m_Next = next;
        }
        else
        {
            m_Head = next;
        }

        if (next != nullptr)
        {
            next->m_Prev = prev;
        }
        else
        {
            m_Tail = prev;
        }

        SAVANNA_DELETE(m_AllocatorKind, node);
        --m_Size;
    }
} // namespace Savanna
