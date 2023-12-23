#pragma once

#include "Memory/MemoryManager.h"

#include <memory>

namespace savanna
{
    template <typename T>
    struct MemoryManagerDeleter
    {
        AllocatorKind m_AllocatorKind;

        void operator()(T* ptr)
        {
            SAVANNA_DELETE(m_AllocatorKind, ptr);
        }
    };

    template <typename T>
    struct MemoryManagerArrayDeleter
    {
        AllocatorKind m_AllocatorKind;

        void operator()(T* ptr)
        {
            SAVANNA_DELETE_ARRAY(m_AllocatorKind, ptr);
        }
    };

    template <typename T>
    inline std::shared_ptr<T> CreateManagedSharedPtr(const AllocatorKind allocatorKind, T* ptr)
    {
        return std::shared_ptr<T>(ptr, MemoryManagerDeleter{allocatorKind});
    }

    template <typename T>
    inline std::unique_ptr<T> CreateManagedUniquePtr(const AllocatorKind allocatorKind, T* ptr)
    {
        return std::unique_ptr<T>(ptr, MemoryManagerDeleter{allocatorKind});
    }

    template <typename T, typename... Args>
    inline std::shared_ptr<T> MakeShared(const AllocatorKind allocatorKind, Args&&... args)
    {
        return CreateManagedSharedPtr(allocatorKind, SAVANNA_NEW(allocatorKind, T, std::forward<Args>(args)...));
    }

    template <typename T, typename... Args>
    inline std::unique_ptr<T> MakeUnique(const AllocatorKind allocatorKind, Args&&... args)
    {
        return CreateManagedUniquePtr(allocatorKind, SAVANNA_NEW(allocatorKind, T, std::forward<Args>(args)...));
    }
} // namespace Savanna
