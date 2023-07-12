/**
 * @file FreeListAllocator.h
 * @author David Mohrhardt (https://github.com/DavidMohrhardt/Savanna)
 * @brief TODO @DavidMohrhardt Document
 * @version 0.1
 * @date 2022-08-02
 *
 */

#pragma once

#include "Allocator.h"
#include "MemoryChunkDescriptors.h"

#include "Utilities/SavannaCoding.h"
#include "Types/Memory/CacheLine.h"

namespace Savanna
{
    /**
     * @brief
     */
    class FreeListAllocator : public Allocator, NonCopyable
    {
    private:
        MemoryChunkDescriptor* m_Head;
        size_t m_Size;
        size_t m_AllocatedBytes;

    public:
        FreeListAllocator();
        FreeListAllocator(void* root, size_t size);
        FreeListAllocator(FreeListAllocator&& other);

        ~FreeListAllocator();

    public:
        FreeListAllocator& operator=(FreeListAllocator&& other);

    public:
        SAVANNA_NO_DISCARD void* alloc(const size_t& size, const size_t& alignment) SAVANNA_OVERRIDE;
        void free(void* const ptr, const size_t& alignment) SAVANNA_OVERRIDE;

        SAVANNA_NO_DISCARD size_t GetAllocatedBytes() const SAVANNA_OVERRIDE { return m_AllocatedBytes; }
        SAVANNA_NO_DISCARD size_t GetSize() const SAVANNA_OVERRIDE { return m_Size; }
    };
} // namespace Savanna
