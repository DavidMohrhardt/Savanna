#include "MemoryPool.h"
#include "CacheLine.h"
#include "Allocators/AllocatorUtils.h"
#include "Math/PointerMath.h"
#include "Types/Exceptions/SavannaException.h"

namespace Savanna
{
    MemoryPool::MemoryPool(void* root, const size_t& size, AllocatorType allocatorType)
        : m_Size(size)
        , m_Root(root)
        , m_Allocated(0)
        , m_Allocator(root, size, allocatorType)
    {
        if (m_Root == nullptr)
        {
            throw RuntimeErrorException("Failed to allocate memory for MemoryPool.");
        }
    }

    MemoryPool::~MemoryPool()
    {
        if (m_Arena > MemoryArena::EngineArenaCount)
        {
            free(m_Root);
        }
    }

    SAVANNA_NO_DISCARD void* MemoryPool::AcquireMemory(size_t& size)
    {
        return m_Allocator.Allocate(size, k_DefaultAlignment);
    }

    void MemoryPool::ReleaseMemory(void* ptr)
    {
        m_Allocator.Free(ptr, k_DefaultAlignment);
    }
} // namespace Savanna
