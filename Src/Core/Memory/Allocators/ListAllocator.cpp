/**
 * @file ListAllocator.cpp
 * @author David Mohrhardt (https://github.com/DavidMohrhardt/Savanna)
 * @brief
 * @version 0.1
 * @date 2022-08-17
 *
 *
 */
#include "ListAllocator.h"

#include "AllocatorUtils.h"

namespace Savanna
{
    ListAllocator::ListAllocator()
        : m_Root(nullptr)
        , m_Head(nullptr)
        , m_Size(0)
        , m_AllocatedBytes(0)
    {}

    ListAllocator::ListAllocator(void* bufferPtr, size_t size)
        : m_Root(bufferPtr)
        , m_Head(bufferPtr)
        , m_Size(size)
        , m_AllocatedBytes(0)
    {}

    ListAllocator::~ListAllocator()
    {}

    ListAllocator::ListAllocator(ListAllocator&& other)
    {
        m_Root = other.m_Root;
        m_Head = other.m_Head;
        m_Size = other.m_Size;
        m_AllocatedBytes = other.m_AllocatedBytes;
        other.m_Root = nullptr;
        other.m_Head = nullptr;
        other.m_Size = 0;
        other.m_AllocatedBytes = 0;
    }

    void* ListAllocator::Allocate(size_t size, const size_t& alignment)
    {
        size_t forwardOffset = GetForwardAlignment(m_Head, alignment);
        size_t totalSize = size + forwardOffset;
        if (m_Size - m_AllocatedBytes < totalSize)
        {
            return nullptr;
        }
        void* result = Add(m_Head, forwardOffset);
        m_Head = Add(m_Head, totalSize);
        m_AllocatedBytes += totalSize;
        return result;
    }

    void ListAllocator::Reset()  { m_Head = m_Root; }
} // namespace Savanna
