#include "SavannaMemory.h"
#include "MemoryManager.h"

#include "Profiling/Profiler.h"

#include <array>
#include <cstring>
#include "Public/ISavannaMemory.h"

#define ENABLE_MEMORY_MANAGEMENT 1

namespace savanna
{
    template <seAllocatorKindBackingInt LABEL>
    consteval seAllocatorInterface GetInterfaceForAllocatorKind()
    {
#if !ENABLE_MEMORY_MANAGEMENT
        return k_HeapAllocatorInterface;
#else // ENABLE_MEMORY_MANAGEMENT
        return seAllocatorInterface
        {
            .m_AllocFunc = [](size_t size, void* pUserData) -> void*
            {
                return MemoryManager::Get()->Allocate(size, LABEL);
            },
            .m_AllocAlignedFunc = [](size_t size, size_t alignment, void* pUserData) -> void*
            {
                return MemoryManager::Get()->AllocateAligned(size, alignment, LABEL);
            },
            .m_ReallocFunc = [](void* ptr, const size_t& newSize, void* pUserData) -> void*
            {
                return MemoryManager::Get()->Reallocate(ptr, newSize, LABEL);
            },
            .m_ReallocAlignedFunc = [](void* ptr, size_t alignment, const size_t& newSize, void* pUserData) -> void*
            {
                return MemoryManager::Get()->ReallocateAligned(ptr, newSize, alignment, LABEL);
            },
            .m_FreeFunc = [](void* ptr, void* pUserData) -> void
            {
                MemoryManager::Get()->Free(ptr, LABEL);
            }
        };
#endif // !ENABLE_MEMORY_MANAGEMENT
    }

    template<>
    consteval seAllocatorInterface GetInterfaceForAllocatorKind<kSavannaAllocatorKindHeap>()
    {
        return k_HeapAllocatorInterface;
    }

    template <std::size_t... I>
    consteval auto GenerateInterfaceArray(std::index_sequence<I...>)
    {
        return std::array
        {
            GetInterfaceForAllocatorKind<I>()...
        };
    }

    static constexpr auto k_AllocatorKindAllocatorInterfaces = GenerateInterfaceArray(std::make_index_sequence<static_cast<size_t>(kSavannaAllocatorKindCount)>());

    const seAllocatorInterface MemoryManager::GetAllocatorInterfaceForAllocatorKind(
        const AllocatorKind& allocatorKind)
    {
        SAVANNA_DEBUG_ASSERT(allocatorKind < kSavannaAllocatorKindCount, "Invalid Allocation Kind!");
        return k_AllocatorKindAllocatorInterfaces[allocatorKind];
    }

    const seAllocatorInterface* MemoryManager::GetAllocatorInterfaceForAllocatorKindPtr(const AllocatorKind &allocatorKind)
    {
        if (allocatorKind >= kSavannaAllocatorKindCount)
        {
            return nullptr;
        }
        return &k_AllocatorKindAllocatorInterfaces[allocatorKind];
    }

    bool MemoryManager::TryGetAllocatorInterfaceForAllocatorKind(
        const uint32& allocatorKind,
        seAllocatorInterface& outAllocatorKindInterface)
    {
        if (allocatorKind >= kSavannaAllocatorKindCount)
        {
            return false;
        }

        outAllocatorKindInterface = k_AllocatorKindAllocatorInterfaces[allocatorKind];
        return true;
    }

    // TODO @DavidMohrhardt: Add initialization of memory manager based on a provided boot configuration.
    MemoryManager::MemoryManager()
        : m_RootMemoryAllocator()
        , m_HeapAllocator{k_HeapAllocatorInterface}
        , m_pAllocators{}
    {
        m_pAllocators[kSavannaAllocatorKindHeap] = static_cast<Allocator*>(&m_HeapAllocator);
    }

    MemoryManager::~MemoryManager() {}

