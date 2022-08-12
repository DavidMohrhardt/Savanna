#pragma once

#if __cplusplus
#include <new>

#include "Handle.h"

#include "Profiling/Profiler.h"

#ifdef IMPLEMENT_SAVANNA_CLASS

#undef IMPLEMENT_SAVANNA_CLASS

#endif

#if SAVANNA_USE_OLD_CORE_MEMORY
#define DECLARE_COMMON_SAVANNA_CLASS(typeName) \
public:\
    void* operator new(size_t size, bool initializeMemoryToZero = false);\
    void* operator new(size_t size, Savanna::AllocatorKind &allocatorKind, bool initializeMemoryToZero = false);\
    void* operator new(size_t size, Savanna::SEHandle<Savanna::Allocator>& allocatorHandle, bool initializeMemoryToZero = false);\
    void* operator new(size_t size, Savanna::Allocator* pAllocator, bool initializeMemoryToZero = false);\
    void operator delete(void* ptr)

#define IMPLEMENT_SAVANNA_CLASS(typeName) \
\
    void* typeName::operator new(size_t size, bool initializeMemoryToZero)\
    {\
        SAVANNA_INSERT_SCOPED_PROFILER(#typeName " Operator new()"); \
        Savanna::SEHandle<Savanna::Allocator> allocatorHandle = Savanna::MemoryManager::GetInstance()->GetAllocatorHandle(Savanna::AllocatorKind::Persistent);\
        if (!allocatorHandle.IsValid())\
            throw Savanna::BadAllocationException();\
        void* dataPtr = Savanna::MemoryManager::GetInstance()->Allocate(allocatorHandle, GetL1CacheLineCount(size), initializeMemoryToZero);\
        if (dataPtr == nullptr)\
            throw Savanna::BadAllocationException();\
        dataPtr->m_AllocatorHandle = allocatorHandle;\
        return dataPtr;\
    }\
\
    void* typeName::operator new(size_t size, Savanna::AllocatorKind &allocatorKind, bool initializeMemoryToZero)\
    {\
        SAVANNA_INSERT_SCOPED_PROFILER(#typeName " Operator new(allocatorKind)"); \
        Savanna::SEHandle<Savanna::Allocator> allocatorHandle = Savanna::MemoryManager::GetInstance()->GetAllocatorHandle(allocatorKind);\
        if (!allocatorHandle.IsValid())\
            throw Savanna::BadAllocationException();\
        void* dataPtr = Savanna::MemoryManager::GetInstance()->Allocate(allocatorHandle, GetL1CacheLineCount(size), initializeMemoryToZero);\
        if (dataPtr == nullptr)\
            throw Savanna::BadAllocationException();\
        dataPtr->m_AllocatorHandle = allocatorHandle;\
        return dataPtr;\
    }\
\
    void* typeName::operator new(size_t size, Savanna::SEHandle<Allocator>& allocatorHandle, bool initializeMemoryToZero)\
    {\
        SAVANNA_INSERT_SCOPED_PROFILER(#typeName " Operator new(allocatorHandle)"); \
        if (!allocatorHandle.IsValid())\
            throw Savanna::BadAllocationException();\
        void* dataPtr = Savanna::MemoryManager::GetInstance()->Allocate(allocatorHandle, GetL1CacheLineCount(size), initializeMemoryToZero);\
        if (dataPtr == nullptr)\
            throw Savanna::BadAllocationException();\
        dataPtr->m_AllocatorHandle = allocatorHandle;\
        return dataPtr;\
    }\
\
    void* typeName::operator new(size_t size, Savanna::Allocator* pAllocator, bool initializeMemoryToZero)\
    {\
        SAVANNA_INSERT_SCOPED_PROFILER(#typeName " Operator new(pAllocator)"); \
        Savanna::SEHandle<Savanna::Allocator> allocatorHandle = *reinterpret_cast<const Savanna::SEHandle<Savanna::Allocator>*>(pAllocator);\
        if (!allocatorHandle.IsValid())\
            throw Savanna::BadAllocationException();\
        void* dataPtr = Savanna::MemoryManager::GetInstance()->Allocate(allocatorHandle, GetL1CacheLineCount(size), initializeMemoryToZero);\
        if (dataPtr == nullptr)\
            throw Savanna::BadAllocationException();\
        dataPtr->m_AllocatorHandle = *reinterpret_cast<Savanna::SEHandle<Allocator>*>(pAllocator);\
        return dataPtr;\
    }\
\
    void typeName::operator delete(void* ptr)\
    {\
        SAVANNA_INSERT_SCOPED_PROFILER(#typeName " Operator delete"); \
        if (ptr == nullptr) return;\
        Savanna::SEHandle<Savanna::Allocator> allocatorHandle = (reinterpret_cast<const typeName *>(ptr))->m_AllocatorHandle;\
        Savanna::MemoryManager::GetInstance()->Delete(allocatorHandle, ptr);\
    }

#else

#define DECLARE_COMMON_SAVANNA_CLASS(typeName) \
public:\
    void* operator new(size_t size);\
    void* operator new(size_t size, bool initializeMemoryToZero);\
    void operator delete(void* ptr)

#define IMPLEMENT_SAVANNA_CLASS(typeName) \
\
    void* typeName::operator new(size_t size)\
    {\
        SAVANNA_INSERT_SCOPED_PROFILER(#typeName " Operator new()"); \
        void dataPtr = malloc(size); \
        if (dataPtr == nullptr)\
            throw Savanna::BadAllocationException();\
        return dataPtr;\
    }\
\
    void* typeName::operator new(size_t size, bool initializeMemoryToZero)\
    {\
        SAVANNA_INSERT_SCOPED_PROFILER(#typeName " Operator new() memset 0"); \
        void dataPtr = initializeMemoryToZero ? calloc(size) : malloc(size); \
        if (dataPtr == nullptr)\
            throw Savanna::BadAllocationException();\
        return dataPtr;\
    }\
\
    void typeName::operator delete(void* ptr)\
    {\
        SAVANNA_INSERT_SCOPED_PROFILER(#typeName " Operator delete"); \
        if (ptr == nullptr) return;\
        free(ptr);\
    }

#define DEFINE_SAVANNA_CLASS \
\
    void* operator new(size_t size)\
    {\
        SAVANNA_INSERT_SCOPED_PROFILER(#typeName " Operator new()"); \
        void dataPtr = malloc(size); \
        if (dataPtr == nullptr)\
            throw Savanna::BadAllocationException();\
        return dataPtr;\
    }\
\
    void* operator new(size_t size, bool initializeMemoryToZero)\
    {\
        SAVANNA_INSERT_SCOPED_PROFILER(#typeName " Operator new() memset 0"); \
        void dataPtr = initializeMemoryToZero ? calloc(size) : malloc(size); \
        if (dataPtr == nullptr)\
            throw Savanna::BadAllocationException();\
        return dataPtr;\
    }\
\
    void operator delete(void* ptr)\
    {\
        SAVANNA_INSERT_SCOPED_PROFILER(#typeName " Operator delete"); \
        if (ptr == nullptr) return;\
        free(ptr);\
    }
#endif

namespace Savanna
{
    /*template<class T>
    class SavannaClass
    {
        DEFINE_SAVANNA_CLASS;
    };*/
} // namespace Savanna


#else // !__cplusplus

#define IMPLEMENT_SAVANNA_CLASS

#endif  // end __cplusplus
