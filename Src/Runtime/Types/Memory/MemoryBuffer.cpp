#include "MemoryBuffer.h"

#include "Profiling/Profiler.h"

#include "Memory/MemoryManager.h"

namespace savanna
{
    MemoryBuffer::MemoryBuffer(AllocatorKind allocatorKind /*= kSavannaAllocatorKindHeap*/)
        : m_Buffer(nullptr)
        , m_Size(0)
        , m_AllocatorKind(allocatorKind)
    {
    }

    MemoryBuffer::MemoryBuffer(size_t size, AllocatorKind allocatorKind /*= kSavannaAllocatorKindHeap*/)
        : m_Buffer(nullptr)
        , m_Size(size)
        , m_AllocatorKind(allocatorKind)
    {
        SAVANNA_INSERT_SCOPED_PROFILER(MemoryBuffer::ctor);
        if (m_Size > 0)
            m_Buffer = SAVANNA_MALLOC(m_AllocatorKind, m_Size);
    }

    MemoryBuffer::MemoryBuffer(MemoryBuffer&& other)
        : m_Buffer(other.m_Buffer)
        , m_Size(other.m_Size)
        , m_AllocatorKind(other.m_AllocatorKind)
    {
        other.m_Buffer = nullptr;
        other.m_Size = 0;
        other.m_AllocatorKind = kSavannaAllocatorKindNone;
    }

    MemoryBuffer::~MemoryBuffer()
    {
        SAVANNA_INSERT_SCOPED_PROFILER(MemoryBuffer::dtor);
        if (m_Buffer != nullptr)
        {
            SAVANNA_FREE(m_AllocatorKind, m_Buffer);
        }

        m_Buffer = nullptr;
        m_Size = 0;
    }

    MemoryBuffer &MemoryBuffer::operator=(MemoryBuffer &&other)
    {
        if (this != &other)
        {
            m_Buffer = other.m_Buffer;
            m_Size = other.m_Size;

            other.m_Buffer = nullptr;
            other.m_Size = 0;
        }

        return *this;
    }

    void MemoryBuffer::Reset()
    {
        if (m_Buffer != nullptr)
        {
            SAVANNA_FREE(m_AllocatorKind, m_Buffer);
        }
    }

    void MemoryBuffer::Resize(const size_t size) {
        if (m_Size == size) SAVANNA_BRANCH_UNLIKELY
        {
            return;
        }
        else if (size != 0)
        {
            m_Buffer = m_Buffer == nullptr
                ? SAVANNA_MALLOC(m_AllocatorKind, size)
                : SAVANNA_REALLOC(m_AllocatorKind, m_Buffer, size);
        }
        else
        {
            Reset();
        }
    }
} // namespace Savanna
