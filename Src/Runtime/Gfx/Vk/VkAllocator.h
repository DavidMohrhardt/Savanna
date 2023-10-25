/**
 * @file VkAllocator.h
 * @author David Mohrhardt (https://github.com/DavidMohrhardt/Savanna)
 * @brief
 * @version 0.1
 * @date 2023-09-27
 *
 * @copyright Copyright (c) 2023
 *
 */
#pragma once

#include <SavannaEngine.h>

#include <vulkan/vulkan.h>

#define SAVANNA_ENABLE_VK_ALLOCATOR_TRACKING 1

namespace Savanna::Gfx::Vk2
{
    class VkAllocator
    {
    private:
        // Only allow VkDriver to set the allocator interface pointer from the init and destroy functions
        // This ensures that the same allocator interface is used for all allocations.
        friend class VkDriver;

    public:
        static void* Alloc(void *pUserData, size_t size, size_t alignment, VkSystemAllocationScope allocationScope);
        static void* Realloc(void *pUserData, void *pOriginal, size_t size, size_t alignment, VkSystemAllocationScope allocationScope);
        static void Free(void *pUserData, void *pMemory);

        static const VkAllocationCallbacks* const Get();

    private:
        static VkAllocationCallbacks s_AllocationCallbacks;

#if SAVANNA_ENABLE_VK_ALLOCATOR_TRACKING
        inline static int64 s_Allocations = 0;
        inline static int64 s_AllocationsMax = 0;
#endif // SAVANNA_ENABLE_VK_ALLOCATOR_TRACKING

        static void SetVkAllocationInterfacePtr(const se_AllocatorInterface_t* pInterface);
    };
}
