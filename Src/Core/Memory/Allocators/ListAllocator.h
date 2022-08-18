/**
 * @file ListAllocator.h
 * @author David Mohrhardt (https://github.com/DavidMohrhardt/Savanna)
 * @brief
 * @version 0.1
 * @date 2022-08-02
 *
 */
#pragma once

#include "Memory/CacheLine.h"
#include "Memory/MemoryArena.h"

#include "Utilities/SavannaCoding.h"

namespace Savanna
{
    class ListAllocator
    {
    private:
        void* m_Root;
        void* m_Head;
        size_t m_Size;
        size_t m_AllocatedBytes;

    public:
        ListAllocator();
        ListAllocator(void* m_Root, size_t size);
        ListAllocator(ListAllocator& other) = delete;
        ListAllocator(ListAllocator&& other);
        ~ListAllocator();

        SAVANNA_NO_DISCARD void* Allocate(size_t size, const size_t& alignment);

        template<typename T>
        SAVANNA_NO_DISCARD T* Allocate(size_t count = 1)
        {
            return reinterpret_cast<T*>(Allocate(sizeof(T) * count, alignof(T)));
        }

        void Reset();

        void Deallocate(void* const ptr, const size_t alignment) {}
        /**
         * @brief Does nothing as this allocator does not support deallocation.
         *
         * @tparam T
         * @param ptr
         */
        template<typename T>
        void Deallocate(T* const ptr) {}

        SAVANNA_NO_DISCARD size_t GetAllocatedBytes() const { return m_AllocatedBytes; };
        SAVANNA_NO_DISCARD size_t GetSize() const { return m_Size; };
    };

} // namespace Savanna
