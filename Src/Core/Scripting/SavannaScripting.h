#pragma once

#include <mutex>

#include "Types/SavannaClass.h"

namespace Savanna::Scripting
{
    std::mutex g_GlobalScriptingLock;

    class SavannaScriptingClass : public SavannaClass
    {
    public:
        // Delete alternate allocator creation methods for non-native classes
        void* operator new(size_t size, bool initializeMemoryToZero = false) = delete;
        void* operator new(size_t size, AllocatorKind& allocatorKind, bool initializeMemoryToZero = false) = delete;
        void* operator new(size_t size, SEHandle<Allocator>& allocatorHandle, bool initializeMemoryToZero = false) = delete;
        void* operator new(size_t size, Allocator* pAllocator, bool initializeMemoryToZero = false) = delete;
    };
} // namespace Savanna::Scripting