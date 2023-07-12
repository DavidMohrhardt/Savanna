/**
 * @file Allocator.h
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
    class Allocator
    {
    protected:
        void* m_Root;

    public:
        Allocator();
        Allocator(void* root);
        Allocator(Allocator& other) = delete;
        Allocator(Allocator&& other);

        virtual ~Allocator() {}
    public:
        Allocator& operator=(Allocator& other) = delete;
        Allocator& operator=(Allocator&& other);

    public:
        SAVANNA_NO_DISCARD virtual void* alloc(const size_t& size, const size_t& alignment) = 0;
        virtual void free(void* const ptr, const size_t& alignment) = 0;

    public:
        template<typename T>
        SAVANNA_NO_DISCARD T* Allocate(const size_t& count = 1)
        {
            return reinterpret_cast<T*>(alloc(sizeof(T) * count, alignof(T)));
        }

        template<typename T>
        void Release(T* const ptr)
        {
            free(ptr, alignof(T));
        }

        const void* GetRoot() const { return m_Root; };

        SAVANNA_NO_DISCARD virtual size_t GetAllocatedBytes() const = 0;
        SAVANNA_NO_DISCARD virtual size_t GetSize() const = 0;
        SAVANNA_NO_DISCARD bool IsValid() const { return m_Root != nullptr && GetSize() > 0; }
    };
} // namespace Savanna