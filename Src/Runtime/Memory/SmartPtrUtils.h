#pragma once

#include "Memory/MemoryManager.h"

#include <memory>

namespace Savanna
{
    struct MemoryManagerDeleter
    {
        void operator()(void* ptr)
        {
            MemoryManager::Get().Free(ptr);
        }
    };

    template <typename T>
    std::shared_ptr<T> CreateManagedSharedPtr(T* ptr)
    {
        return std::shared_ptr<T>(ptr, MemoryManagerDeleter{});
    }

    template <typename T>
    std::unique_ptr<T> CreateManagedUniquePtr(T* ptr)
    {
        return std::unique_ptr<T>(ptr, MemoryManagerDeleter{});
    }

    template <typename T, typename... Args>
    std::shared_ptr<T> MakeShared(Args&&... args)
    {
        return CreateManagedSharedPtr(SAVANNA_NEW(T, std::forward<Args>(args)...));
    }

    template <typename T, typename... Args>
    std::unique_ptr<T> MakeUnique(Args&&... args)
    {
        return CreateManagedUniquePtr(SAVANNA_NEW(T, std::forward<Args>(args)...));
    }

    template <typename T>
    std::shared_ptr<T> MakeSharedArray(const size_t& count)
    {
        return CreateManagedSharedPtr(SAVANNA_NEW_ARRAY(T, count));
    }

    template <typename T>
    std::unique_ptr<T> MakeUniqueArray(const size_t& count)
    {
        return CreateManagedUniquePtr(SAVANNA_NEW_ARRAY(T, count));
    }
} // namespace Savanna
