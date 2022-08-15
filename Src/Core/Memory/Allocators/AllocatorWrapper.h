/**
 * @file AllocatorWrapper.h
 * @author David Mohrhardt (DavidMohrhardt@github.com)
 * @brief TODO @DavidMohrhardt Document
 * @version 0.1
 * @date 2022-08-02
 *
 * @copyright Copyright (c) 2022
 *
 */

#pragma once

#include "Utilities/SavannaCoding.h"

namespace Savanna
{

    // template <class T>
    // class StlAdaptor
    // {
    // public:
    //     typedef T value_type;

    //     StlAdaptor() = delete;

    //     StlAdaptor(Allocator& allocator) SAVANNA_NO_EXCEPT : Allocator(allocator) {}
    //     StlAdaptor(ISavannaAllocator& interface) SAVANNA_NO_EXCEPT : Allocator(interface) {}

    //     ~StlAdaptor() {}

    //     SAVANNA_NO_DISCARD constexpr value_type* allocate(size_t size)
    //     {
    //         if (GetInterface().m_AllocateFunc != nullptr)
    //         {
    //             return reinterpret_cast<value_type*>GetInterface().m_AllocateFunc(size);
    //         }
    //         else
    //         {
    //             throw BadAllocationException();
    //         }
    //     }

    //     size_t MaxAllocationSize() const SAVANNA_NO_EXCEPT
    //     {
    //         if (GetInterface.m_GetSizeFunc != nullptr)
    //             return GetInterface.m_GetSize(sizeof(value_type));
    //         else
    //             return 0;
    //     }

    //     constexpr value_type* deallocate(value_type* ptr, size_t size)
    //     {
    //         if (GetInterface().m_DeallocateFunc != nullptr)
    //         {
    //             return GetInterface().m_DeallocateFunc(ptr, size);
    //         }
    //     }

    //     inline bool operator==(const StlAdaptor<value_type>& rhs) const SAVANNA_NO_EXCEPT
    //     {
    //         return rhs == this;
    //     }

    //     inline bool operator!=(const StlAdaptor<value_type>& rhs) const SAVANNA_NO_EXCEPT
    //     {
    //         return rhs != this;
    //     }
    // };
} // namespace Savanna
