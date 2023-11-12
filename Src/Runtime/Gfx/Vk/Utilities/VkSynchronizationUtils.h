/**
 * @file VkSemaphore.h
 * @author David Mohrhardt (https://github.com/DavidMohrhardt/Savanna)
 * @brief
 * @version 0.1
 * @date 2023-11-10
 *
 * @copyright Copyright (c) 2023
 *
 */

#pragma once

#include "SavannaVk2.h"
#include "VkGpu.h"
#include "VkAllocator.h"

namespace Savanna::Gfx::Vk2::Utils
{

    struct VkSynchronizationCapabilities
    {
        bool m_UsingSynchronization2;
        bool m_UsingTimelineSemaphores;
    };

    class VkSynchronizationInfo
    {
    private:
        static VkSynchronizationCapabilities m_Capabilities;

    public:
        static void Initialize(const VkGpu& gpu, const char** availableDeviceExt);

        static const VkSynchronizationCapabilities& GetCapabilities();
    };

    inline VkResult CreateSemaphore(const VkGpu& gpu, VkSemaphore& outSemaphore)
    {
        VkSemaphoreCreateInfo createInfo {};
        createInfo.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO;
        return vkCreateSemaphore(gpu, &createInfo, VkAllocator::Get(), &outSemaphore);
    }

} // namespace Savanna::Gfx::Vk2


// using VulkanSemaphore = VkSem