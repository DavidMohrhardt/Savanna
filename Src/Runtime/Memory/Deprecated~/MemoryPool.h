#pragma once

#include <Utilities/SavannaCoding.h>

namespace Savanna
{
    struct MemoryPool
    {
    public:
        void* m_Buffer;
        size_t m_PoolSize;
        size_t m_PoolCount;
        int64 m_AllocatedPoolsBitField;
        bool m_OwnsMemory;

        MemoryPool* m_pNextPool;

        MemoryPool() = default;

        MemoryPool(void* pBuffer, size_t poolSize, size_t poolCount, bool ownsMemory, bool uninitialized = true)
            : m_Buffer(malloc(poolSize * poolCount))
            , m_PoolSize(poolSize)
            , m_PoolCount(poolCount)
            , m_AllocatedPoolsBitField(0x0)
            , m_OwnsMemory(ownsMemory)
            , m_pNextPool(nullptr)
        {
            if (!uninitialized)
            {
                memset(m_Buffer, 0, poolSize * poolCount);
            }
        }

        MemoryPool(size_t poolSize, size_t poolCount, bool uninitialized = true)
            : MemoryPool(malloc(poolSize * poolCount), poolSize, poolCount, true, uninitialized)
        {}

        ~MemoryPool()
        {
            if (m_OwnsMemory)
                free(m_Buffer);
        }

    private:
        void* AcquirePoolInternal()
        {
            if (m_AllocatedPoolsBitField == 0xFFFFFFFFFFFFFFFF) SAVANNA_BRANCH_UNLIKELY
            {
                return nullptr;
            }

            // TODO @DavidMohrhardt: Practically this is probably fine but a faster to use a bit scan intrinsic. Work for the future.
            int64 firstZeroBitIndex = 0;
            while (m_AllocatedPoolsBitField & (1 << firstZeroBitIndex))
            {
                ++firstZeroBitIndex;
            }

            // Set the bit to 1
            m_AllocatedPoolsBitField |= (1 << firstZeroBitIndex);

            // Return the pointer to the pool
            return Add(m_Buffer, m_PoolSize * firstZeroBitIndex);
        }

        bool TryReleasePoolInternal(void* pPoolBuffer)
        {
            int64 poolIndex = (reinterpret_cast<intptr>(pPoolBuffer) - reinterpret_cast<intptr>(m_Buffer)) / m_PoolSize;

            if (poolIndex < 0 || poolIndex >= m_PoolCount)
            {
                return false;
            }

            // Set the bit to 0
            m_AllocatedPoolsBitField &= ~(1 << poolIndex);
        }

    private:
        static void* AcquireNextAvailablePool(MemoryPool* pRoot)
        {
            if (pRoot == nullptr)
            {
                return nullptr;
            }

            MemoryPool* currentBuffer = pRoot;
            while (currentBuffer != nullptr)
            {
                if (currentBuffer->m_AllocatedPoolsBitField != 0xFFFFFFFFFFFFFFFF) SAVANNA_BRANCH_LIKELY
                {
                    // Acquire a pool from this buffer
                    return currentBuffer->AcquirePoolInternal();
                }
                else if (currentBuffer->m_pNextPool != nullptr) SAVANNA_BRANCH_LIKELY
                {
                    // Move to the next buffer
                    currentBuffer = currentBuffer->m_pNextPool;
                }
                else SAVANNA_BRANCH_UNLIKELY
                {
                    // Allocate another pool from the heal
                    currentBuffer->m_pNextPool = new MemoryPool(currentBuffer->m_PoolSize, currentBuffer->m_PoolCount);
                    return currentBuffer->m_pNextPool->AcquirePoolInternal();
                }
            }
        }

        static void ReleasePool(void* pPoolEntry, MemoryPool* pPools, const size_t& poolCount)
        {
            for (size_t i = 0; i < poolCount; ++i)
            {
                if (pPools[i].TryReleasePoolInternal(pPoolEntry))
                    return;
            }

            SAVANNA_ASSERT(false, "Attempting to release a pool that does not belong to this allocator");
        }
    };
} // namespace Savanna
