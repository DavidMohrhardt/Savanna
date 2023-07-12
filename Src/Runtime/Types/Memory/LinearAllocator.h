/**
 * @file LinearAllocator.h
 * @author David Mohrhardt (https://github.com/DavidMohrhardt/Savanna)
 * @brief
 * @version 0.1
 * @date 2022-08-02
 *
 */
#pragma once

#include "Allocator.h"

#include "Types/Memory/CacheLine.h"

#include "Utilities/SavannaCoding.h"

namespace Savanna
{
    class LinearAllocator : public Allocator
    {
    private:
        void* m_Head;
        size_t m_Size;
        size_t m_AllocatedBytes;

    public:
        LinearAllocator();
        LinearAllocator(void* bufferPtr, size_t size);
        LinearAllocator(LinearAllocator& other) = delete;
        LinearAllocator(LinearAllocator&& other);
        ~LinearAllocator();

        SAVANNA_NO_DISCARD void* alloc(size_t size, const size_t& alignment);
        void free(void* const ptr, const size_t alignment) {}

        void Reset();

        SAVANNA_NO_DISCARD size_t GetAllocatedBytes() const { return m_AllocatedBytes; };
        SAVANNA_NO_DISCARD size_t GetSize() const { return m_Size; };
    };

} // namespace Savanna
