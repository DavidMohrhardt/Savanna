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
        : m_Size(0)
        , m_AllocatedBytes(0)
        , m_Root(nullptr)
    {
    }

    Allocator::Allocator(void* root, size_t size)
        : m_Size(size)
        , m_AllocatedBytes(0)
        , m_Root(root)
    {
    }

    Allocator::Allocator(Allocator&& other)
        : m_Size(other.m_Size)
        , m_AllocatedBytes(other.m_AllocatedBytes)
        , m_Root(other.m_Root)
    {
        other.m_Root = nullptr;
        other.m_Size = 0;
        other.m_AllocatedBytes = 0;
    }

    Allocator &Allocator::operator=(Allocator &&other)
    {
        m_Size = other.m_Size;
        m_AllocatedBytes = other.m_AllocatedBytes;
        m_Root = other.m_Root;

        other.m_Root = nullptr;
        other.m_Size = 0;
        other.m_AllocatedBytes = 0;

        return *this;
    }
} // namespace Savanana
