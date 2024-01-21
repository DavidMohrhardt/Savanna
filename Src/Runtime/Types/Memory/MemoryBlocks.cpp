#include "MemoryBlocks.h"

#include <map>

#if defined(__cplusplus)

namespace savanna
{
    template <typename T>
    static void* AllocatePages(size_t count)
    {
        return malloc(count * sizeof(T));
    }

    // Create functions for allocating unified page memory
    const std::map<size_t, pfn_sePageAlloc> k_PageAllocFuncs {
        #define PAGE_ALLOC_FUNC_ENTRY(pageSize) { pageSize, AllocatePages<UnifiedPage##pageSize##KiB> }
        { 1, AllocatePages<sePage1KiB> },
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

pfn_sePageAlloc GetPageAllocForSize(size_t size)
{
    auto it = savanna::k_PageAllocFuncs.find(size);
    if (it != savanna::k_PageAllocFuncs.end())
    {
        return it->second;
    }
    return nullptr;
}
