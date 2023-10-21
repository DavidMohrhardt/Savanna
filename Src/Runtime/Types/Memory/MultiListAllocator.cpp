#include "MultiListAllocator.h"
#include "MemoryChunkDescriptors.h"

#include "Memory/MemoryManager.h"
#include "Profiling/Profiler.h"

namespace Savanna
{
    MultiListAllocator::MultiListAllocator(const MemoryLabel label)
        : Allocator()
        , m_MemoryLabel {label}
        , m_BufferBlockSize {0}
        , m_AllocatedBytes {0}
        , m_Size { 0 }
        , m_Pools(0, label)
    {}

    MultiListAllocator::MultiListAllocator(
        size_t initialBufferCount,
        size_t bufferBlockSize,
        const MemoryLabel label)
        : Allocator()
        , m_MemoryLabel {label}
        , m_BufferBlockSize { bufferBlockSize }
        , m_AllocatedBytes {0}
        , m_Size { 0 }
        , m_Pools(initialBufferCount > 0 ? initialBufferCount : 1, label)
    {
        // SAVANNA_INSERT_SCOPED_PROFILER(MultiListAllocator::ctor);
        SAVANNA_ASSERT(m_BufferBlockSize != 0, "Buffer block size must be greater than 0.");
        size_t bufferSize = m_BufferBlockSize;
        AllocateAdditionalMemoryBuffer(bufferSize * initialBufferCount);
    }

    MultiListAllocator& MultiListAllocator::operator=(MultiListAllocator &&other)
    {
        if (this != &other)
        {
            m_MemoryLabel = other.m_MemoryLabel;
            m_BufferBlockSize = other.m_BufferBlockSize;
            m_AllocatedBytes = other.m_AllocatedBytes;
            m_Size = other.m_Size;
            m_Pools = std::move(other.m_Pools);

            other.m_MemoryLabel = k_SavannaMemoryLabelNone;
            other.m_BufferBlockSize = 0;
            other.m_AllocatedBytes = 0;
            other.m_Size = 0;
        }
        return *this;
    }

    SAVANNA_NO_DISCARD void* MultiListAllocator::alloc(
        const size_t& size, const size_t& alignment)
    {
        // SAVANNA_INSERT_SCOPED_PROFILER(MultiListAllocator::alloc);
        FreeListAllocator& freeListAllocator = FindAllocatorForSize(size);
        return freeListAllocator.alloc(size, alignment);
    }

    void MultiListAllocator::free(
        void* const ptr, const size_t& alignment)
    {
        // SAVANNA_INSERT_SCOPED_PROFILER(MultiListAllocator::free);
        FreeListAllocator& owningAllocator = FindPointerInPools(ptr);
        owningAllocator.free(ptr, alignment);
    }

    void* MultiListAllocator::realloc(
        void* const ptr, const size_t& newSize, const size_t& alignment)
    {
        // SAVANNA_INSERT_SCOPED_PROFILER(MultiListAllocator::realloc);
        FreeListAllocator& owningAllocator = FindPointerInPools(ptr);
        if (owningAllocator.GetSize() - owningAllocator.GetAllocatedBytes() >= newSize)
        {
            return owningAllocator.realloc(ptr, newSize, alignment);
        }
        else
        {
            auto allocationDescriptor = owningAllocator.GetAllocationDescriptor(ptr);
            FreeListAllocator& secondaryAllocator = FindAllocatorForSize(newSize);
            void* pBuffer = secondaryAllocator.alloc(newSize, alignment);
            memcpy(pBuffer, ptr, allocationDescriptor.m_Size);
            owningAllocator.free(ptr, alignment);
            return pBuffer;
        }
        return nullptr;
    }

    void MultiListAllocator::AllocateAdditionalMemoryBuffer(size_t size)
    {
        // SAVANNA_INSERT_SCOPED_PROFILER(MultiListAllocator::AcquireNewBuffer);

        size_t bufferSize = m_BufferBlockSize + sizeof(MemoryChunkDescriptor);
        while (bufferSize < size)
        {
            bufferSize *= 2;
        }

        m_Pools.push_back(std::move(MemoryPool(bufferSize, m_MemoryLabel)));
        m_Size += bufferSize;
    }

    inline FreeListAllocator &MultiListAllocator::FindPointerInPools(void *const ptr)
    {
        for (auto& pool : m_Pools)
        {
            FreeListAllocator& freeListAllocator = pool.m_FreeListAllocator;
            MemoryBuffer& memoryBuffer = pool.m_MemoryBuffer;
            if (memoryBuffer.PointerIsInBuffer(ptr))
            {
                return freeListAllocator;
            }
        }

        throw BadAllocationException();
    }

    inline FreeListAllocator &MultiListAllocator::FindAllocatorForSize(size_t size)
    {
        for (auto& pool : m_Pools)
        {
            FreeListAllocator& freeListAllocator = pool.m_FreeListAllocator;
            if (freeListAllocator.GetSize() - freeListAllocator.GetAllocatedBytes() >= size)
            {
                return freeListAllocator;
            }
        }

        AllocateAdditionalMemoryBuffer(size);
        return m_Pools[m_Pools.size() - 1].m_FreeListAllocator;
    }

} // namespace Savanna
