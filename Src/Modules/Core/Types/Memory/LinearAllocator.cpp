/**
 * @file LinearAllocator.cpp
 * @author David Mohrhardt (https://github.com/DavidMohrhardt/Savanna)
 * @brief
 * @version 0.1
 * @date 2022-08-17
 *
 *
 */
#include "LinearAllocator.h"

#include "AllocatorUtils.h"

namespace Savanna
{
    LinearAllocator::LinearAllocator()
        : Allocator()
        , m_Head(nullptr)
    {}

    LinearAllocator::LinearAllocator(void* bufferPtr, size_t size)
        : Allocator(bufferPtr)
        , m_Head(bufferPtr)
    {}

    LinearAllocator::~LinearAllocator()
    {}

    LinearAllocator::LinearAllocator(LinearAllocator&& other)
        : Allocator(std::move(other))
        , m_Head(other.m_Head)
    {
        other.m_Root = nullptr;
        other.m_Head = nullptr;
        other.m_Size = 0;
        other.m_AllocatedBytes = 0;
    }

    void* LinearAllocator::alloc(size_t size, const size_t& alignment)
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

    void LinearAllocator::Reset()  { m_Head = m_Root; }
} // namespace Savanna
