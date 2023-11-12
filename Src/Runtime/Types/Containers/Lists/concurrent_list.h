/**
 * @file concurrent_list.h
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

#include "Types/Locks/MultiReadSingleWriteLock.h"

#include <algorithm>
#include <initializer_list>
#include <iterator>
#include <type_traits>

namespace Savanna
{

    // Implements the same interface as std::list except it accepts a AllocatorKind
    template <typename T>
    class concurrent_list
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
        MultiReadSingleWriteLock m_Lock;

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
        se_size m_Size;

    public:
        concurrent_list(AllocatorKind allocatorKind = kSavannaAllocatorKindHeap)
            : m_AllocatorKind(allocatorKind), m_Head(nullptr), m_Tail(nullptr)
        {}

        concurrent_list(const concurrent_list<T>& other);
        concurrent_list(concurrent_list&& other) noexcept;

        concurrent_list(std::initializer_list<T> init, AllocatorKind allocatorKind = kSavannaAllocatorKindHeap)
            : m_AllocatorKind(allocatorKind), m_Head(nullptr), m_Tail(nullptr)
        {
            for (const auto& item : init)
            {
                push_back(item);
            }
        }

        ~concurrent_list(){ clear(); }

        concurrent_list& operator=(const concurrent_list<T>& other);
        concurrent_list& operator=(concurrent_list&& other) noexcept;

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

        class const_iterator
        {
        public:
            using iterator_category = std::bidirectional_iterator_tag;
            using value_type = T;
            using difference_type = ptrdiff_t;
            using pointer = const T*;
            using reference = const T&;

        private:
            Node* m_Node;
            MultiReadSingleWriteLock* m_pLock;

        public:
            const_iterator()
                : m_Node(nullptr)
                , m_pLock(nullptr)
            {}

            const_iterator(Node* node, MultiReadSingleWriteLock* pLock)
                : m_Node(node)
                , m_pLock(lock)
            {
                if (m_pLock != nullptr)
                    m_pLock->ReadLock();
            }

            ~const_iterator()
            {
                if (m_pLock != nullptr)
                    m_pLock->ReadUnlock();
            }

            const_iterator(const const_iterator& other)
                : m_Node(other.m_Node)
                , m_pLock(other.m_pLock)
            {
                m_pLock->ReadLock();
            }

            const_iterator(const_iterator&& other) noexcept
                : m_Node(other.m_Node)
                , m_pLock(other.m_pLock)
            {
                other.m_Node = nullptr;
                other.m_pLock = nullptr;
            }

            const_iterator& operator=(const const_iterator& other)
            {
                if (this != &other)
                {
                    m_Node = other.m_Node;
                    m_pLock = other.m_pLock;
                    m_pLock->ReadLock();
                }
                return *this;
            }

            const_iterator& operator=(const_iterator&& other) noexcept
            {
                if (this != &other)
                {
                    m_Node = other.m_Node;
                    m_pLock = other.m_pLock;
                    other.m_Node = nullptr;
                    other.m_pLock = nullptr;
                }
                return *this;
            }

            const_iterator& operator++()
            {
                m_Node = m_Node->m_Next;
                return *this;
            }

            const_iterator operator++(int)
            {
                const_iterator tmp(*this);
                operator++();
                return tmp;
            }

            const_iterator& operator--()
            {
                m_Node = m_Node->m_Prev;
                return *this;
            }

            const_iterator operator--(int)
            {
                const_iterator tmp(*this);
                operator--();
                return tmp;
            }

            auto operator<=>(const const_iterator& other) const
            {
                return m_Node <=> other.m_Node;
            }
        };

        const_iterator begin() const { return const_iterator(m_Head); }
        const_iterator end() const { return const_iterator(nullptr); }

        bool empty() const
        {
            return m_Head == nullptr;
        }

        size_t size() const
        {
            return m_Size;
        }

        void swap(concurrent_list& other);

        void remove(const T& value);

    private:
        void InsertNodeUnsafe(Node* node, Node* prev, Node* next);
        void RemoveNodeUnsafe(Node* node);
    };

    template<typename T>
    inline concurrent_list<T>::concurrent_list(const concurrent_list<T> & other)
    {
        SAVANNA_READ_CRITICAL_SECTION(other.m_Lock,
        {
            for (const auto& item : other)
            {
                push_back(item);
            }
        });
    }

    template<typename T>
    inline concurrent_list<T>::concurrent_list(concurrent_list && other) noexcept
    {
        clear();
        SAVANNA_WRITE_CRITICAL_SECTION(other.m_Lock,
        SAVANNA_WRITE_CRITICAL_SECTION(m_Lock,
        {
            m_AllocatorKind = other.m_AllocatorKind;
            m_Head = other.m_Head;
            m_Tail = other.m_Tail;
            m_Size = other.m_Size;
            other.m_Head = nullptr;
            other.m_Tail = nullptr;
            other.m_Size = 0;
        }));
    }

    template <typename T>
    inline concurrent_list<T>& concurrent_list<T>::operator=(const concurrent_list<T> &other)
    {
        if (this != &other)
        {
            clear();
            SAVANNA_READ_CRITICAL_SECTION(other.m_Lock,
            {
                for (const auto& item : other)
                {
                    push_back(item);
                }
            });
        }
        return *this;
    }

    template <typename T>
    inline concurrent_list<T>&
    concurrent_list<T>::operator=(concurrent_list<T> &&other) noexcept
    {
        if (this != &other)
        {
            clear();
            SAVANNA_WRITE_CRITICAL_SECTION(other.m_Lock,
            SAVANNA_WRITE_CRITICAL_SECTION(m_Lock,
            {
                m_AllocatorKind = other.m_AllocatorKind;
                m_Head = other.m_Head;
                m_Tail = other.m_Tail;
                m_Size = other.m_Size;
                other.m_Head = nullptr;
                other.m_Tail = nullptr;
                other.m_Size = 0;
            }));
        }
        return *this;
    }

    template <typename T>
    inline void concurrent_list<T>::assign(std::initializer_list<T> init)
    {
        SAVANNA_WRITE_CRITICAL_SECTION(m_Lock,
        {
            for (int i = 0; i < m_Size; ++i)
            {
                auto* node = m_Head;
                RemoveNodeUnsafe(node);
                SAVANNA_DELETE(m_AllocatorKind, node);
            }

            m_Head = nullptr;
            m_Tail = nullptr;
            m_Size = 0;

            for (const auto& item : init)
            {
                InsertNodeUnsafe(
                    SAVANNA_NEW(m_AllocatorKind, Node, item),
                    m_Tail,
                    nullptr);
            }
        });
    }

    template <typename T>
    inline void concurrent_list<T>::assign(size_t count, const T &value)
    {
        Node* node = nullptr;
        SAVANNA_WRITE_CRITICAL_SECTION(m_Lock,
        {
            node = m_Head;
            m_Head = nullptr;
            m_Tail = nullptr;
            m_Size = 0;

            for (size_t i = 0; i < count; ++i)
            {
                InsertNodeUnsafe(
                    SAVANNA_NEW(m_AllocatorKind, Node, value),
                    m_Tail,
                    nullptr);
            }
        });

        // No need to lock here since we already cleared the list
        while (node != nullptr)
        {
            Node* next = node->m_Next;
            SAVANNA_DELETE(m_AllocatorKind, node);
            node = next;
        }
    }

    template <typename T> inline void concurrent_list<T>::clear()
    {
        Node* node = nullptr;
        SAVANNA_WRITE_CRITICAL_SECTION(m_Lock,
        {
            node = m_Head;
            m_Head = nullptr;
            m_Tail = nullptr;
            m_Size = 0;
        });

        while (node != nullptr)
        {
            Node* next = node->m_Next;
            SAVANNA_DELETE(m_AllocatorKind, node);
            node = next;
        }
    }

    template <typename T>
    inline void concurrent_list<T>::push_back(const T &value)
    {
        SAVANNA_WRITE_CRITICAL_SECTION(m_Lock,
        {
            InsertNodeUnsafe(
                SAVANNA_NEW(m_AllocatorKind, Node, value),
                m_Tail,
                nullptr);
        });
    }

    template <typename T>
    inline void concurrent_list<T>::push_back(T &&value)
    {
        SAVANNA_WRITE_CRITICAL_SECTION(m_Lock,
        {
            InsertNodeUnsafe(
                SAVANNA_NEW(m_AllocatorKind, Node, std::move(value)),
                m_Tail,
                nullptr);
        });
    }

    template <typename T>
    inline void concurrent_list<T>::push_front(const T &value)
    {
        SAVANNA_WRITE_CRITICAL_SECTION(m_Lock,
        {
            InsertNodeUnsafe(
                SAVANNA_NEW(m_AllocatorKind, Node, value),
                nullptr,
                m_Head);
        });
    }

    template <typename T>
    inline void concurrent_list<T>::push_front(T &&value)
    {
        SAVANNA_WRITE_CRITICAL_SECTION(m_Lock,
        {
            InsertNodeUnsafe(
                SAVANNA_NEW(m_AllocatorKind, Node, std::move(value)),
                nullptr,
                m_Head);
        });
    }

    template <typename T>
    template <typename... Args>
    inline T &concurrent_list<T>::emplace_back(Args &&... args)
    {
        Node* node = nullptr;
        SAVANNA_WRITE_CRITICAL_SECTION(m_Lock,
        {
            node = SAVANNA_NEW(m_AllocatorKind, Node, std::forward<Args>(args)...);
            InsertNodeUnsafe(node, m_Tail, nullptr);
        });
        return node->m_Data;
    }

    template <typename T>
    template <typename... Args>
    inline T &concurrent_list<T>::emplace_front(Args &&... args)
    {
        Node* node = nullptr;
        SAVANNA_WRITE_CRITICAL_SECTION(m_Lock,
        {
            node = SAVANNA_NEW(m_AllocatorKind, Node, std::forward<Args>(args)...);
            InsertNodeUnsafe(node, nullptr, m_Head);
        });
        return node->m_Data;
    }

    template <typename T> inline void concurrent_list<T>::pop_back()
    {
        Node* node = nullptr;
        SAVANNA_WRITE_CRITICAL_SECTION(m_Lock,
        {
            node = m_Tail;
            RemoveNodeUnsafe(node);
        });
        SAVANNA_DELETE(m_AllocatorKind, node);
    }

    template <typename T> inline void concurrent_list<T>::pop_front()
    {
        Node* node = nullptr;
        SAVANNA_WRITE_CRITICAL_SECTION(m_Lock,
        {
            node = m_Head;
            RemoveNodeUnsafe(node);
        });
        SAVANNA_DELETE(m_AllocatorKind, node);
    }

    template <typename T> inline void concurrent_list<T>::resize(size_t count)
    {
        SAVANNA_WRITE_CRITICAL_SECTION(m_Lock,
        {
            if (count < m_Size)
            {
                for (size_t i = 0; i < m_Size - count; ++i)
                {
                    Node* node = m_Tail;
                    RemoveNodeUnsafe(node);
                    SAVANNA_DELETE(m_AllocatorKind, node);
                }
            }
            else if (count > m_Size)
            {
                for (size_t i = 0; i < count - m_Size; ++i)
                {
                    InsertNodeUnsafe(
                        SAVANNA_NEW(m_AllocatorKind, Node),
                        m_Tail,
                        nullptr);
                }
            }
        });
    }

    template <typename T>
    inline void concurrent_list<T>::resize(size_t count, const T &value)
    {
        SAVANNA_WRITE_CRITICAL_SECTION(m_Lock,
        {
            if (count < m_Size)
            {
                for (size_t i = 0; i < m_Size - count; ++i)
                {
                    Node* node = m_Tail;
                    RemoveNodeUnsafe(node);
                    SAVANNA_DELETE(m_AllocatorKind, node);
                }
            }
            else if (count > m_Size)
            {
                for (size_t i = 0; i < count - m_Size; ++i)
                {
                    InsertNodeUnsafe(
                        SAVANNA_NEW(m_AllocatorKind, Node, value),
                        m_Tail,
                        nullptr);
                }
            }
        });
    }

    template <typename T> inline void concurrent_list<T>::swap(concurrent_list<T> &other)
    {
        if (this != &other)
        {
            SAVANNA_WRITE_CRITICAL_SECTION(other.m_Lock,
            SAVANNA_WRITE_CRITICAL_SECTION(m_Lock,
            {
                m_AllocatorKind = other.m_AllocatorKind;
                std::swap(m_Head, other.m_Head);
                std::swap(m_Tail, other.m_Tail);
                std::swap(m_Size, other.m_Size);
            }));
        }
        return *this;
    }

    template <typename T>
    inline void concurrent_list<T>::remove(const T &value)
    {
        SAVANNA_WRITE_CRITICAL_SECTION(m_Lock,
        {
            Node* node = m_Head;
            while (node != nullptr)
            {
                if (node->m_Data == value)
                {
                    Node* next = node->m_Next;
                    Node* prev = node->m_Prev;
                    RemoveNodeUnsafe(node);
                    SAVANNA_DELETE(m_AllocatorKind, node);
                    node = next;
                }
                else
                {
                    node = node->m_Next;
                }
            }
        });
    }

    template <typename T>
    inline void concurrent_list<T>::InsertNodeUnsafe(
        concurrent_list<T>::Node *node, concurrent_list<T>::Node *prev,
        concurrent_list<T>::Node *next)
    {
        if (!node)
            return;

        if (!prev)
        {
            SAVANNA_DEBUG_ASSERT(next == m_Head, "Error: node is not head");

            // node becomes new head
            node->m_Prev = nullptr;
            m_Head = node;
        }

        if (!next)
        {
            SAVANNA_DEBUG_ASSERT(prev == m_Tail, "Error: node is not tail");

            // node becomes new tail
            node->m_Next = nullptr;
            m_Tail = node;
        }

        node->m_Prev = prev;
        node->m_Next = next;
        ++m_Size;
    }

    template <typename T>
    inline void concurrent_list<T>::RemoveNodeUnsafe(
        Node *node)
    {
        if (!node)
            return;

        Node* prev = node->m_Prev;
        Node* next = node->m_Next;

        if (prev)
        {
            prev->m_Next = next;
        }
        else
        {
            SAVANNA_DEBUG_ASSERT(node == m_Head, "Error: node is not head");
            m_Head = next;
        }

        if (next)
        {
            next->m_Prev = prev;
        }
        else
        {
            SAVANNA_DEBUG_ASSERT(node == m_Tail, "Error: node is not tail");
            m_Tail = prev;
        }

        --m_Size;
    }

} // namespace Savanna
