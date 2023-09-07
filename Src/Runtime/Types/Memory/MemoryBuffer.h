/**
 * @file MemoryBuffer.h
 * @author David Mohrhardt (https://github.com/DavidMohrhardt/Savanna)
 * @brief
 * @version 0.1
 * @date 2023-02-03
 *
 * @copyright Copyright (c) 2023
 *
 */
#pragma once

#include "Utilities/SavannaCoding.h"

namespace Savanna
{
    struct MemoryBuffer : public NonCopyable
    {
    protected:
        void* m_Buffer;
        size_t m_Size;

    public:
        MemoryBuffer();
        MemoryBuffer(size_t size);
        MemoryBuffer(MemoryBuffer&& other);
        ~MemoryBuffer();

    public:
        MemoryBuffer& operator=(MemoryBuffer&& other);

    public:
        SAVANNA_NO_DISCARD void* GetBuffer() const { return m_Buffer; }
        SAVANNA_NO_DISCARD size_t GetSize() const { return m_Size; }

        SAVANNA_NO_DISCARD bool IsValid() const { return m_Buffer != nullptr; }
    };
} // namespace Savanna::Memory
