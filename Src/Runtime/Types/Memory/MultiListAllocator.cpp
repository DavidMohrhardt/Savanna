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
        , m_Pools(0, MemoryManager::GetAllocatorInterfaceForLabel(label))
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
        , m_Pools(initialBufferCount > 0 ? initialBufferCount : 1, MemoryManager::GetAllocatorInterfaceForLabel(label))
    {
        SAVANNA_INSERT_SCOPED_PROFILER(MultiListAllocator::ctor);
        SAVANNA_ASSERT(m_BufferBlockSize != 0, "Buffer block size must be greater than 0.");
        size_t bufferSize = m_BufferBlockSize;
        AllocateAdditionalMemoryBuffer(bufferSize * initialBufferCount);
    }

    MultiListAllocator& MultiListAllocator::operator=(MultiListAllocator &&other)
    {
        if (this != &other)
        {
            Allocator::operator=(static_cast<Allocator&&>(other));
            SAVANNA_MOVE_MEMBER(m_MemoryLabel, other);
            SAVANNA_MOVE_MEMBER(m_BufferBlockSize, other);
            SAVANNA_MOVE_MEMBER(m_AllocatedBytes, other);
            SAVANNA_MOVE_MEMBER(m_Size, other);
            SAVANNA_MOVE_MEMBER(m_Pools, other);
        }
        return *this;
    }

    SAVANNA_NO_DISCARD void* MultiListAllocator::alloc(
        const size_t& size, const size_t& alignment)
    {
        SAVANNA_INSERT_SCOPED_PROFILER(MultiListAllocator::alloc);
        void* outPtr = nullptr;
        for (auto& pool : m_Pools)
        {
            FreeListAllocator& freeListAllocator = pool.m_FreeListAllocator;
            if (freeListAllocator.GetSize() - freeListAllocator.GetAllocatedBytes() >= size)
            {
                outPtr = freeListAllocator.alloc(size, alignment);
            }
        }

        if (outPtr == nullptr)
        {
            AllocateAdditionalMemoryBuffer(size);
            outPtr = m_Pools[m_Pools.size() - 1].m_FreeListAllocator.alloc(size, alignment);
        }

        return outPtr;
    }

    void MultiListAllocator::free(
        void* const ptr, const size_t& alignment)
    {
        SAVANNA_INSERT_SCOPED_PROFILER(MultiListAllocator::free);
        for (auto& pool : m_Pools)
        {
            FreeListAllocator& freeListAllocator = pool.m_FreeListAllocator;
            MemoryBuffer& memoryBuffer = pool.m_MemoryBuffer;
            if (memoryBuffer.PointerIsInBuffer(ptr))
            {
                freeListAllocator.free(ptr, alignment);
                return;
            }
        }
        SAVANNA_ASSERT(false, "Pointer is not in any of the memory buffers managed by this allocator.");
    }

    void MultiListAllocator::AllocateAdditionalMemoryBuffer(size_t size)
    {
        SAVANNA_INSERT_SCOPED_PROFILER(MultiListAllocator::AcquireNewBuffer);

        size_t bufferSize = m_BufferBlockSize + sizeof(MemoryChunkDescriptor);
        while (bufferSize < size)
        {
            bufferSize *= 2;
        }

        m_Pools.Append(MemoryPool(bufferSize, m_MemoryLabel));
        m_Size += bufferSize;
    }
} // namespace Savanna
