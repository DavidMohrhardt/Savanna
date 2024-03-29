/**
 * @file LocklessStack.h
 * @author David Mohrhardt (https://github.com/DavidMohrhardt/Savanna)
 * @brief
 * @version 0.1
 * @date 2023-09-16
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
    class LocklessStack
    {
    private:
        struct Node
        {
            Node* m_Next;
            T m_Data;
        };

        std::atomic<Node*> m_Head;

    public:
        LocklessStack() : m_Head(nullptr) {}
        ~LocklessStack() = default;

        LocklessStack(const LocklessStack&) = delete;
        LocklessStack& operator=(const LocklessStack&) = delete;

        LocklessStack(LocklessStack&&) = default;
        LocklessStack& operator=(LocklessStack&&) = default;

        void Push(T&& data)
        {
            Node* newNode = SAVANNA_NEW(Node);
            newNode->m_Data = std::move(data);
            newNode->m_Next = nullptr;

            Node* oldHead = m_Head.load(std::memory_order_relaxed);
            do
            {
                newNode->m_Next = oldHead;
            } while (!m_Head.compare_exchange_weak(oldHead, newNode, std::memory_order_release, std::memory_order_relaxed));
        }

        bool TryPop(T& data)
        {
            Node* oldHead = nullptr;
            do
            {
                oldHead = m_Head.load(std::memory_order_relaxed);
                if (oldHead == nullptr)
                {
                    return false;
                }
            } while (!m_Head.compare_exchange_weak(oldHead, oldHead->m_Next, std::memory_order_release, std::memory_order_relaxed));

            data = std::move(oldHead->m_Data);
            SAVANNA_DELETE(oldHead);
            return true;
        }

        bool IsEmpty() const
        {
            return m_Head.load(std::memory_order_relaxed) == nullptr;
        }

        // STL compatibility
        bool empty() const
        {
            return IsEmpty();
        }

        T& front()
        {
            return m_Head.load(std::memory_order_relaxed)->m_Data;
        }
    };
} // namespace Savanna
