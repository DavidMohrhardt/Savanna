/**
 * @file VkAllocationCallback.h
 * @author David Mohrhardt (https://github.com/DavidMohrhardt/Savanna)
 * @brief
 * @version 0.1
 * @date 2023-09-14
 *
 * @copyright Copyright (c) 2023
 *
 */
#pragma once

#include <SavannaVk2.h>

#if defined(__cplusplus)
extern "C"
{
#endif // defined(__cplusplus)

    void* SavannaGfxVkAllocatorCallbackAlloc(void* pUserData, size_t size, size_t alignment, VkSystemAllocationScope allocationScope);
    void* SavannaGfxVkAllocatorCallbackRealloc(void* pUserData, void* pOriginal, size_t size, size_t alignment, VkSystemAllocationScope allocationScope);
    void SavannaGfxVkAllocatorCallbackFree(void* pUserData, void* pMemory);

    se_AllocatorInterface_t SavannaGfxGetDefaultAllocatorInterface();

    static const VkAllocationCallbacks k_SavannaDefaultVkAllocationCallbacks =
    {
        nullptr, // void* pUserData;
        SavannaGfxVkAllocatorCallbackAlloc, // PFN_vkAllocationFunction pfnAllocation;
        SavannaGfxVkAllocatorCallbackRealloc, // PFN_vkReallocationFunction pfnReallocation;
        SavannaGfxVkAllocatorCallbackFree, // PFN_vkFreeFunction pfnFree;
        nullptr, // PFN_vkInternalAllocationNotification pfnInternalAllocation;
        nullptr, // PFN_vkInternalFreeNotification pfnInternalFree;
    };

#if defined(__cplusplus)
}
#endif // defined(__cplusplus)
