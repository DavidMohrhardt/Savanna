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
        InterfaceAllocator m_Allocator;
        void* m_pUserData;
    };

    VkAllocationCallbacks GetVkAllocationCallbacks(VkAllocator* pRealAllocator);
}
