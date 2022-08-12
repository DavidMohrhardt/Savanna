#include "MemoryArena.h"
#include "CacheLine.h"
#include "Allocators/AllocatorUtils.h"
#include "Types/Exceptions/SavannaException.h"
#include "Types/Pointers/PointerUtilities.h"

namespace Savanna
{
    MemoryArena::MemoryArena(size_t initialMemoryRequest)
        : m_Size(initialMemoryRequest)
        , m_Root(malloc(initialMemoryRequest))
        , m_Allocated(0)
        , m_Head(m_Root)
    {
        if (m_Root == nullptr)
        {
            throw RuntimeErrorException("Failed to allocate memory for MemoryArena.");
        }
    }

    MemoryArena::~MemoryArena()
    {
        if (m_Root != nullptr)
        {
            free(m_Root);
        }
    }

    SAVANNA_NO_DISCARD void* MemoryArena::AcquireMemory(size_t size)
    {
        if (size > GetMaxSize() || size == 0)
        {
            return nullptr;
        }

        void* ptr = m_Head;
        m_Head = GetForwardAlignedPtr<void, void>(Add(ptr, size), L1CacheLineLength());

        if (m_Head == nullptr)
        {
            throw RuntimeErrorException("MemoryArena::AcquireMemory - Failed to allocate memory.");
        }

        m_Allocated = m_Allocated + size;
        if (m_Allocated >= m_Size)
        {
            m_Head = nullptr;
        }

        return ptr;
    }

    void MemoryArena::ReleaseMemory(void* ptr)
    {
        // TODO: Implement
    }
} // namespace Savanna
