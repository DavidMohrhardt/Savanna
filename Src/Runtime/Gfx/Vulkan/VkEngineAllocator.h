/**
 * @file VkSavannaVkEngineAllocator.h
 * @author David Mohrhardt (https://github.com/DavidMohrhardt/Savanna)
 * @brief
 * @version 0.1
 * @date 2023-09-14
 *
 * @copyright Copyright (c) 2023
 *
 */
#pragma once

#include <SavannaVk.h>

#if defined(__cplusplus)
extern "C"
{
#endif // defined(__cplusplus)

    void* savanna_gfx_vk_engine_allocator_alloc(void* pUserData, size_t size, size_t alignment, VkSystemAllocationScope allocationScope);
    void* savanna_gfx_vk_engine_allocator_realloc(void* pUserData, void* pOriginal, size_t size, size_t alignment, VkSystemAllocationScope allocationScope);
    void savanna_gfx_vk_engine_allocator_free(void* pUserData, void* pMemory);

#if defined(__cplusplus)
}
#endif // defined(__cplusplus)

static VkAllocationCallbacks k_SavannaDefaultVkAllocationCallbacks =
{
    nullptr, // void* pUserData;
    savanna_gfx_vk_engine_allocator_alloc, // PFN_vkAllocationFunction pfnAllocation;
    savanna_gfx_vk_engine_allocator_realloc, // PFN_vkReallocationFunction pfnReallocation;
    savanna_gfx_vk_engine_allocator_free, // PFN_vkFreeFunction pfnFree;
    nullptr, // PFN_vkInternalAllocationNotification pfnInternalAllocation;
    nullptr, // PFN_vkInternalFreeNotification pfnInternalFree;
};
