/**
 * @file ManagedObject.h
 * @author David Mohrhardt (https://github.com/DavidMohrhardt/Savanna)
 * @brief
 * @version 0.1
 * @date 2023-01-08
 *
 * @copyright Copyright (c) 2023
 *
 */
#pragma once

#if __cplusplus
#include <new>

#include "Handle.h"

#include "Profiling/Profiler.h"

namespace Savanna
{
    template <typename T>
    class ManagedObject
    {
    private:
        Handle m_Handle;

    protected:
        ManagedObject() = default;
        virtual ~ManagedObject() = default;

        ManagedObject(Handle handle) : m_Handle(handle) {}

        ManagedObject(const ManagedObject&) = default;
        ManagedObject(ManagedObject&&) = default;

    public:
        ManagedObject& operator=(const ManagedObject&) = default;
        ManagedObject& operator=(ManagedObject&&) = default;

        // For ManagedObjects, the new operator is overloaded to return a handle to the allocated memory
        void* operator new(size_t size) = delete;
        void operator delete(void* ptr) = delete;

        void* operator new(size_t size, Handle& outHandle, const uint8& arenaId) final
        {
            SAVANNA_INSERT_SCOPED_PROFILER("ManagedObject Operator new");
            outHandle = MemoryManager::GetInstance()->AllocateHandle(size, alignof(T));
            if (!outHandle.IsValid())
                throw BadAllocationException();
            return outHandle.Get();
        }

        void operator delete(void* ptr, const Handle& handle) final
        {
            SAVANNA_INSERT_SCOPED_PROFILER("ManagedObject Operator delete");
            if (!handle.IsValid()) return;
            MemoryManager::Get()->ReleaseHandle(handle);
        }
    };
}


#else // !__cplusplus

#define IMPLEMENT_SAVANNA_CLASS

#endif  // end __cplusplus
