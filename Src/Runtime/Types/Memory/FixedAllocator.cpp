#include "FixedAllocator.h"

#include "Memory/MemoryManager.h"

namespace Savanna
{
    FixedAllocator::FixedAllocator(
        size_t size,
        AllocatorKind allocatorKind)
        : m_AllocatorKind{allocatorKind}
        , m_Root{nullptr}
        , m_Size{size}
        , m_AllocatedBytes{0}
    {
        m_Root = SAVANNA_MALLOC(m_AllocatorKind, size);
    }

    FixedAllocator::FixedAllocator(void *root, size_t size)
        : m_AllocatorKind{kSavannaAllocatorKindNone}
        , m_Root{root}
        , m_Size{size}
        , m_AllocatedBytes{0}
    {}

    FixedAllocator::FixedAllocator(FixedAllocator &&other)
    {
        *this = std::move(other);
    }

    FixedAllocator &FixedAllocator::operator=(FixedAllocator &&other)
    {
        if (this != &other)
        {
            if (m_Root != nullptr && m_AllocatorKind != kSavannaAllocatorKindNone)
            {
                SAVANNA_FREE(m_AllocatorKind, m_Root);
            }

            SAVANNA_MOVE_MEMBER_ASSIGN(m_AllocatorKind, other);
            SAVANNA_MOVE_MEMBER_ASSIGN(m_Size, other);
            SAVANNA_MOVE_MEMBER_ASSIGN(m_AllocatedBytes, other);
        }
        return *this;
    }

    FixedAllocator::~FixedAllocator()
    {
        if (m_Root != nullptr && m_AllocatorKind != kSavannaAllocatorKindNone)
        {
            SAVANNA_FREE(m_AllocatorKind, m_Root);
        }

        m_Root = nullptr;
        m_AllocatorKind = kSavannaAllocatorKindNone;
        m_Size = 0;
        m_AllocatedBytes = 0;
    }

    SAVANNA_NO_DISCARD void* FixedAllocator::alloc(const size_t& size, const size_t& alignment)
    {
        return AllocateInternal(size, alignment);
    }

    SAVANNA_NO_DISCARD void* FixedAllocator::realloc(void* const ptr, const size_t& newSize, const size_t& alignment)
    {
        return AllocateInternal(newSize, alignment);
    }

    SAVANNA_NO_DISCARD inline void* FixedAllocator::AllocateInternal(const size_t& size, const size_t& alignment)
    {
        byte* pBuffer = (byte*)m_Root + m_AllocatedBytes;
        size_t forwardAlignment = GetForwardAlignment(pBuffer, alignment);
        size_t requiredSize = size + forwardAlignment;
        if (requiredSize >= m_Size - m_AllocatedBytes)
        {
            m_AllocatedBytes += requiredSize;
            return static_cast<void*>(pBuffer + forwardAlignment);
        }

        if (size <= m_Size - m_AllocatedBytes)
        {
            // In this case just return the last scratch buffer and accept the perf hit
            m_AllocatedBytes += size;
            return pBuffer;
        }

        throw std::bad_alloc();
    }

} // namespace Savanna
