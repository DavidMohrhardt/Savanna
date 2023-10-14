/**
 * @file LocklessQueue.h
 * @author David Mohrhardt (https://github.com/DavidMohrhardt/Savanna)
 * @brief
 * @version 0.1
 * @date 2023-01-20
 *
 * @copyright Copyright (c) 2023
 *
 */
#pragma once

#include "SavannaEngine.h"
#include "Utilities/SavannaCoding.h"

#include "Memory/MemoryManager.h"

#include <atomic>

namespace Savanna
{
    template <typename T>
    class LocklessQueue
    {
    private:
        MemoryLabel m_MemoryLabel;

        struct Node
        {
            Node* m_Next;
            T m_Data;
        };

        std::atomic<Node*> m_Head;
        std::atomic<Node*> m_Tail;

        void EnqueueNode(Node* node);

    public:
        LocklessQueue(MemoryLabel label = k_SavannaMemoryLabelHeap) : m_MemoryLabel(label), m_Head(nullptr), m_Tail(nullptr) {}
        ~LocklessQueue() = default;

        LocklessQueue(const LocklessQueue&) = delete;
        LocklessQueue& operator=(const LocklessQueue&) = delete;

        LocklessQueue(LocklessQueue&&) = default;
        LocklessQueue& operator=(LocklessQueue&&) = default;

        void Enqueue(const T& data);
        void Enqueue(T&& data);

        bool TryDequeue(T& data);

        bool IsEmpty() const
        {
            return m_Head.load(std::memory_order_relaxed) == nullptr;
        }
    };

    template <typename T>
    inline void LocklessQueue<T>::EnqueueNode(Node *node)
    {
        Node* checkedNodePtr { nullptr };
        // First if the head is null, set the head and tail to the new node
        if (m_Head.compare_exchange_strong(checkedNodePtr, node, std::memory_order_release, std::memory_order_relaxed))
        {
            m_Tail.store(node, std::memory_order_relaxed);
            return;
        }

        // If the head is not null, add the new node to the tail and set the tail to the new node
        while(!m_Tail.compare_exchange_weak(checkedNodePtr, node, std::memory_order_release, std::memory_order_relaxed))
        {
            checkedNodePtr = m_Tail.load(std::memory_order_relaxed);
        }

        checkedNodePtr->m_Next = node;
    }

    template <typename T> inline void LocklessQueue<T>::Enqueue(const T& data)
    {
        static_assert(std::is_copy_constructible<T>::value, "Type must be copy constructible!");
        Node* node = SAVANNA_NEW(m_MemoryLabel, Node);
        *node = { nullptr, data };

        EnqueueNode(node);
    }

    template <typename T> inline void LocklessQueue<T>::Enqueue(T&& data)
    {
        static_assert(std::is_move_constructible<T>::value, "Type must be move constructible!");
        Node* node = SAVANNA_NEW(m_MemoryLabel, Node);
        *node = { nullptr, std::move(data) };

        EnqueueNode(node);
    }

    template <typename T> inline bool LocklessQueue<T>::TryDequeue(T &data)
    {
        // Note: even if the head becomes null, we don't need to modify the tail as it will be overwritten
        // during the next enqueue operation.
        Node* oldHead { nullptr };
        do
        {
            oldHead = m_Head.load(std::memory_order_relaxed);
            if (oldHead == nullptr)
            {
                return false;
            }
        } while (!m_Head.compare_exchange_weak(oldHead, oldHead->m_Next, std::memory_order_release, std::memory_order_relaxed));

        data = std::move(oldHead->m_Data);
        SAVANNA_DELETE(m_MemoryLabel, oldHead);
        return true;
    }
} // namespace Savanna
