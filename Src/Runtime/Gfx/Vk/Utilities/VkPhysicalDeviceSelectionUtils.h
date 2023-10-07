/**
 * @file VkPhysicalDeviceSelectionUtils.h
 * @author David Mohrhardt (https://github.com/DavidMohrhardt/Savanna)
 * @brief
 * @version 0.1
 * @date 2023-10-07
 *
 * @copyright Copyright (c) 2023
 *
 */
#pragma once

#include "SavannaVk2.h"

#include <vulkan/vulkan.h>

namespace Savanna::Gfx::Vk2::Utils
{
    inline bool TrySelectPhysicalDevice(
        VkInstance instance,
        VkPhysicalDevice& outPhysicalDevice,
        const se_VkPhysicalDeviceCreateArgs_t& physicalDeviceCreateArgs,
        const se_AllocatorInterface_t& allocatorInterface)
    {
        uint32 physicalDeviceCount = 0;
        VK_MUST_SUCCEED_RETURN(vkEnumeratePhysicalDevices(instance, &physicalDeviceCount, nullptr),
            "Failed to enumerate physical devices.",
            false);
        if (physicalDeviceCount == 0)
        {
            SAVANNA_LOG("No physical devices found.");
            return false;
        }
        DynamicArray<VkPhysicalDevice> physicalDevices{ physicalDeviceCount, allocatorInterface };

        // TODO @DavidMohrhardt: Use a scratch allocator to allocate this array since it's short lived
        // Savanna::MemoryManager::GetScratchAllocator().Allocate(physicalDeviceCount * sizeof(VkPhysicalDevice), 1, 1);
        VK_MUST_SUCCEED_RETURN(vkEnumeratePhysicalDevices(instance, &physicalDeviceCount, physicalDevices.Data()),
            "Failed to enumerate physical devices.",
            false);

        // TODO @David.Mohrhardt: Select a physical device based on some criteria.
        int preferredGraphicsDeviceIndex = physicalDeviceCreateArgs.m_PreferredGraphicsDeviceIndex < physicalDeviceCount
            ? physicalDeviceCreateArgs.m_PreferredGraphicsDeviceIndex
            : 0;

        outPhysicalDevice = physicalDevices[preferredGraphicsDeviceIndex];
        return true;
    }
} // namespace Savanna::Gfx::Vk2::Utils
