#pragma once

#include <Utilities/SavannaCoding.h>

#include "MemoryLabel.h"

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
        static const se_AllocatorInterface_t GetAllocatorInterfaceForLabel(const MemoryLabel& label);
        static const se_AllocatorInterface_t* GetAllocatorInterfaceForLabelPtr(const MemoryLabel& label);
        static bool TryGetAllocatorInterfaceForLabel(const uint32& label, se_AllocatorInterface_t& outLabelInterface);

    private:
        dynamic_array<AtomicMultiListAllocator> m_MemoryArenas;

        MemoryManager();
        ~MemoryManager();

    public:
        void* Allocate(
            size_t size,
            const se_MemoryLabelBackingInt_t label = k_SavannaMemoryLabelGeneral);

        void* AllocateAligned(
            const size_t& size,
            const size_t& alignment,
            const se_MemoryLabelBackingInt_t label = k_SavannaMemoryLabelGeneral);

        void* Reallocate(
            void* ptr,
            const size_t& newSize,
            const se_MemoryLabelBackingInt_t label = k_SavannaMemoryLabelGeneral);

        void* ReallocateAligned(
            void* ptr,
            const size_t& newSize,
            size_t alignment,
            const se_MemoryLabelBackingInt_t label = k_SavannaMemoryLabelGeneral);

        void Free(void* ptr, const se_MemoryLabelBackingInt_t label = k_SavannaMemoryLabelGeneral);

        template <typename T, typename ...ARGS>
        inline T* New(const se_MemoryLabelBackingInt_t label, ARGS&&... args)
        {
            return new (static_cast<T*>(AllocateAligned(sizeof(T), alignof(T), label))) T(std::forward<ARGS>(args)...);
        }

        template <typename T>
        inline T* NewArray(const size_t& count, const se_MemoryLabelBackingInt_t label)
        {
            return new (static_cast<T*>(AllocateAligned(sizeof(T) * count, alignof(T), label))) T[count];
        }

        template <typename T>
        inline void Delete(T* ptr, const se_MemoryLabelBackingInt_t label)
        {
            ptr->~T();
            Free(static_cast<void*>(ptr), label);
        }

        template <typename T>
        inline void DeleteArray(T* ptr, const size_t& count, const se_MemoryLabelBackingInt_t label)
        {
            for (size_t i = 0; i < count; ++i)
            {
                ptr[i].~T();
            }
            Free(static_cast<void*>(ptr), label);
        }

    protected:
        void* AllocateInternal(
            const size_t& size,
            const size_t& alignment,
            const se_MemoryLabelBackingInt_t label);

        void* ReallocateInternal(
            void* ptr,
            const size_t& newSize,
            const size_t& alignment,
            const se_MemoryLabelBackingInt_t label);

        bool InitializeInternal() override;
        void StartInternal() override;
        void StopInternal() override;
        void ShutdownInternal() override;
    };
} // namespace Savanna

// Override new and delete operators for to use the MemoryManager
#if ENABLE_GLOBAL_NEW_DELETE_OP_OVERRIDES

void* operator new(size_t sz);
void* operator new[](size_t sz);
void operator delete(void* ptr) noexcept;
void operator delete(void* ptr, size_t size) noexcept;
void operator delete[](void* ptr) noexcept;
void operator delete[](void* ptr, size_t size) noexcept;

#endif // ENABLE_GLOBAL_NEW_DELETE_OP_OVERRIDES

#endif // defined(__cplusplus)
