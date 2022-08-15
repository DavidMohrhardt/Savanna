/**
 * @file ListAllocator.h
 * @author David Mohrhardt (DavidMohrhardt@github.com)
 * @brief
 * @version 0.1
 * @date 2022-08-02
 *
 * @copyright Copyright (c) 2022
 *
 */
#pragma once

#ifdef ENABLE_LIST_ALLOCATOR
#include "AllocatorUtils.h"

#include "Memory/CacheLine.h"
#include "Memory/MemoryArena.h"

#include "Utilities/SavannaCoding.h"

namespace Savanna
{
    template<typename T>
    class ListAllocator : public FreeListAllocator
    {
    public:
        ListAllocator(MemoryArena& arena, size_t size);
        ~ListAllocator();

        SAVANNA_NO_DISCARD T* allocate(size_t size);
        void deallocate(T* ptr, size_t size);

        size_t max_size() SAVANNA_NO_EXCEPT;
    };

    ListAllocator(MemoryArena& arena, size_t size)
    {}

    ListAllocator::~ListAllocator()
    {}

} // namespace Savanna
#endif // ENABLE_LIST_ALLOCATOR
