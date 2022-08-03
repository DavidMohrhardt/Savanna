#include "MemoryArena.h"
#include "Allocators/Mallocator.h"
#include "Types/Exceptions/SavannaException.h"

namespace Savanna
{
    MemoryArena::MemoryArena(size_t initialMemoryRequest)
        : m_Size(initialMemoryRequest)
        , m_Root(nullptr)
    {}

    MemoryArena::~MemoryArena()
    {
        // RawMallocator::deallocate<byte>(m_Root, m_Size);
    }

    SAVANNA_NO_DISCARD void* MemoryArena::AcquireMemory(size_t size)
    {
        return nullptr;
    }

    void MemoryArena::ReleaseMemory(void* ptr, size_t size)
    {
        size_t offsetFromRoot = static_cast<size_t>(reinterpret_cast<intptr>(ptr) - reinterpret_cast<intptr>(m_Root));
        if (offsetFromRoot < 0 || offsetFromRoot > m_Size || size + offsetFromRoot > m_Size)
        {
            throw RuntimeErrorException("Pointer out of the arena bounds Memory Arena.");
        }


    }
} // namespace Savanna
