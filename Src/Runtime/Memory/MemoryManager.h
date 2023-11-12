#pragma once

#include <Utilities/SavannaCoding.h>

#include "SavannaMemory.h"

#include "Types/Containers/Arrays/dynamic_array.h"
#include "Types/Manager/GlobalManager.h"
#include "Types/Memory/MemoryBlocks.h"

#define ENABLE_GLOBAL_NEW_DELETE_OP_OVERRIDES 0

#if defined(__cplusplus)
#include <new>

#include "Types/Memory/Allocators.h"

namespace Savanna
{
    class MemoryManager final : public GlobalManager<MemoryManager>
    {
    private:
        DEFINE_GLOBAL_MANAGER_FRIENDS_FOR(MemoryManager);

    public:
        struct DebugAllocationInfo
        {
            const char* m_FileName;
            const int m_LineNo;
        };

        static const se_AllocatorInterface_t GetAllocatorInterfaceForAllocatorKind(const AllocatorKind& allocatorKind);
        static const se_AllocatorInterface_t* GetAllocatorInterfaceForAllocatorKindPtr(const AllocatorKind& allocatorKind);
        static bool TryGetAllocatorInterfaceForAllocatorKind(const uint32& allocatorKind, se_AllocatorInterface_t& outAllocatorKindInterface);

    private:
        AtomicMultiListAllocator m_RootMemoryAllocator;
        InterfaceAllocator m_HeapAllocator;
        Allocator* m_pAllocators[kSavannaAllocatorKindCount];

        MemoryManager();
        ~MemoryManager();

    public:
        void* Allocate(
            const size_t& size,
            const se_AllocatorKindBackingInt_t allocatorKind = kSavannaAllocatorKindGeneral,
            const DebugAllocationInfo* pDbgInfo = nullptr);

        void* AllocateAligned(
            const size_t& size,
            const size_t& alignment,
            const se_AllocatorKindBackingInt_t allocatorKind = kSavannaAllocatorKindGeneral,
            const DebugAllocationInfo* pDbgInfo = nullptr);

        void* Reallocate(
            void* ptr,
            const size_t& newSize,
            const se_AllocatorKindBackingInt_t allocatorKind = kSavannaAllocatorKindGeneral,
            const DebugAllocationInfo* pDbgInfo = nullptr);

        void* ReallocateAligned(
            void* ptr,
            const size_t& newSize,
            const size_t& alignment,
            const se_AllocatorKindBackingInt_t allocatorKind = kSavannaAllocatorKindGeneral,
            const DebugAllocationInfo* pDbgInfo = nullptr);

        void Free(void* ptr, const se_AllocatorKindBackingInt_t allocatorKind = kSavannaAllocatorKindGeneral);

        void OnFrameEnd();

    protected:
        void* AllocateInternal(
            const size_t& size,
            const size_t& alignment,
            const se_AllocatorKindBackingInt_t allocatorKind,
            const DebugAllocationInfo* pDbgInfo = nullptr);

        void* ReallocateInternal(
            void* ptr,
            const size_t& newSize,
            const size_t& alignment,
            const se_AllocatorKindBackingInt_t allocatorKind,
            const DebugAllocationInfo* pDbgInfo = nullptr);

        bool InitializeInternal() override;
        void StartInternal() override;
        void StopInternal() override;
        void ShutdownInternal() override;
    };
} // namespace Savanna

#endif // defined(__cplusplus)
