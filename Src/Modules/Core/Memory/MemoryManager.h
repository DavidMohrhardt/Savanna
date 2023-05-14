#pragma once

#include <SavannaCoreConfiguration.h>
#include <Utilities/SavannaCoding.h>

#include <Types/Singleton/EmbeddedSingleton.h>
#include <Types/Memory/ExpandableBlockAllocator.h>

namespace Savanna
{
    static constexpr size_t k_DefaultPlatformAlignment = std::max(alignof(void*), alignof(std::max_align_t));

    class MemoryManager : public EmbeddedSingleton<MemoryManager>
    {
    private:
        friend class EmbeddedSingleton<MemoryManager>;

        ExpandableBlockAllocator m_CoreAllocator;
        bool m_Initialized;

    private:
        MemoryManager();
        MemoryManager(MemoryManager &other);
        MemoryManager(MemoryManager &&other);

        ~MemoryManager();

        void InitializeInternal();
        void ShutdownInternal();

    public:
        bool IsInitialized() const;

        void* Allocate(size_t size, size_t alignment = k_DefaultPlatformAlignment);
        void Free(void* p, size_t alignment = k_DefaultPlatformAlignment);

    public:
        static void Initialize();
        static void Shutdown();
    };
}

#if SAVANNA_OVERLOAD_GLOBAL_NEW_DELETE
    void* operator new(size_t size) { return Savanna::MemoryManager::Get().Allocate(size); }
    void* operator new[](size_t size) { return Savanna::MemoryManager::Get().Allocate(size); }

    void* operator new(size_t size, std::align_val_t alignment) { return Savanna::MemoryManager::Get().Allocate(size, static_cast<size_t>(alignment)); }
    void* operator new[](size_t size, std::align_val_t alignment) { return Savanna::MemoryManager::Get().Allocate(size, static_cast<size_t>(alignment)); }

    void operator delete(void* p) SAVANNA_NOEXCEPT { Savanna::MemoryManager::Get().Free(p); }
    void operator delete[](void* p) SAVANNA_NOEXCEPT { Savanna::MemoryManager::Get().Free(p); }

    void operator delete(void* p, std::align_val_t alignment) SAVANNA_NOEXCEPT { Savanna::MemoryManager::Get().Free(p, static_cast<size_t>(alignment)); }
#endif
