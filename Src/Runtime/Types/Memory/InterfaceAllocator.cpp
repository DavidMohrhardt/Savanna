#include "InterfaceAllocator.h"
#include "Memory/SavannaMemory.h"

namespace Savanna
{
    InterfaceAllocator::InterfaceAllocator(se_AllocatorInterface_t allocatorInterface)
        : m_AllocatorInterface(allocatorInterface)
    {
        SAVANNA_ASSERT_VALID_ALLOCATOR_INTERFACE(m_AllocatorInterface);
    }

    InterfaceAllocator::InterfaceAllocator(const InterfaceAllocator& other)
        : m_AllocatorInterface(other.m_AllocatorInterface)
    {
        SAVANNA_ASSERT_VALID_ALLOCATOR_INTERFACE(m_AllocatorInterface);
    }

    InterfaceAllocator::InterfaceAllocator(InterfaceAllocator&& other) noexcept
        : m_AllocatorInterface(other.m_AllocatorInterface)
    {
        SAVANNA_ASSERT_VALID_ALLOCATOR_INTERFACE(m_AllocatorInterface);
        other.m_AllocatorInterface = {};
    }

    InterfaceAllocator& InterfaceAllocator::operator=(const InterfaceAllocator &other)
    {
        m_AllocatorInterface = other.m_AllocatorInterface;
        SAVANNA_ASSERT_VALID_ALLOCATOR_INTERFACE(m_AllocatorInterface);
        return *this;
    }

    InterfaceAllocator& InterfaceAllocator::operator=(InterfaceAllocator &&other)
    {
        m_AllocatorInterface = other.m_AllocatorInterface;
        SAVANNA_ASSERT_VALID_ALLOCATOR_INTERFACE(m_AllocatorInterface);
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

    SAVANNA_NO_DISCARD void* InterfaceAllocator::realloc(
        void *const ptr, const size_t &newSize,
        const size_t &alignment)
    {
        return ReallocateAligned(ptr, newSize, alignment);
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
