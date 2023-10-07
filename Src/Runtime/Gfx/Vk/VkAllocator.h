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
    class VkAllocator
    {
    private:
        inline static InterfaceAllocator* EnsureValidInterface(void* pUserData)
        {
            if (pUserData == nullptr)
            {
                throw std::runtime_error("pUserData is nullptr!");
            }
            return reinterpret_cast<InterfaceAllocator*>(pUserData);
        }

    public:
        static void* Alloc(void *pUserData, size_t size, size_t alignment, VkSystemAllocationScope allocationScope)
        {
            return EnsureValidInterface(pUserData)->AllocateAligned(size, alignment);
        }

        static void* Realloc(void *pUserData, void *pOriginal, size_t size, size_t alignment, VkSystemAllocationScope allocationScope)
        {
            return EnsureValidInterface(pUserData)->ReallocateAligned(pOriginal, alignment, size);
        }

        static void Free(void *pUserData, void *pMemory)
        {
            EnsureValidInterface(pUserData)->Free(pMemory);
        }

        static VkAllocationCallbacks CreateAllocationCallbacksForInterface(InterfaceAllocator* pAllocator)
        {
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
