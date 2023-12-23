/**
 * @file MemoryBuffer.h
 * @author David Mohrhardt (https://github.com/DavidMohrhardt/Savanna)
 * @brief TODO @David.Mohrhardt Document
 * @version 0.1
 * @date 2023-02-03
 *
 * @copyright Copyright (c) 2023
 *
 */
#pragma once

#include "Utilities/SavannaCoding.h"

namespace savanna
{
    struct MemoryBuffer : public NonCopyable
    {
    protected:
        void* m_Buffer;
        size_t m_Size;
        AllocatorKind m_AllocatorKind;

    public:
        MemoryBuffer(AllocatorKind allocatorKind = kSavannaAllocatorKindHeap);
        MemoryBuffer(size_t size, AllocatorKind allocatorKind = kSavannaAllocatorKindHeap);
        MemoryBuffer(MemoryBuffer&& other);
        ~MemoryBuffer();

        MemoryBuffer& operator=(MemoryBuffer&& other);

        SAVANNA_NO_DISCARD void* GetBuffer() const { return m_Buffer; }
        SAVANNA_NO_DISCARD size_t GetSize() const { return m_Size; }

        SAVANNA_NO_DISCARD bool IsValid() const { return m_Buffer != nullptr; }
        SAVANNA_NO_DISCARD bool PointerIsInBuffer(void* const ptr) const { return ptr >= m_Buffer && ptr < Add(m_Buffer, m_Size); }

        void Reset();
        void Resize(const size_t size);
    };
} // namespace savanna::Memory
