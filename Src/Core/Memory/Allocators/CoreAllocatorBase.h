/**
 * @file CoreAllocatorBase.h
 * @author David Mohrhardt (https://github.com/DavidMohrhardt/Savanna)
 * @brief
 * @version 0.1
 * @date 2022-08-19
 *
 */
#pragma once

#include "AllocatorUtils.h"

#include "Utilities/SavannaCoding.h"

namespace Savanna
{
    class CoreAllocatorBase
    {
    protected:
        size_t m_Size;
        size_t m_AllocatedBytes;
        void* m_Root;

    public:
        CoreAllocatorBase();
        CoreAllocatorBase(void* root, size_t size);
        CoreAllocatorBase(CoreAllocatorBase& other) = delete;
        CoreAllocatorBase(CoreAllocatorBase&& other);

        virtual ~CoreAllocatorBase() {}
    public:
        CoreAllocatorBase& operator=(CoreAllocatorBase& other) = delete;
        CoreAllocatorBase& operator=(CoreAllocatorBase&& other);

    public:
        SAVANNA_NO_DISCARD virtual void* Allocate(const size_t& size, const size_t& alignment) = 0;
        virtual void Deallocate(void* const ptr, const size_t& alignment) = 0;

    public:
        template<typename T>
        SAVANNA_NO_DISCARD T* Allocate(const size_t& count = 1)
        {
            return reinterpret_cast<T*>(Allocate(sizeof(T) * count, alignof(T)));
        }

        template<typename T>
        void Deallocate(T* const ptr)
        {
            Deallocate(ptr, alignof(T));
        }

        const void* GetRoot() const { return m_Root; };

        SAVANNA_NO_DISCARD size_t GetAllocatedBytes() const { return m_AllocatedBytes; };
        SAVANNA_NO_DISCARD size_t GetSize() const { return m_Size; };
    };
} // namespace Savanna
