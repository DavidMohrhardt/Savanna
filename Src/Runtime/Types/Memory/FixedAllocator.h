/**
 * @file FixedAllocator.h
 * @author David Mohrhardt (https://github.com/DavidMohrhardt/Savanna)
 * @brief
 * @version 0.1
 * @date 2023-11-09
 *
 * @copyright Copyright (c) 2023
 *
 */

#pragma once

#include "Utilities/SavannaCoding.h"

#include "Allocator.h"
#include "AtomicAllocatorWrapper.h"
#include "MemoryBuffer.h"

namespace Savanna
{
    /**
     * @brief TODO @David.Mohrhardt Document
     */
    class FixedAllocator : public Allocator
    {
    private:
        AllocatorKind m_AllocatorKind;
        void* m_Root;
        size_t m_Size;
        size_t m_AllocatedBytes;

    public:
        FixedAllocator(size_t size, AllocatorKind allocatorKind = k_SavannaAllocatorKindHeap);
        FixedAllocator(void* root, size_t size);
        FixedAllocator(FixedAllocator&& other);

        ~FixedAllocator();

        FixedAllocator& operator=(FixedAllocator&& other);

        SAVANNA_NO_DISCARD void* alloc(const size_t& size, const size_t& alignment) SAVANNA_OVERRIDE;
        SAVANNA_NO_DISCARD void* realloc(void* const ptr, const size_t& newSize, const size_t& alignment) SAVANNA_OVERRIDE;
        void free(void* const ptr, const size_t& alignment) SAVANNA_OVERRIDE {/* Do nothing */}

        SAVANNA_NO_DISCARD size_t GetAllocatedBytes() const { return m_AllocatedBytes; }
        SAVANNA_NO_DISCARD size_t GetSize() const { return m_Size; }

        void Reset() { m_Size = 0; }

    private:
        SAVANNA_NO_DISCARD void* AllocateInternal(const size_t& size, const size_t& alignment);
    };

    using AtomicFixedAllocator = AtomicAllocatorWrapper<FixedAllocator>;
}
