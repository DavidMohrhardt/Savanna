#include "MemoryManager.h"
#include "Public/ISavannaMemory.hpp"

#include <cstring>

namespace Savanna
{
    template <se_uint32 LABEL>
    consteval se_AllocatorInterface_t GetInterfaceForLabel()
    {
        return se_AllocatorInterface_t
        {
            .m_AllocFunc = [](size_t size, void* pUserData) -> void*
            {
                return MemoryManager::Get().Allocate(size, LABEL);
            },
            .m_AllocAlignedFunc = [](size_t size, size_t alignment, void* pUserData) -> void*
            {
                return MemoryManager::Get().Allocate(size, alignment, LABEL);
            },
            .m_ReallocFunc = [](void* ptr, const size_t& newSize, void* pUserData) -> void*
            {
                return MemoryManager::Get().Reallocate(ptr, newSize, LABEL);
            },
            .m_ReallocAlignedFunc = [](void* ptr, size_t alignment, const size_t& newSize, void* pUserData) -> void*
            {
                return MemoryManager::Get().Reallocate(ptr, newSize, alignment, LABEL);
            },
            .m_FreeFunc = [](void* ptr, void* pUserData) -> void
            {
                MemoryManager::Get().Free(ptr, LABEL);
            }
        };
    }

    static constexpr se_AllocatorInterface_t k_LabelAllocatorInterfaces[] =
    {
        k_HeapAllocatorInterface,
        GetInterfaceForLabel<SE_MEMORY_LABEL_GENERAL>(),
        GetInterfaceForLabel<SE_MEMORY_LABEL_GRAPHICS>(),
        GetInterfaceForLabel<SE_MEMORY_LABEL_ENTITIES>(),
    };

    const se_AllocatorInterface_t MemoryManager::GetAllocatorInterfaceForLabel(
        const se_uint32 &label)
    {
        return k_LabelAllocatorInterfaces[label];
    }

    // TODO @DavidMohrhardt: Add initialization of memory manager based on a provided boot configuration.
    MemoryManager::MemoryManager() {}

    MemoryManager::~MemoryManager() {}

    void* MemoryManager::Allocate(size_t size, const se_uint32 label)
    {
        return Allocate(size, 1, label);
    }

    void* MemoryManager::Allocate(size_t size, size_t alignment, const se_uint32 label)
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

    bool MemoryManager::InitializeInternal() {
        m_MemoryArenas = std::move(DynamicArray<AtomicExpandableBlockAllocator>(
            SE_MEMORY_LABEL_COUNT, k_HeapAllocatorInterface));
        for (size_t i = 0; i < SE_MEMORY_LABEL_COUNT; ++i)
        {
            m_MemoryArenas.Append(std::move(AtomicExpandableBlockAllocator(1, sizeof(UnifiedPage4096KiB), true)));
        }
        return true;
    }

    void MemoryManager::StartInternal() {}

    void MemoryManager::StopInternal() {}

    void MemoryManager::ShutdownInternal()
    {
        m_MemoryArenas.~DynamicArray();
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
SAVANNA_EXPORT(const se_AllocatorInterface_t) SavannaMemoryManagerGetDefaultAllocatorInterface()
{
    return Savanna::MemoryManager::GetAllocatorInterfaceForLabel(SE_MEMORY_LABEL_GENERAL);
}
