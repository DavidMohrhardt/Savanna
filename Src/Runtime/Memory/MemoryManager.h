#pragma once

#include <Utilities/SavannaCoding.h>

#include "MemoryLabel.h"

#include "Public/ISavannaMemory.h"

#include "Types/Containers/Arrays/DynamicArray.h"
#include "Types/Manager/GlobalManager.h"
#include "Types/Memory/MemoryBlocks.h"
#include "Types/Memory/ExpandableBlockAllocator.h"

#define ENABLE_GLOBAL_NEW_DELETE_OP_OVERRIDES 0
#define ENABLE_MEMORY_MANAGEMENT 1

#if defined(__cplusplus)
#include <new>

#include "Types/Memory/Allocators.h"

namespace Savanna
{
    class MemoryManager final : public GlobalManager<MemoryManager>
    {
    private:
        DEFINE_GLOBAL_MANAGER_FRIENDS_FOR(MemoryManager);

        AtomicExpandableBlockAllocator m_InitializationAllocator;
        DynamicArray<AtomicExpandableBlockAllocator> m_MemoryArenas;

        MemoryManager();
        ~MemoryManager();

    public:
        void* Allocate(size_t size, const se_uint32 label = SE_MEMORY_LABEL_GENERAL);
        void* Allocate(size_t size, size_t alignment, const se_uint32 label = SE_MEMORY_LABEL_GENERAL);
        void* Reallocate(void* ptr, size_t newSize, const se_uint32 label = SE_MEMORY_LABEL_GENERAL);
        void* Reallocate(void* ptr, size_t newSize, size_t alignment, const se_uint32 label = SE_MEMORY_LABEL_GENERAL);

        void Free(void* ptr, const se_uint32 label = SE_MEMORY_LABEL_GENERAL);

        template <typename T, typename ...ARGS>
        inline T* New(const se_uint32 label, ARGS&&... args)
        {
            return new (static_cast<T*>(Allocate(sizeof(T), label))) T(std::forward<ARGS>(args)...);
        }

        template <typename T>
        inline T* NewArray(const size_t& count, const se_uint32 label)
        {
            return new (static_cast<T*>(Allocate(sizeof(T) * count, label))) T[count];
        }

        template <typename T>
        inline void Delete(T* ptr)
        {
            ptr->~T();
            Free(static_cast<void*>(ptr));
        }

        template <typename T>
        inline void DeleteArray(T* ptr, const size_t& count, const se_uint32 label)
        {
            for (size_t i = 0; i < count; ++i)
            {
                ptr[i].~T();
            }
            Free(static_cast<void*>(ptr), label);
        }

    protected:
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

#if ENABLE_MEMORY_MANAGEMENT

#define SAVANNA_NEW(type, ...) \
    Savanna::MemoryManager::Get().New<type>(SE_MEMORY_LABEL_GENERAL, __VA_ARGS__)

#define SAVANNA_NEW_ARRAY(type, count) \
    Savanna::MemoryManager::Get().NewArray<type>(count, SE_MEMORY_LABEL_GENERAL)

#define SAVANNA_DELETE(ptr) \
    Savanna::MemoryManager::Get().Delete(ptr)

#define SAVANNA_DELETE_ARRAY(ptr, count) \
    Savanna::MemoryManager::Get().DeleteArray(ptr, count, SE_MEMORY_LABEL_GENERAL)

#define SAVANNA_INPLACE_NEW(type, ptr, ...) \
    new (ptr) type(__VA_ARGS__)

#define SAVANNA_INPLACE_NEW_ARRAY(type, count, ptr) \
    new (ptr) type[count]

#define SAVANNA_MALLOC(size) \
    Savanna::MemoryManager::Get().Allocate(size, SE_MEMORY_LABEL_GENERAL)

#define SAVANNA_MALLOC_ALIGNED(size, alignment) \
    Savanna::MemoryManager::Get().Allocate(size, alignment, SE_MEMORY_LABEL_GENERAL)

#define SAVANNA_FREE(ptr) \
    Savanna::MemoryManager::Get().Free(ptr)

#else

#define SAVANNA_NEW(type, ...) new type(__VA_ARGS__)
#define SAVANNA_NEW_ARRAY(type, count) new type[count]
#define SAVANNA_DELETE(ptr) delete ptr
#define SAVANNA_DELETE_ARRAY(ptr) delete[] ptr

#define SAVANNA_INPLACE_NEW(type, ptr, ...) new (ptr) type(__VA_ARGS__)
#define SAVANNA_INPLACE_NEW_ARRAY(type, count, ptr) new (ptr) type[count]

#define SAVANNA_MALLOC(size) malloc(size)
#define SAVANNA_MALLOC_ALIGNED(size, alignment) _aligned_malloc(size, alignment)
#define SAVANNA_FREE(ptr) free(ptr)

#endif // ENABLE_MEMORY_MANAGEMENT

#endif // defined(__cplusplus)
