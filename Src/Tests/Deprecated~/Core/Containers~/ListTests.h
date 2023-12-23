/**
 * @file ListTests.h
 * @author David Mohrhardt (https://github.com/DavidMohrhardt/Savanna)
 * @brief TODO @David.Mohrhardt Document
 * @version 0.1
 * @date 2023-05-27
 *
 * @copyright Copyright (c) 2023
 *
 */
#pragma once

#include <gtest/gtest.h>

namespace savanna::Core::Tests
{
    template <typename T>
    class ListUnitTestAdaptor
    {
    private:
        T m_List;
        int m_ExpectedCount = 0;

    public:
        ListUnitTestAdaptor() = default;

        template <typename... Args>
        ListUnitTestAdaptor(Args&&... args)
            : m_List(std::forward<Args>(args)...)
        {
        }

        virtual ~ListUnitTestAdaptor() = default;

        ListUnitTestAdaptor(const ListUnitTestAdaptor&) = delete;
        ListUnitTestAdaptor& operator=(const ListUnitTestAdaptor&) = delete;

        ListUnitTestAdaptor(ListUnitTestAdaptor&&) = default;
        ListUnitTestAdaptor& operator=(ListUnitTestAdaptor&&) = default;

        void Add(const typename T::value_type& data)
        {
            m_List.Add(data);
        }

        void Add(typename T::value_type&& data)
        {
            m_List.Add(std::move(data));
        }

        bool Remove(const typename T::value_type& data)
        {
            return m_List.Remove(data);
        }

        typename T::value_type& Get(size_t index)
        {
            return m_List.Get(index);
        }

        const typename T::value_type& Get(size_t index) const
        {
            return m_List.Get(index);
        }

        size_t Count() const
        {
            return m_List.Count();
        }

        bool IsEmpty() const
        {
            return m_List.IsEmpty();
        }

        void Clear()
        {
            m_List.Clear();
        }

public:

        typename T::iterator Begin()
        {
            auto beginIter = m_List.Begin();

            if (beginIter == m_List.End())
            {
                EXPECT_EQ(m_List.End(), m_List.Begin());
                EXPECT_EQ(m_List.Length(), 0);
                EXPECT_EQ(m_List.IsEmpty(), true);
            }
            else
            {
                EXPECT_NE(m_List.End(), m_List.Begin());
                EXPECT_EQ(m_List.Length(), m_List.Count());
                EXPECT_EQ(m_List.IsEmpty(), false);
            }

            return beginIter;
        }

        typename T::const_iterator Begin() const
        {
            auto beginIter = m_List.Begin();

            if (beginIter == m_List.End())
            {
                EXPECT_EQ(m_List.End(), m_List.Begin());
                EXPECT_EQ(m_List.Length(), 0);
                EXPECT_EQ(m_List.IsEmpty(), true);
            }
            else
            {
                EXPECT_NE(m_List.End(), m_List.Begin());
                EXPECT_EQ(m_List.Length(), m_List.Count());
                EXPECT_EQ(m_List.IsEmpty(), false);
            }

            return beginIter;
        }

        typename T::iterator End()
        {
            auto endIter = m_List.End();

            if (endIter == m_List.Begin())
            {
                EXPECT_EQ(m_List.End(), m_List.Begin());
                EXPECT_EQ(m_List.Length(), 0);
                EXPECT_EQ(m_List.IsEmpty(), true);
            }
            else
            {
                EXPECT_NE(m_List.End(), m_List.Begin());
                EXPECT_EQ(m_List.Length(), m_List.Count());
                EXPECT_EQ(m_List.IsEmpty(), false);
            }

            return endIter;
        }

        typename T::const_iterator End() const
        {
            return m_List.End();
        }

        typename T::iterator Find(const typename T::value_type& data)
        {
            return m_List.Find(data);
        }

        typename T::const_iterator Find(const typename T::value_type& data) const
        {
            return m_List.Find(data);
        }

        typename T::iterator Insert(const typename T::value_type& data, typename T::iterator position)
        {
            return m_List.Insert(data, position);
        }

        typename T::iterator Insert(typename T::value_type&& data, typename T::iterator position)
        {
            return m_List.Insert(std::move(data), position);
        }

        typename T::iterator Erase(typename T::iterator position)
        {
            return m_List.Erase(position);
        }

        typename T::iterator Erase(typename T::iterator first, typename T::iterator last)
        {
            return m_List.Erase(first, last);
        }

        typename T::value_type& operator[](size_t index)
        {
            return m_List[index];
        }

        const typename T::value_type& operator[](size_t index) const
        {
            return m_List[index];
        }

        bool operator==(const ListUnitTestAdaptor& other) const
        {
            return m_List == other.m_List;
        }

        bool operator!=(const ListUnitTestAdaptor& other) const
        {
            return m_List != other.m_List;
        }
    };
} // namespace savanna::Core::Tests
