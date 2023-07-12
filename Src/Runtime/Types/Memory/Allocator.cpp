/**
 * @file Allocator.cpp
 * @author David Mohrhardt (https://github.com/DavidMohrhardt/Savanna)
 * @brief
 * @version 0.1
 * @date 2022-08-19
 *
 */
#include "Allocator.h"

namespace Savanna
{
    Allocator::Allocator()
        : m_Root(nullptr)
    {
    }

    Allocator::Allocator(void* root)
        : m_Root(root)
    {
    }

    Allocator::Allocator(Allocator&& other)
        : m_Root(other.m_Root)
    {
        other.m_Root = nullptr;
    }

    Allocator &Allocator::operator=(Allocator &&other)
    {
        m_Root = other.m_Root;
        other.m_Root = nullptr;

        return *this;
    }
} // namespace Savanana