    void* MemoryManager::Allocate(
        const size_t &size,
        const seAllocatorKindBackingInt allocatorKind,
        const DebugAllocationInfo *pDbgInfo)
    {
        return AllocateInternal(size, alignof(byte), allocatorKind, pDbgInfo);
    }

    void* MemoryManager::AllocateAligned(
        const size_t& size,
        const size_t& alignment,
        const seAllocatorKindBackingInt allocatorKind,
        const DebugAllocationInfo *pDbgInfo)
    {
        return AllocateInternal(size, alignment, allocatorKind, pDbgInfo);
    }

    void* MemoryManager::Reallocate(
        void* ptr,
        const size_t& newSize,
        const seAllocatorKindBackingInt allocatorKind,
        const DebugAllocationInfo *pDbgInfo)
    {
        return ReallocateInternal(ptr, newSize, alignof(byte), allocatorKind, pDbgInfo);
    }

    void* MemoryManager::ReallocateAligned(
        void* ptr,
        const size_t& newSize,
        const size_t& alignment,
        const seAllocatorKindBackingInt allocatorKind,
        const DebugAllocationInfo *pDbgInfo)
    {
        return ReallocateInternal(ptr, newSize, alignment, allocatorKind, pDbgInfo);
    }

    void MemoryManager::Free(void* ptr, const seAllocatorKindBackingInt allocatorKind)
    {
        m_pAllocators[allocatorKind]->free(ptr, 1);
    }

    inline void* MemoryManager::AllocateInternal(
        const size_t& size,
        const size_t& alignment,
        const seAllocatorKindBackingInt allocatorKind,
        const DebugAllocationInfo *pDbgInfo)
    {
        return m_pAllocators[allocatorKind]->alloc(size, alignof(byte));
    }

    inline void* MemoryManager::ReallocateInternal(
        void* ptr,
        const size_t& newSize,
        const size_t& alignment,
        const seAllocatorKindBackingInt allocatorKind,
        const DebugAllocationInfo *pDbgInfo)
    {
        if (allocatorKind >= kSavannaAllocatorKindCount)
        {
            return nullptr;
        }

        return m_pAllocators[allocatorKind]->realloc(ptr, newSize, alignof(byte));
    }

    bool MemoryManager::InitializeInternal()
    {
        m_RootMemoryAllocator = std::move(AtomicMultiListAllocator(6, sizeof(UnifiedPage4096KiB)));
        m_pAllocators[kSavannaAllocatorKindPersistent] = &m_RootMemoryAllocator;
        for (int i = 0; i < kSavannaAllocatorKindCount; ++i)
        {
            Allocator* pAllocator = nullptr;
            switch(i)
            {
                case kSavannaAllocatorKindGeneral:
                    pAllocator = m_RootMemoryAllocator.New<AtomicMultiListAllocator>(4, sizeof(UnifiedPage4096KiB), kSavannaAllocatorKindPersistent);
                    break;
                case kSavannaAllocatorKindTemp:
                    pAllocator = m_RootMemoryAllocator.New<FixedAllocator>(sizeof(UnifiedPage32KiB), kSavannaAllocatorKindGeneral);
                    break;
                case kSavannaAllocatorKindThreadSafeTemp:
                    // TODO @DavidMohrhardt: Make this an specialized atomic block allocator so we can count frame lifespan of allocations using a mask.
                    pAllocator = m_RootMemoryAllocator.New<AtomicFixedAllocator>(sizeof(UnifiedPage64KiB), kSavannaAllocatorKindGeneral);
                    break;
                case kSavannaAllocatorKindHeap:
                case kSavannaAllocatorKindPersistent:
                default:
                    continue;
            }
            m_pAllocators[i] = pAllocator;
        }

        return true;
    }

    void MemoryManager::StartInternal() {}

    void MemoryManager::StopInternal() {}

