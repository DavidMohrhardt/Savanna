/**
 * @file hash_map.h
 * @author David Mohrhardt (https://github.com/DavidMohrhardt/Savanna)
 * @brief TODO @David.Mohrhardt Document
 * @version 0.1
 * @date 2023-10-24
 *
 * @copyright Copyright (c) 2023
 *
 */
#pragma once

#include "Utilities/SavannaCoding.h"

#include "Memory/Public/ISavannaMemory.h"

#include <initializer_list>

namespace savanna
{
    class MemoryManager;

    // Conforms to the C++17 std::unordered_map interface but allows for custom allocators.
    template <typename Key, typename Value>
    class hash_map
    {
    private:
        struct Node
        {
            Key m_Key;
            Value m_Value;
            Node* m_Next = nullptr;

            Node(const Key& key, const Value& value)
                : m_Key(key)
                , m_Value(value)
            {
            }
        };

        Node** m_HashTable = nullptr;

        size_t m_Size = 0;
        size_t m_Capacity = 0;

        se_AllocatorKindBackingInt_t m_AllocatorKind;

        size_t m_Hash(const Key& key) const
        {
            return std::hash<Key>()(key) % m_Capacity;
        }

    public:
        hash_map() = default;
        hash_map(const size_t& capacity, const se_AllocatorKindBackingInt_t allocatorKind = kSavannaAllocatorKindHeap);
        hash_map(const std::initializer_list<std::pair<const Key, Value>>& initList, const se_AllocatorKindBackingInt_t allocatorKind = kSavannaAllocatorKindHeap);
        hash_map(const hash_map& other);
        hash_map(hash_map&& other) noexcept;
        ~hash_map();

        hash_map& operator=(const hash_map& other);
        hash_map& operator=(hash_map&& other) noexcept;

        Value& operator[](const Key& key);
        const Value& operator[](const Key& key) const;

        bool empty() const { return m_Size == 0; }
        size_t size() const { return m_Size; }
        size_t capacity() const { return m_Capacity; }

        void clear();

        void insert(const Key& key, const Value& value);
        void insert(const std::pair<const Key, Value>& pair);
        void insert(const std::initializer_list<std::pair<const Key, Value>>& initList);

        void erase(const Key& key);

        Value& at(const Key& key);
        const Value& at(const Key& key) const;

        bool contains(const Key& key) const;

        void reserve(const size_t& capacity);

        void swap(hash_map& other) noexcept;
    };

    template <typename Key, typename Value>
    hash_map<Key, Value>::hash_map(const size_t& capacity, const se_AllocatorKindBackingInt_t allocatorKind)
        : m_Capacity(capacity)
        , m_Size(0)
        , m_AllocatorKind(allocatorKind)
    {
        m_HashTable = static_cast<Node**>(MemoryManager::Get()->Allocate(sizeof(Node*) * m_Capacity, alignof(Node*), allocatorKind));
        memset(m_HashTable, 0, sizeof(Node*) * m_Capacity);
    }

    template <typename Key, typename Value>
    hash_map<Key, Value>::hash_map(const std::initializer_list<std::pair<const Key, Value>>& initList, const se_AllocatorKindBackingInt_t allocatorKind)
        : m_Capacity(initList.size())
        , m_Size(0)
        , m_AllocatorKind(allocatorKind)
    {
        m_HashTable = static_cast<Node**>(MemoryManager::Get()->Allocate(sizeof(Node*) * m_Capacity, alignof(Node*), allocatorKind));
        memset(m_HashTable, 0, sizeof(Node*) * m_Capacity);
        insert(initList);
    }

    template <typename Key, typename Value>
    hash_map<Key, Value>::hash_map(const hash_map& other)
    {
        reserve(other.m_Capacity);
        insert(other);
    }

    template <typename Key, typename Value>
    hash_map<Key, Value>::hash_map(hash_map&& other) noexcept
    {
        swap(other);
    }

    template <typename Key, typename Value>
    hash_map<Key, Value>::~hash_map()
    {
        clear();
        MemoryManager::Get()->Free(m_HashTable);
    }

    template <typename Key, typename Value>
    hash_map<Key, Value>& hash_map<Key, Value>::operator=(const hash_map& other)
    {
        if (this != &other)
        {
            clear();
            reserve(other.m_Capacity);
            insert(other);
        }
        return *this;
    }

    template <typename Key, typename Value>
    hash_map<Key, Value>& hash_map<Key, Value>::operator=(hash_map&& other) noexcept
    {
        swap(other);
        return *this;
    }

    template <typename Key, typename Value>
    Value& hash_map<Key, Value>::operator[](const Key& key)
    {
        const size_t hash = m_Hash(key);
        Node* node = m_HashTable[hash];
        while (node != nullptr)
        {
            if (node->m_Key == key)
            {
                return node->m_Value;
            }
            node = node->m_Next;
        }

        // If we get here, the key doesn't exist in the map yet.
        insert(key, Value{});
        return m_HashTable[hash]->m_Value;
    }

