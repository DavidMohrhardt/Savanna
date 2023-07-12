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

#include <SavannaEngine.h>
#include <Utilities/SavannaCoding.h>

#include <atomic>

namespace Savanna
{
    template <typename T>
    class LocklessQueue
    {
    private:
        struct Node
        {
            Node* m_Next;
            T m_Data;
        };

        std::atomic<Node*> m_Head;

    public:
        LocklessQueue() : m_Head(nullptr) {}
        ~LocklessQueue() = default;

        LocklessQueue(const LocklessQueue&) = delete;
        LocklessQueue& operator=(const LocklessQueue&) = delete;

        LocklessQueue(LocklessQueue&&) = default;
        LocklessQueue& operator=(LocklessQueue&&) = default;

        void Push(const T& data)
        {
            Node* newNode = new Node();
            newNode->m_Data = data;
            newNode->m_Next = nullptr;

            Node* oldHead = m_Head.load(std::memory_order_relaxed);
            do
            {
                newNode->m_Next = oldHead;
            } while (!m_Head.compare_exchange_weak(oldHead, newNode, std::memory_order_release, std::memory_order_relaxed));
        }

        bool Pop(T& data)
        {
            Node* oldHead = m_Head.load(std::memory_order_relaxed);
            do
            {
                if (oldHead == nullptr)
                {
                    return false;
                }
            } while (!m_Head.compare_exchange_weak(oldHead, oldHead->m_Next, std::memory_order_release, std::memory_order_relaxed));

            data = oldHead->m_Data;
            delete oldHead;
            return true;
        }
    };
} // namespace Savanna
