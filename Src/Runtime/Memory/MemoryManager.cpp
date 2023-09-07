#include "MemoryManager.h"

#include <cstring>

namespace Savanna
{
    static constexpr size_t k_DefaultAlignment = L1CacheLineLength();

    // TODO @DavidMohrhardt: Add initialization of memory manager based on a provided boot configuration.
    MemoryManager::MemoryManager() {}

    MemoryManager::~MemoryManager() {}

    void* MemoryManager::Allocate(size_t size, const MemoryLabel label)
    {
        return Allocate(size, k_DefaultAlignment, label);
    }

    inline void* MemoryManager::Allocate(size_t size, size_t alignment, const MemoryLabel label)
    {
        return m_RootAllocator.alloc(size, alignment);
    }

    void MemoryManager::Free(void* ptr)
    {
        m_RootAllocator.free(ptr, k_DefaultAlignment);
    }

    bool MemoryManager::InitializeInternal()
    {
        m_RootAllocator = std::move(AtomicExpandableBlockAllocator(1, sizeof(UnifiedPage4096KiB), true));
        return true;
    }

    void MemoryManager::StartInternal() {}

    void MemoryManager::StopInternal() {}

    void MemoryManager::ShutdownInternal()
    {
        m_RootAllocator.~AtomicAllocatorWrapper();
    }
} // namespace Savanna

#if ENABLE_OPERATOR_NEW_DELETE_OVERRIDES

// Operator new and delete overrides
void *operator new(size_t sz)
{
    return Savanna::MemoryManager::Get().Allocate(sz);
}

void* operator new(size_t sz, const se_MemoryLabel_t& label)
{
    return Savanna::MemoryManager::Get().Allocate(sz, label);
}

void *operator new[](size_t sz)
{
    return Savanna::MemoryManager::Get().Allocate(sz);
}

void operator delete(void *ptr) noexcept
{
    Savanna::MemoryManager::Get().Free(ptr);
}

void operator delete(void *ptr, size_t size) noexcept
{
    Savanna::MemoryManager::Get().Free(ptr);
}

void operator delete[](void *ptr) noexcept
{
    Savanna::MemoryManager::Get().Free(ptr);
}

void operator delete[](void *ptr, size_t size) noexcept
{
    Savanna::MemoryManager::Get().Free(ptr);
}

#endif // ENABLE_OPERATOR_NEW_DELETE_OVERRIDES

// C-Api

inline void* savanna_memory_manager_allocate(
    size_t size,
    size_t alignment,
    se_MemoryLabel_t label)
{
    return Savanna::MemoryManager::Get().Allocate(size, alignment, label);
}

inline void savanna_memory_manager_free(void* ptr)
{
    if (ptr)
    {
        Savanna::MemoryManager::Get().Free(ptr);
    }
}

inline void* savanna_memory_manager_realloc(
    void* ptr,
    const size_t& originalSize,
    const size_t& newSize)
{
    void* newPtr = savanna_memory_manager_allocate(newSize);
    memcpy(newPtr, ptr, originalSize);
    savanna_memory_manager_free(ptr);
    return newPtr;
}
