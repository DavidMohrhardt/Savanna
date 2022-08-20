/**
 * @file CoreAllocatorWrapper.cpp
 * @author David Mohrhardt (https://github.com/DavidMohrhardt/Savanna)
 * @brief
 * @version 0.1
 * @date 2022-08-14
 *
 */

#include "CoreAllocatorWrapper.h"

namespace Savanna
{
    CoreAllocatorWrapper::CoreAllocatorWrapper(void* root, size_t size, AllocatorType allocatorType)
        : m_CoreAllocatorWrapperType(allocatorType)
    {
        switch (allocatorType)
        {
        case k_FreeList:
            m_FreeListAllocator = FreeListAllocator(root, size);
            break;
        // case k_Linear:
        //     m_LinearAllocator = LinearAllocator(root, size);
        //     break;
        // case k_Stack:
        //     m_StackAllocator = StackAllocator(root, size);
        //     break;
        default:
            break;
        }
    }

    CoreAllocatorWrapper::CoreAllocatorWrapper(CoreAllocatorWrapper&& other)
    {
        m_CoreAllocatorWrapperType = other.m_CoreAllocatorWrapperType;
        other.m_CoreAllocatorWrapperType = k_None;
        switch (m_CoreAllocatorWrapperType)
        {
        case k_FreeList:
            m_FreeListAllocator = std::move(other.m_FreeListAllocator);
            other.m_FreeListAllocator = FreeListAllocator();
            break;
        default:
            break;
        }
    }

    void* CoreAllocatorWrapper::Allocate(size_t& size, size_t& alignment)
    {
        // return m_allocator->Allocate(size, alignment);?
        switch (m_CoreAllocatorWrapperType)
        {
            case k_FreeList:
                return m_FreeListAllocator.Allocate(size, alignment);
            // case k_Linear:
            //     return m_linearAllocator->Allocate(size, alignment);
            // case k_Stack:
            //     return m_stackAllocator->Allocate(size, alignment);
            default:
                return nullptr;
        }
    }

    void CoreAllocatorWrapper::Free(void* ptr, const size_t& alignment)
    {
        switch (m_CoreAllocatorWrapperType)
        {
            case k_FreeList:
                m_FreeListAllocator.Deallocate(ptr, alignment);
                break;
            // case k_Linear:
            //     m_linearAllocator->Deallocate(ptr);
            //     break;
            // case k_Stack:
            //     m_stackAllocator->Deallocate(ptr);
            //     break;
            default:
                break;
        }
    }
} // namespace Savanna
