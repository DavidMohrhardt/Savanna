#include "MemoryBlocks.h"

#include <unordered_map>

namespace Savanna
{
    template <typename T>
    static void* AllocatePages(size_t count)
    {
        return malloc(count * sizeof(T));
    }

    // Create functions for allocating unified page memory
    constexpr std::unordered_map<size_t, PageAllocFunc> m_PageAllocFuncs {
        #define PAGE_ALLOC_FUNC_ENTRY(pageSize) { pageSize, AllocatePages<UnifiedPage##pageSizeKiB> }
        PAGE_ALLOC_FUNC_ENTRY(1),
        PAGE_ALLOC_FUNC_ENTRY(2),
        PAGE_ALLOC_FUNC_ENTRY(4),
        PAGE_ALLOC_FUNC_ENTRY(8),
        PAGE_ALLOC_FUNC_ENTRY(16),
        PAGE_ALLOC_FUNC_ENTRY(32),
        PAGE_ALLOC_FUNC_ENTRY(64),
        PAGE_ALLOC_FUNC_ENTRY(128),
        PAGE_ALLOC_FUNC_ENTRY(256),
        PAGE_ALLOC_FUNC_ENTRY(512),
        PAGE_ALLOC_FUNC_ENTRY(1024),
        PAGE_ALLOC_FUNC_ENTRY(2048),
        PAGE_ALLOC_FUNC_ENTRY(4096),
        #undef PAGE_ALLOC_FUNC_ENTRY
    };
} // namespace Savanna

#endif // __cplusplus

se_PageAllocFunc GetPageAllocFuncForSize(size_t size)
{
    auto it = m_PageAllocFuncs.find(size);
    if (it != m_PageAllocFuncs.end())
    {
        return it->second;
    }
    return nullptr;
}
