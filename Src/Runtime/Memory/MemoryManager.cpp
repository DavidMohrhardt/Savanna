#include "SavannaMemory.h"
#include "MemoryManager.h"

#include "Profiling/Profiler.h"

#include <array>
#include <cstring>
#include "Public/ISavannaMemory.h"

#define ENABLE_MEMORY_MANAGEMENT 1

namespace Savanna
{
    template <se_MemoryLabelBackingInt_t LABEL>
    consteval se_AllocatorInterface_t GetInterfaceForLabel()
    {
#if !ENABLE_MEMORY_MANAGEMENT
        return k_HeapAllocatorInterface;
#else // ENABLE_MEMORY_MANAGEMENT
        return se_AllocatorInterface_t
        {
            .m_AllocFunc = [](size_t size, void* pUserData) -> void*
            {
                return MemoryManager::Get()->Allocate(size, LABEL);
            },
            .m_AllocAlignedFunc = [](size_t size, size_t alignment, void* pUserData) -> void*
            {
                return MemoryManager::Get()->Allocate(size, alignment, LABEL);
            },
            .m_ReallocFunc = [](void* ptr, const size_t& newSize, void* pUserData) -> void*
            {
                return MemoryManager::Get()->Reallocate(ptr, newSize, LABEL);
            },
            .m_ReallocAlignedFunc = [](void* ptr, size_t alignment, const size_t& newSize, void* pUserData) -> void*
            {
                return MemoryManager::Get()->Reallocate(ptr, newSize, alignment, LABEL);
            },
            .m_FreeFunc = [](void* ptr, void* pUserData) -> void
            {
                MemoryManager::Get()->Free(ptr, LABEL);
            }
        };
#endif // !ENABLE_MEMORY_MANAGEMENT
    }

    template<>
    consteval se_AllocatorInterface_t GetInterfaceForLabel<k_SavannaMemoryLabelHeap>()
    {
        return k_HeapAllocatorInterface;
    }

    template <std::size_t... I>
    consteval auto GenerateInterfaceArray(std::index_sequence<I...>)
    {
        return std::array
        {
            GetInterfaceForLabel<I>()...
        };
    }

    static constexpr auto k_LabelAllocatorInterfaces = GenerateInterfaceArray(std::make_index_sequence<static_cast<size_t>(k_SavannaMemoryLabelCount)>());

    const se_AllocatorInterface_t MemoryManager::GetAllocatorInterfaceForLabel(
        const MemoryLabel& label)
    {
        if (label >= k_SavannaMemoryLabelCount)
        {
            throw BadAllocationException();
        }
        return k_LabelAllocatorInterfaces[label.m_BackingValue];
    }

