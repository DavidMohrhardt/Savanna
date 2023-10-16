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

#define SAVANNA_ENABLE_VK_ALLOCATOR_TRACKING 0

namespace Savanna::Gfx::Vk2
{
#if SAVANNA_ENABLE_VK_ALLOCATOR_TRACKING
    inline static int64 s_Allocations = 0;
    inline static int64 s_AllocationsMax = 0;
#endif // SAVANNA_ENABLE_VK_ALLOCATOR_TRACKING

    struct VkAllocator
    {
        static void* Alloc(void *pUserData, size_t size, size_t alignment, VkSystemAllocationScope allocationScope)
        {
#if !SAVANNA_ENABLE_VK_ALLOCATOR_TRACKING
            return SAVANNA_INTERFACE_ALLOCATE_ALIGNED(pUserData, size, alignment, nullptr);
#else
            void* pBuffer = SAVANNA_INTERFACE_ALLOCATE_ALIGNED(pUserData, size, alignment, nullptr);
            if (pBuffer != nullptr)
            {
                s_Allocations++;
                s_AllocationsMax = std::max(s_AllocationsMax, s_Allocations);
            }
            return pBuffer;
#endif // !SAVANNA_ENABLE_VK_ALLOCATOR_TRACKING
        }

        static void* Realloc(void *pUserData, void *pOriginal, size_t size, size_t alignment, VkSystemAllocationScope allocationScope)
        {
            return SAVANNA_INTERFACE_REALLOCATE_ALIGNED(pUserData, pOriginal, size, alignment, nullptr);
        }

        static void Free(void *pUserData, void *pMemory)
        {
            if (pMemory == nullptr)
            {
                return;
            }
#if SAVANNA_ENABLE_VK_ALLOCATOR_TRACKING
            s_Allocations -= 1;
            SAVANNA_ASSERT(s_Allocations >= 0, "Cannot free more times that you've allocated!");
#endif // SAVANNA_ENABLE_VK_ALLOCATOR_TRACKING

            SAVANNA_INTERFACE_FREE(pUserData, pMemory, nullptr);
        }

        static VkAllocationCallbacks CreateAllocationCallbacksForInterface(se_AllocatorInterface_t* pAllocator)
        {
            if (pAllocator == nullptr)
            {
                throw std::runtime_error("pAllocator is nullptr!");
            }

            return {
                .pUserData = pAllocator,
                .pfnAllocation = Alloc,
                .pfnReallocation = Realloc,
                .pfnFree = Free,
                .pfnInternalAllocation = nullptr,
                .pfnInternalFree = nullptr
            };
        }
    };
}
