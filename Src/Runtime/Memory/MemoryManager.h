#pragma once

#include <Utilities/SavannaCoding.h>

#include "MemoryLabel.h"

#include "Types/Manager/GlobalManager.h"
#include "Types/Memory/MemoryBlocks.h"
#include "Types/Memory/ExpandableBlockAllocator.h"

#define ENABLE_GLOBAL_NEW_DELETE_OP_OVERRIDES 0
#define ENABLE_MEMORY_MANAGEMENT 1

#if defined(__cplusplus)

#include "Types/Memory/Allocators.h"

namespace Savanna
{
    class MemoryManager final : public GlobalManager<MemoryManager>
    {
    private:
        DEFINE_GLOBAL_MANAGER_FRIENDS_FOR(MemoryManager);

        AtomicExpandableBlockAllocator m_RootAllocator;

        MemoryManager();
        ~MemoryManager();

    public:
        void* Allocate(size_t size, const MemoryLabel label = SE_MEMORY_LABEL_GENERAL);
        void* Allocate(size_t size, size_t alignment, const MemoryLabel label = SE_MEMORY_LABEL_GENERAL);

        void Free(void* ptr);

        template <typename T, typename ...ARGS>
        inline T* Allocate(const MemoryLabel label, ARGS&&... args)
        {
            return new (static_cast<T*>(Allocate(sizeof(T), label))) T(std::forward<ARGS>(args)...);
        }

        template <typename T>
        inline T* AllocateArray(const size_t& count, const MemoryLabel label)
        {
            return static_cast<T*>(Allocate(sizeof(T) * count, label));
        }

        template <typename T>
        inline void Free(T* ptr)
        {
            ptr->~T();
            Free(static_cast<void*>(ptr));
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

#define SAVANNA_NEW(type, ...) Savanna::MemoryManager::Get().Allocate<type>(SE_MEMORY_LABEL_GENERAL, __VA_ARGS__)
#define SAVANNA_NEW_ARRAY(type, count) Savanna::MemoryManager::Get().AllocateArray<type>(SE_MEMORY_LABEL_GENERAL, count)
#define SAVANNA_DELETE(ptr) Savanna::MemoryManager::Get().Free(ptr)
#define SAVANNA_DELETE_ARRAY(ptr) Savanna::MemoryManager::Get().Free(ptr)

#define SAVANNA_INPLACE_NEW(type, ptr, ...) new (ptr) type(__VA_ARGS__)
#define SAVANNA_INPLACE_NEW_ARRAY(type, count, ptr) new (ptr) type[count]

#define SAVANNA_MALLOC(size) Savanna::MemoryManager::Get().Allocate(size, SE_MEMORY_LABEL_GENERAL)
#define SAVANNA_MALLOC_ALIGNED(size, alignment) Savanna::MemoryManager::Get().Allocate(size, alignment, SE_MEMORY_LABEL_GENERAL)
#define SAVANNA_FREE(ptr) Savanna::MemoryManager::Get().Free(ptr)

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

#endif

extern "C" {
#endif // defined(__cplusplus)

void* savanna_memory_manager_allocate(size_t size, size_t alignment = 0, se_MemoryLabel_t label = SE_MEMORY_LABEL_GENERAL);

void savanna_memory_manager_free(void* ptr);

void* savanna_memory_manager_realloc(void* ptr, size_t alignment, const size_t& originalSize, const size_t& newSize);

#if defined(__cplusplus)
} // extern "C"

#endif

