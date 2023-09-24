#include "InterfaceAllocator.h"

namespace Savanna
{
    inline void ValidateAllocatorInterface(const se_AllocatorInterface_t& interface)
    {
        // The functions that must be defined are allocAligned, free, reallocAligned
        // As the allocator can mask the missing alloc and realloc functions by calling
        // allocAligned and reallocAligned with alignment = 1
        SAVANNA_ASSERT(interface.m_AllocAlignedFunc != nullptr, "m_AllocatorInterface.m_AllocAlignedFunc is nullptr!");
        SAVANNA_ASSERT(interface.m_FreeFunc != nullptr, "m_AllocatorInterface.m_FreeFunc is nullptr!");
        SAVANNA_ASSERT(interface.m_ReallocAlignedFunc != nullptr, "m_AllocatorInterface.m_ReallocAlignedFunc is nullptr!");
    }

    InterfaceAllocator::InterfaceAllocator(se_AllocatorInterface_t allocatorInterface)
        : m_AllocatorInterface(allocatorInterface)
    {
        ValidateAllocatorInterface(m_AllocatorInterface);
    }

    InterfaceAllocator::InterfaceAllocator(const InterfaceAllocator& other)
        : m_AllocatorInterface(other.m_AllocatorInterface)
    {
        ValidateAllocatorInterface(m_AllocatorInterface);
    }

    InterfaceAllocator::InterfaceAllocator(InterfaceAllocator&& other) noexcept
        : m_AllocatorInterface(other.m_AllocatorInterface)
    {
        ValidateAllocatorInterface(m_AllocatorInterface);
        other.m_AllocatorInterface = {};
    }

    InterfaceAllocator& InterfaceAllocator::operator=(const InterfaceAllocator &other)
    {
        m_AllocatorInterface = other.m_AllocatorInterface;
        ValidateAllocatorInterface(m_AllocatorInterface);
        return *this;
    }

    InterfaceAllocator& InterfaceAllocator::operator=(InterfaceAllocator &&other)
    {
        m_AllocatorInterface = other.m_AllocatorInterface;
        ValidateAllocatorInterface(m_AllocatorInterface);
        other.m_AllocatorInterface = {};
        return *this;
    }

    SAVANNA_NO_DISCARD void* InterfaceAllocator::alloc(
        const size_t &size,
        const size_t &alignment)
    {
        return AllocateAligned(size, alignment);
    }

    void InterfaceAllocator::free(void* const ptr, const size_t& alignment)
    {
        Free(ptr);
    }

    void* InterfaceAllocator::Allocate(size_t size)
    {
        return m_AllocatorInterface.m_AllocFunc != nullptr
            ? m_AllocatorInterface.m_AllocFunc(size, nullptr)
            : AllocateAligned(size, 1);
    }

    void* InterfaceAllocator::AllocateAligned(size_t size, size_t alignment)
    {
        return m_AllocatorInterface.m_AllocAlignedFunc(size, alignment, nullptr);
    }

    void* InterfaceAllocator::Reallocate(void* pMemory, size_t newSize)
    {
        return m_AllocatorInterface.m_ReallocFunc != nullptr
            ? m_AllocatorInterface.m_ReallocFunc(pMemory, newSize, nullptr)
            : ReallocateAligned(pMemory, newSize, 1);
    }

    void* InterfaceAllocator::ReallocateAligned(void* pMemory, size_t newSize, size_t alignment)
    {
        return m_AllocatorInterface.m_ReallocAlignedFunc(pMemory, newSize, alignment, nullptr);
    }

    void InterfaceAllocator::Free(void* pMemory)
    {
        m_AllocatorInterface.m_FreeFunc(pMemory, nullptr);
    }
} // namespace Savanna