    void MemoryManager::ShutdownInternal()
    {
        constexpr size_t k_FreeableAllocatorCount = kSavannaAllocatorKindCount - 2;
        constexpr seAllocatorKind k_AllocatorDeletionOrder[kSavannaAllocatorKindCount - 2]
        {
            kSavannaAllocatorKindTemp,
            kSavannaAllocatorKindThreadSafeTemp,
            kSavannaAllocatorKindGeneral
        };
        for (int i = 0; i < k_FreeableAllocatorCount; ++i)
        {
            m_RootMemoryAllocator.Delete(m_pAllocators[k_AllocatorDeletionOrder[i]]);
            m_pAllocators[k_AllocatorDeletionOrder[i]] = nullptr;
        }

        m_pAllocators[kSavannaAllocatorKindPersistent] = nullptr;
        m_RootMemoryAllocator = std::move(AtomicMultiListAllocator());
    }

    void MemoryManager::OnFrameEnd()
    {
        if (auto pTempAllocator = static_cast<FixedAllocator*>(m_pAllocators[kSavannaAllocatorKindTemp]))
        {
            pTempAllocator->Reset();
        }
        if (auto pThreadTempAllocator = static_cast<AtomicFixedAllocator*>(m_pAllocators[kSavannaAllocatorKindThreadSafeTemp]))
        {
            pThreadTempAllocator->GetSafeAllocator().Get().Reset();
        }
    }
} // namespace Savanna

// C-Api
SAVANNA_EXPORT(const seAllocatorInterface) SavannaMemoryManagerGetAllocatorInterfaceForAllocatorKind(const seAllocatorKindBackingInt& allocatorKind)
{
    return savanna::MemoryManager::GetAllocatorInterfaceForAllocatorKind((seAllocatorKind)allocatorKind);
}

SAVANNA_EXPORT(const seAllocatorInterface) SavannaMemoryManagerGetDefaultAllocatorInterface()
{
    return savanna::MemoryManager::GetAllocatorInterfaceForAllocatorKind(kSavannaAllocatorKindGeneral);
}

SAVANNA_EXPORT(bool) SavannaMemoryManagerTryGetAllocatorInterfaceForAllocatorKind(const se_uint32& allocatorKind, seAllocatorInterface& outAllocatorKindInterface)
{
    return savanna::MemoryManager::TryGetAllocatorInterfaceForAllocatorKind(allocatorKind, outAllocatorKindInterface);
}

SAVANNA_EXPORT(void*) SavannaMemoryManagerAllocateAligned(
    size_t size,
    size_t alignment,
    seAllocatorKind allocatorKind,
    const char* fileName,
    const int lineNo)
{
#if SAVANNA_ENABLE_RUNTIME_MEMORY_VALIDATION
    savanna::MemoryManager::DebugAllocationInfo info{fileName, lineNo};
    return savanna::MemoryManager::Get()->AllocateAligned(size, alignment, allocatorKind, &info);
#else
    return savanna::MemoryManager::Get()->AllocateAligned(size, alignment, allocatorKind, nullptr);
#endif
}

SAVANNA_EXPORT(void) SavannaMemoryManagerFree(void* ptr, seAllocatorKind allocatorKind)
{
    return savanna::MemoryManager::Get()->Free(ptr, allocatorKind);
}

SAVANNA_EXPORT(void *)
SavannaMemoryManagerReallocateAligned(
    void *ptr, size_t size, size_t alignment,
    seAllocatorKind allocatorKind,
    const char *fileName, const int lineNo)
{
#if SAVANNA_ENABLE_RUNTIME_MEMORY_VALIDATION
    savanna::MemoryManager::DebugAllocationInfo info{fileName, lineNo};
    return savanna::MemoryManager::Get()->ReallocateAligned(ptr, size, alignment, allocatorKind, &info);
#else
    return savanna::MemoryManager::Get()->ReallocateAligned(ptr, size, alignment, allocatorKind, nullptr);
#endif
}