    const se_AllocatorInterface_t* MemoryManager::GetAllocatorInterfaceForLabelPtr(const MemoryLabel &label)
    {
        if (label >= k_SavannaMemoryLabelCount)
        {
            return nullptr;
        }
        return &k_LabelAllocatorInterfaces[label.m_BackingValue];
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
    MemoryManager::MemoryManager()
        : m_MemoryArenas(0, k_SavannaMemoryLabelHeap)
    {}

    MemoryManager::~MemoryManager() {}

    void* MemoryManager::Allocate(size_t size, const se_MemoryLabelBackingInt_t label)
    {
        return Allocate(size, 1, label);
    }

    void* MemoryManager::Allocate(size_t size, size_t alignment, const se_MemoryLabelBackingInt_t label)
    {
        if (label == k_SavannaMemoryLabelHeap)
        {
            return k_HeapAllocatorInterface.m_AllocAlignedFunc(size, alignment, nullptr);
        }

        auto arenaId = SavannaMemoryGetArenaIdFromLabel(label);
        if (arenaId == k_SavannaMemoryArenaIdInvalid)
        {
            throw BadAllocationException();
        }

        return m_MemoryArenas[arenaId].alloc(size, alignment);
    }

    void* MemoryManager::Reallocate(
        void *ptr,
        size_t newSize,
        const se_MemoryLabelBackingInt_t label)
    {
        return Reallocate(ptr, newSize, 1, label);
    }

    void* MemoryManager::Reallocate(
        void* ptr,
        size_t newSize,
        size_t alignment,
        const se_MemoryLabelBackingInt_t label)
    {
        if (label == k_SavannaMemoryLabelHeap)
        {
            return k_HeapAllocatorInterface.m_ReallocAlignedFunc(ptr, alignment, newSize, nullptr);
        }

        auto arenaId = SavannaMemoryGetArenaIdFromLabel(label);
        if (arenaId == k_SavannaMemoryArenaIdInvalid)
        {
            throw BadAllocationException();
        }

        return m_MemoryArenas[arenaId].realloc(ptr, newSize, alignment);
    }

    void MemoryManager::Free(void* ptr, const se_MemoryLabelBackingInt_t label)
    {
        // TODO @DavidMohrhardt: This is a hack to get the heap allocator working.
        //      Remove this once MemoryArena's are implemented.
        if (label == k_SavannaMemoryLabelHeap)
        {
            return k_HeapAllocatorInterface.m_FreeFunc(ptr, nullptr);
        }

        auto arenaId = SavannaMemoryGetArenaIdFromLabel(label);
        if (arenaId == k_SavannaMemoryArenaIdInvalid)
        {
            throw BadAllocationException();
        }

        m_MemoryArenas[arenaId].free(ptr, 1);
    }

    bool MemoryManager::InitializeInternal()
    {
        m_MemoryArenas.clear();
        for (int i = 0; i < k_SavannaMemoryArenaIdCount; ++i)
        {
            m_MemoryArenas.push_back(std::move(AtomicMultiListAllocator(4, sizeof(UnifiedPage4096KiB))));
        }
        return true;
    }

    void MemoryManager::StartInternal() {}

    void MemoryManager::StopInternal() {}

    void MemoryManager::ShutdownInternal()
    {
        m_MemoryArenas.clear();
    }
} // namespace Savanna

#if ENABLE_OPERATOR_NEW_DELETE_OVERRIDES

// Operator new and delete overrides
void *operator new(size_t sz)
{
    return Savanna::MemoryManager::Get()->Allocate(sz);
}

void* operator new(size_t sz, const uint32_t& label)
{
    return Savanna::MemoryManager::Get()->Allocate(sz, label);
}

void *operator new[](size_t sz)
{
    return Savanna::MemoryManager::Get()->Allocate(sz);
}

void operator delete(void *ptr) noexcept
{
    Savanna::MemoryManager::Get()->Free(ptr);
}

void operator delete(void *ptr, size_t size) noexcept
{
    Savanna::MemoryManager::Get()->Free(ptr);
}

void operator delete[](void *ptr) noexcept
{
    Savanna::MemoryManager::Get()->Free(ptr);
}

void operator delete[](void *ptr, size_t size) noexcept
{
    Savanna::MemoryManager::Get()->Free(ptr);
}

#endif // ENABLE_OPERATOR_NEW_DELETE_OVERRIDES


// C-Api
SAVANNA_EXPORT(const se_AllocatorInterface_t) SavannaMemoryManagerGetAllocatorInterfaceForLabel(const se_uint32 &label)
{
    return Savanna::MemoryManager::GetAllocatorInterfaceForLabel(label);
}

SAVANNA_EXPORT(const se_AllocatorInterface_t) SavannaMemoryManagerGetDefaultAllocatorInterface()
{
    return Savanna::MemoryManager::GetAllocatorInterfaceForLabel(k_SavannaMemoryLabelGeneral);
}

SAVANNA_EXPORT(bool) SavannaMemoryManagerTryGetAllocatorInterfaceForLabel(const se_uint32& label, se_AllocatorInterface_t& outLabelInterface)
{
    return Savanna::MemoryManager::TryGetAllocatorInterfaceForLabel(label, outLabelInterface);
}
