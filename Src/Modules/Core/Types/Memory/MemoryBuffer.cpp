#include "MemoryBuffer.h"

namespace Savanna
{
    MemoryBuffer::MemoryBuffer()
        : m_Buffer(nullptr)
        , m_Size(0)
    {
    }

    MemoryBuffer::MemoryBuffer(size_t size)
        : m_Buffer(malloc(size))
        , m_Size(size)
    {
    }

    MemoryBuffer::MemoryBuffer(MemoryBuffer&& other)
        : m_Buffer(other.m_Buffer)
        , m_Size(other.m_Size)
    {
        other.m_Buffer = nullptr;
        other.m_Size = 0;
    }

    MemoryBuffer::~MemoryBuffer()
    {
        if (m_Buffer != nullptr)
        {
            free(m_Buffer);
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