    template <typename Key, typename Value>
    const Value& hash_map<Key, Value>::operator[](const Key& key) const
    {
        const size_t hash = m_Hash(key);
        Node* node = m_HashTable[hash];
        while (node != nullptr)
        {
            if (node->m_Key == key)
            {
                return node->m_Value;
            }
            node = node->m_Next;
        }

        // If we get here, the key doesn't exist in the map yet.
        insert(key, Value{});
        return m_HashTable[hash]->m_Value;
    }

    template <typename Key, typename Value>
    void hash_map<Key, Value>::clear()
    {
        for (size_t i = 0; i < m_Capacity; ++i)
        {
            Node* node = m_HashTable[i];
            while (node != nullptr)
            {
                Node* next = node->m_Next;
                SAVANNA_DELETE(m_AllocatorKind, node);
                node = next;
            }
            m_HashTable[i] = nullptr;
        }
        m_Size = 0;
    }

    template <typename Key, typename Value>
    void hash_map<Key, Value>::insert(const Key& key, const Value& value)
    {
        const size_t hash = m_Hash(key);
        Node* node = m_HashTable[hash];
        while (node != nullptr)
        {
            if (node->m_Key == key)
            {
                node->m_Value = value;
                return;
            }
            node = node->m_Next;
        }

        // If we get here, the key doesn't exist in the map yet.
        Node* newNode = SAVANNA_NEW(m_AllocatorKind, Node, key, value);
        newNode->m_Next = m_HashTable[hash];
        m_HashTable[hash] = newNode;
        ++m_Size;
    }

    template <typename Key, typename Value>
    void hash_map<Key, Value>::insert(const std::pair<const Key, Value>& pair)
    {
        insert(pair.first, pair.second);
    }

    template <typename Key, typename Value>
    void hash_map<Key, Value>::insert(const std::initializer_list<std::pair<const Key, Value>>& initList)
    {
        for (const auto& pair : initList)
        {
            insert(pair);
        }
    }

    template <typename Key, typename Value>
    void hash_map<Key, Value>::erase(const Key& key)
    {
        const size_t hash = m_Hash(key);
        Node* node = m_HashTable[hash];
        Node* prev = nullptr;
        while (node != nullptr)
        {
            if (node->m_Key == key)
            {
                if (prev == nullptr)
                {
                    m_HashTable[hash] = node->m_Next;
                }
                else
                {
                    prev->m_Next = node->m_Next;
                }
                SAVANNA_DELETE(m_AllocatorKind, node);
                --m_Size;
                return;
            }
            prev = node;
            node = node->m_Next;
        }
    }

    template <typename Key, typename Value>
    Value& hash_map<Key, Value>::at(const Key& key)
    {
        const size_t hash = m_Hash(key);
        Node* node = m_HashTable[hash];
        while (node != nullptr)
        {
            if (node->m_Key == key)
            {
                return node->m_Value;
            }
            node = node->m_Next;
        }

        throw std::out_of_range("Key not found in hash_map!");
    }

    template <typename Key, typename Value>
    const Value& hash_map<Key, Value>::at(const Key& key) const
    {
        return const_cast<hash_map<Key, Value>*>(this)->at(key);
    }

    template <typename Key, typename Value>
    bool hash_map<Key, Value>::contains(const Key& key) const
    {
        const size_t hash = m_Hash(key);
        Node* node = m_HashTable[hash];
        while (node != nullptr)
        {
            if (node->m_Key == key)
            {
                return true;
            }
            node = node->m_Next;
        }
        return false;
    }

    template <typename Key, typename Value>
    void hash_map<Key, Value>::reserve(const size_t& capacity)
    {
        if (capacity > m_Capacity)
        {
            Node** newHashTable = static_cast<Node**>(MemoryManager::Get()->Allocate(sizeof(Node*) * capacity, alignof(Node*), m_AllocatorKind));
            memcpy(newHashTable, m_HashTable, sizeof(Node*) * m_Capacity);
            memset(newHashTable + m_Capacity, 0, sizeof(Node*) * (capacity - m_Capacity));
            MemoryManager::Get()->Free(m_HashTable);
            m_HashTable = newHashTable;
            m_Capacity = capacity;
        }
    }

    template <typename Key, typename Value>
    void hash_map<Key, Value>::swap(hash_map& other) noexcept
    {
        std::swap(m_HashTable, other.m_HashTable);
        std::swap(m_Size, other.m_Size);
        std::swap(m_Capacity, other.m_Capacity);
        std::swap(m_AllocatorKind, other.m_AllocatorKind);
    }
} // namespace Savanna
