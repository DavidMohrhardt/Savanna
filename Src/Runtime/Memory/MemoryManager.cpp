#include "MemoryManager.h"

#include <cstring>

namespace Savanna
{
    // TODO @DavidMohrhardt: Add initialization of memory manager based on a provided boot configuration.
    MemoryManager::MemoryManager() {}

    MemoryManager::~MemoryManager() {}

    void* MemoryManager::Allocate(size_t size, const se_uint32 label)
    {
        return Allocate(size, 1, label);
    }

    inline void* MemoryManager::Allocate(size_t size, size_t alignment, const se_uint32 label)
    {
        if (label >= m_MemoryArenas.Size())
        {
            throw BadAllocationException();
        }

        return m_MemoryArenas[label].alloc(size, alignment);
    }

    void* MemoryManager::Reallocate(
        void *ptr,
        size_t newSize,
        const se_uint32 label)
    {
        return Reallocate(ptr, newSize, 1, label);
    }

    void* MemoryManager::Reallocate(
        void* ptr,
        size_t newSize,
        size_t alignment,
        const se_uint32 label)
    {
        void* newPtr = Allocate(newSize, alignment, label);
        if (ptr)
        {
            memcpy(newPtr, ptr, newSize);
            Free(ptr, label);
        }
        return newPtr;
    }

    void MemoryManager::Free(void* ptr, const se_uint32 label)
    {
        if (label >= m_MemoryArenas.Size())
        {
            throw BadAllocationException();
        }

        m_MemoryArenas[label].free(ptr, 1);
    }

    bool MemoryManager::InitializeInternal()
    {
        m_InitializationAllocator = std::move(AtomicExpandableBlockAllocator(1, sizeof(UnifiedPage4096KiB), true));
        m_MemoryArenas = std::move(DynamicArray<AtomicExpandableBlockAllocator>(SE_MEMORY_LABEL_COUNT));
        for (size_t i = 0; i < SE_MEMORY_LABEL_COUNT; ++i)
        {
            m_MemoryArenas[i] = std::move(AtomicExpandableBlockAllocator(1, sizeof(UnifiedPage4096KiB), true));
        }
        return true;
    }

    void MemoryManager::StartInternal() {}

    void MemoryManager::StopInternal() {}

    void MemoryManager::ShutdownInternal()
    {
        m_InitializationAllocator.~AtomicAllocatorWrapper();
        m_MemoryArenas.Clear();
    }
} // namespace Savanna

#if ENABLE_OPERATOR_NEW_DELETE_OVERRIDES

// Operator new and delete overrides
void *operator new(size_t sz)
{
    return Savanna::MemoryManager::Get().Allocate(sz);
}

void* operator new(size_t sz, const uint32_t& label)
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

inline void* SavannaMemoryManagerAllocateAligned(
    size_t size,
    size_t alignment,
    void* pUserData)
{
    return Savanna::MemoryManager::Get().Allocate(size, alignment, SE_MEMORY_LABEL_GENERAL);
}

inline void* SavannaMemoryManagerAllocate(
    size_t size,
    void* pUserData)
{
    return Savanna::MemoryManager::Get().Allocate(size, 1, SE_MEMORY_LABEL_GENERAL);
}

inline void* SavannaMemoryManagerReallocAligned(
    void* ptr,
    size_t alignment,
    const size_t& newSize,
    void* pUserData)
{
    return Savanna::MemoryManager::Get().Reallocate(ptr, newSize, alignment, SE_MEMORY_LABEL_GENERAL);
}

inline void* SavannaMemoryManagerRealloc(
    void* ptr,
    const size_t& newSize,
    void* pUserData)
{
    return Savanna::MemoryManager::Get().Reallocate(ptr, newSize, 1, SE_MEMORY_LABEL_GENERAL);
}

inline void SavannaMemoryManagerFree(void* ptr, void* pUserData)
{
    if (ptr)
    {
        Savanna::MemoryManager::Get().Free(ptr, SE_MEMORY_LABEL_GENERAL);
    }
}

SAVANNA_EXPORT(const se_AllocatorInterface_t) SavannaMemoryManagerGetDefaultAllocatorInterface()
{
    return se_AllocatorInterface_t
    {
        .m_AllocFunc = SavannaMemoryManagerAllocate,
        .m_AllocAlignedFunc = SavannaMemoryManagerAllocateAligned,
        .m_ReallocFunc = SavannaMemoryManagerRealloc,
        .m_ReallocAlignedFunc = SavannaMemoryManagerReallocAligned,
        .m_FreeFunc = SavannaMemoryManagerFree
    };
}
