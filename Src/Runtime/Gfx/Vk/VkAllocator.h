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
    struct VkAllocator;
    VkAllocationCallbacks GetVkAllocationCallbacks(VkAllocator* pRealAllocator);

    /**
     * @brief This struct provides a wrapper for the Vulkan allocation callbacks.
     * It uses a provided se_AllocatorInterface_t to perform the actual allocations.
     *
     */
    struct VkAllocator
    {
    public:
        InterfaceAllocator m_Allocator {  };
        void* m_pUserData { nullptr };
        VkAllocationCallbacks m_AllocationCallbacks { GetVkAllocationCallbacks(this) };

    public:
        VkAllocator() = default;
        ~VkAllocator() = default;

        VkAllocator(se_AllocatorInterface_t allocatorInterface, void* pUserData)
            : m_Allocator(allocatorInterface)
            , m_pUserData(pUserData)
        {
        }

        VkAllocationCallbacks& GetAllocationCallbacks()
        {
            return m_AllocationCallbacks;
        }
    };
}
