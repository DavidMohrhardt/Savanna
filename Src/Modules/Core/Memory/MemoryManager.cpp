#include "MemoryManager.h"


#define SAVANNA_PLATFORM_DEFAULT_MEMORY_BLOCK_COUNT 16
#define SAVANNA_PLATFORM_DEFAULT_MEMORY_BLOCK_SIZE_IN_KB 64
#define SAVANNA_PLATFORM_DEFAULT_MEMORY_BLOCK_SIZE (SAVANNA_PLATFORM_DEFAULT_MEMORY_BLOCK_SIZE_IN_KB * 1024)

namespace Savanna
{
    static constexpr size_t k_DefaultCoreMemoryBlockCount = SAVANNA_PLATFORM_DEFAULT_MEMORY_BLOCK_COUNT;
    static constexpr size_t k_DefaultCoreMemoryBlockSize = SAVANNA_PLATFORM_DEFAULT_MEMORY_BLOCK_SIZE;

    MemoryManager::MemoryManager()
    {
#if !SAVANNA_PLATFORM_INITIALIZES_MEMORY_MANAGER_MANUALLY
        InitializeInternal();
#endif
    }

    MemoryManager::~MemoryManager() {}

    bool MemoryManager::IsInitialized() const
    {
#if SAVANNA_PLATFORM_ALLOW_LAZY_MEMORY_INITIALIZATION
        static auto _LazyInitializationSentinel = [this]() { this->InitializeInternal(); return true; }();
#endif
        return m_Initialized;
     }

    void MemoryManager::InitializeInternal()
    {
        if (m_Initialized)
            return;

#if SAVANNA_ENABLE_MEMORY_MANAGER_HEURISTIC
        // TODO - Generate a heuristic for the memory manager to allocate a reasonable amount of memory for a given platform
#else

#endif

        m_Initialized = true;
    }

    void MemoryManager::ShutdownInternal()
    {
        if (!m_Initialized)
            return;

#if SAVANNA_PLATFORM_INITIALIZES_MEMORY_MANAGER_MANUALLY
        m_CoreAllocator = std::move(ExpandablePageAllocator(0, 0, true));
        m_Initialized = false;
#endif
    }

    void *MemoryManager::Allocate(size_t size, size_t alignment)
    {
#if SAVANNA_PLATFORM_INITIALIZES_MEMORY_MANAGER_MANUALLY
        if (!IsInitialized())
            return nullptr;
#endif

        return m_CoreAllocator.alloc(size, alignment);
    }

    void MemoryManager::Free(void *p, size_t alignment)
    {
#if SAVANNA_PLATFORM_INITIALIZES_MEMORY_MANAGER_MANUALLY
        if (!IsInitialized())
            return nullptr;
#endif

            m_CoreAllocator.free(p, alignment);
        }

    void MemoryManager::Initialize()
    {
#if SAVANNA_PLATFORM_INITIALIZES_MEMORY_MANAGER_MANUALLY
        MemoryManager::Get().InitializeInternal();
#endif
    }

    void MemoryManager::Shutdown()
    {
#if SAVANNA_PLATFORM_INITIALIZES_MEMORY_MANAGER_MANUALLY
        MemoryManager::Get().ShutdownInternal();
#endif
    }

} // namespace Savanna
