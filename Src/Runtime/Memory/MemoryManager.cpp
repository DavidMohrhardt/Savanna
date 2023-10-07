#include "SavannaMemory.h"
#include "MemoryManager.h"

#include "Profiling/Profiler.h"

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
        GetInterfaceForLabel<k_SavannaMemoryLabelGeneral>(),
        GetInterfaceForLabel<k_SavannaMemoryLabelGfx>(),
        GetInterfaceForLabel<k_SavannaMemoryLabelEcs>(),
    };

    const se_AllocatorInterface_t MemoryManager::GetAllocatorInterfaceForLabel(
        const MemoryLabel& label)
    {
        return k_LabelAllocatorInterfaces[(uint32)label];
    }

    bool MemoryManager::TryGetAllocatorInterfaceForLabel(
        const uint32& label,
        se_AllocatorInterface_t& outLabelInterface)
    {
        if (label >= k_SavannaMemoryLabelCount)
        {
            return false;
        }

        outLabelInterface = k_LabelAllocatorInterfaces[label];
        return true;
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
        // TODO @DavidMohrhardt: This is a hack to get the heap allocator working.
        //      Remove this once MemoryArena's are implemented.
        if (label == 0)
        {
            return k_HeapAllocatorInterface.m_AllocFunc(size, nullptr);
        }

        if (label - 1 >= m_MemoryArenas.Size())
        {
            throw BadAllocationException();
        }

        return m_MemoryArenas[label - 1].alloc(size, alignment);
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
        // TODO @DavidMohrhardt: This is a hack to get the heap allocator working.
        //      Remove this once MemoryArena's are implemented.
        if (label == 0)
        {
            return k_HeapAllocatorInterface.m_FreeFunc(ptr, nullptr);
        }

        if (label - 1 >= m_MemoryArenas.Size())
        {
            throw BadAllocationException();
        }

        m_MemoryArenas[label - 1].free(ptr, 1);
    }

    bool MemoryManager::InitializeInternal() {
        m_MemoryArenas = std::move(DynamicArray<AtomicExpandableBlockAllocator>(
            k_SavannaMemoryLabelCount, k_HeapAllocatorInterface));
        for (size_t i = 0; i < k_SavannaMemoryLabelCount; ++i)
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
    return Savanna::MemoryManager::GetAllocatorInterfaceForLabel(k_SavannaMemoryLabelGeneral);
}

SAVANNA_EXPORT(bool) SavannaMemoryManagerTryGetAllocatorInterfaceForLabel(const se_uint32& label, se_AllocatorInterface_t& outLabelInterface)
{
    return Savanna::MemoryManager::TryGetAllocatorInterfaceForLabel(label, outLabelInterface);
}
