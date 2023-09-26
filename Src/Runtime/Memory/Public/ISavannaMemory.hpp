/**
 * @file ISavannaMemory.hpp
 * @author David Mohrhardt (https://github.com/DavidMohrhardt/Savanna)
 * @brief
 * @version 0.1
 * @date 2023-09-25
 *
 * @copyright Copyright (c) 2023
 *
 */
#ifndef I_SAVANNA_MEMORY_HPP
#define I_SAVANNA_MEMORY_HPP

#include "ISavannaMemory.h"

extern "C"
{
    void* SavannaHeapAllocatorAllocateAligned(size_t size, size_t alignment, void* pUserData);
    void* SavannaHeapAllocatorAllocate(size_t size, void* pUserData);
    void* SavannaHeapAllocatorReallocAligned(void* ptr, size_t alignment, const size_t& newSize, void* pUserData);
    void* SavannaHeapAllocatorRealloc(void* ptr, const size_t& newSize, void* pUserData);
    void SavannaHeapAllocatorFree(void* ptr, void* pUserData);
}

namespace Savanna
{
    static constexpr se_AllocatorInterface_t k_HeapAllocatorInterface
    {
        .m_AllocFunc            = SavannaHeapAllocatorAllocate,
        .m_AllocAlignedFunc     = SavannaHeapAllocatorAllocateAligned,
        .m_ReallocFunc          = SavannaHeapAllocatorRealloc,
        .m_ReallocAlignedFunc   = SavannaHeapAllocatorReallocAligned,
        .m_FreeFunc             = SavannaHeapAllocatorFree
    };
} // namespace Savanna::Memory

#endif // I_SAVANNA_MEMORY_HPP
