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

namespace Savanna::Gfx::Vk2
{
    struct VkAllocator
    {
        static void* Alloc(void *pUserData, size_t size, size_t alignment, VkSystemAllocationScope allocationScope)
        {
            return (*static_cast<se_AllocatorInterface_t*>(pUserData)).m_AllocAlignedFunc(size, alignment, nullptr);
        }

        static void* Realloc(void *pUserData, void *pOriginal, size_t size, size_t alignment, VkSystemAllocationScope allocationScope)
        {
            return SAVANNA_INTERFACE_REALLOCATE_ALIGNED(pUserData, pOriginal, alignment, size, nullptr);
        }

        static void Free(void *pUserData, void *pMemory)
        {
            return SAVANNA_INTERFACE_FREE(pUserData, pMemory, nullptr);
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
