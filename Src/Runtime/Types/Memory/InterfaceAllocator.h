/**
 * @file InterfaceAllocator.h
 * @author David Mohrhardt (https://github.com/DavidMohrhardt/Savanna)
 * @brief
 * @version 0.1
 * @date 2023-09-19
 *
 * @copyright Copyright (c) 2023
 *
 */
#pragma once
#include "IAllocator.h"
#include "Memory/Public/ISavannaMemory.h"

namespace Savanna
{
    class InterfaceAllocator final : public IAllocator
    {
    public:
        InterfaceAllocator(se_AllocatorInterface_t allocatorInterface = SavannaMemoryGetHeapAllocatorInterface());

        // These allocators are allowed to move and copy
        InterfaceAllocator(const InterfaceAllocator& other);
        InterfaceAllocator(InterfaceAllocator&& other) noexcept;
        InterfaceAllocator& operator=(const InterfaceAllocator& other);
        InterfaceAllocator& operator=(InterfaceAllocator&& other);

        SAVANNA_NO_DISCARD void* alloc(const size_t& size, const size_t& alignment) override;
        void free(void* const ptr, const size_t& alignment) override;

        ~InterfaceAllocator() = default;

        void* Allocate(size_t size);
        void* AllocateAligned(size_t size, size_t alignment);
        void* Reallocate(void* pMemory, size_t newSize);
        void* ReallocateAligned(void* pMemory, size_t newSize, size_t alignment);
        void Free(void* pMemory);

        const se_AllocatorInterface_t& GetInterface() const { return m_AllocatorInterface; }

    private:
        se_AllocatorInterface_t m_AllocatorInterface;
    };
} // namespace Savanna
