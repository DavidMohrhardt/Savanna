/**
 * @file Map.h
 * @author David Mohrhardt (https://github.com/DavidMohrhardt/Savanna)
 * @brief
 * @version 0.1
 * @date 2023-05-13
 *
 * @copyright Copyright (c) 2023
 *
 */
#pragma once

namespace Savanna
{
    template <typename K, typename V>
    class Map
    {
    private:
        struct Node
        {
            K m_Key;
            V m_Value;

            Node* m_Next = nullptr;
            Node* m_Prev = nullptr;

            Node() = default;
            Node(const K& key, const V& value)
                : m_Key(key)
                , m_Value(value)
            {}
            Node(const K& key, V&& value)
                : m_Key(key)
                , m_Value(std::move(value))
            {}
            Node(K&& key, const V& value)
                : m_Key(std::move(key))
                , m_Value(value)
            {}
            Node(K&& key, V&& value)
                : m_Key(std::move(key))
                , m_Value(std::move(value))
            {}
            ~Node() = default;
        };

        List<Node> m_List;
    };
} // namespace Savanna
