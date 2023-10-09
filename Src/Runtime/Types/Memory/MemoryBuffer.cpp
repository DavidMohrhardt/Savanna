#include "MemoryBuffer.h"

#include "Profiling/Profiler.h"

#include "Memory/MemoryManager.h"

namespace Savanna
{
    MemoryBuffer::MemoryBuffer(MemoryLabel label /*= k_SavannaMemoryLabelHeap*/)
        : m_Buffer(nullptr)
        , m_Size(0)
        , m_MemoryLabel(label)
    {
    }

    MemoryBuffer::MemoryBuffer(size_t size, MemoryLabel label /*= k_SavannaMemoryLabelHeap*/)
        : m_Buffer(nullptr)
        , m_Size(size)
        , m_MemoryLabel(label)
    {
        SAVANNA_INSERT_SCOPED_PROFILER(MemoryBuffer::ctor);
        if (m_Size > 0)
            m_Buffer = MemoryManager::Get().Allocate(m_Size, (uint32)m_MemoryLabel);
    }

    MemoryBuffer::MemoryBuffer(MemoryBuffer&& other)
        : m_Buffer(other.m_Buffer)
        , m_Size(other.m_Size)
        , m_MemoryLabel(other.m_MemoryLabel)
    {
        other.m_Buffer = nullptr;
        other.m_Size = 0;
        other.m_MemoryLabel = k_SavannaMemoryLabelNone;
    }

    MemoryBuffer::~MemoryBuffer()
    {
        SAVANNA_INSERT_SCOPED_PROFILER(MemoryBuffer::dtor);
        if (m_Buffer != nullptr)
        {
            MemoryManager::Get().Free(m_Buffer, m_MemoryLabel);
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
}
