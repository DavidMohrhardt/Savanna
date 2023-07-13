/**
 * @file LocklessList.h
 * @author David Mohrhardt (https://github.com/DavidMohrhardt/Savanna)
 * @brief
 * @version 0.1
 * @date 2023-05-14
 *
 * @copyright Copyright (c) 2023
 *
 */
#pragma once

#include "SavannaEngine.h"
#include "Utilities/SavannaCoding.h"

#include <algorithm>
#include <atomic>

namespace Savanna
{
    template <typename T>
    class LocklessList
    {
    private:
        struct Node
        {
            std::atomic<T> m_aData;
            std::atomic<Node*> m_apNext;

            Node(const T& data, Node* next)
                : m_aData(data)
                , m_apNext(next)
            {}

            Node(T&& data, Node* next)
                : m_aData(std::move(data))
                , m_apNext(next)
            {}

            bool operator==(const Node& other) const
            {
                return m_aData == other.m_aData && m_apNext == other.m_apNext;
            }
        };

        std::atomic<Node*> m_Head = nullptr;
        std::atomic<Node*> m_Tail = nullptr;

        std::atomic_size_t m_Count = 0;

        SAVANNA_NO_DISCARD inline static bool IsValidNode(const std::atomic<Node*>& node)
        {
            node.load(std::memory_order_acquire) != k_NullElement;
        }

        void AddNode(std::atomic<Node*>& node);
        void InsertNode(std::atomic<Node*>& node, size_t index);

    public:
        LocklessList() = default;
        LocklessList(const LocklessList&) = delete;
        LocklessList& operator=(const LocklessList&) = delete;

        ~LocklessList();

        SAVANNA_NO_DISCARD inline size_t Count() const noexcept
        {
            return m_Count.load(std::memory_order_acquire);
        }

        SAVANNA_NO_DISCARD inline bool IsEmpty() const noexcept
        {
            return Count() == 0;
        }

        void Add(const T& data);
        void Add(T&& data);

        void Remove(const size_t index);

        void Insert(const T& data, size_t index);
        void Insert(T&& data, size_t index);

        void Clear();
    };
} // namespace Savanna
