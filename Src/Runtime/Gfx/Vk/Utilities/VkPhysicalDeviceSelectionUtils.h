/**
 * @file VkPhysicalDeviceSelectionUtils.h
 * @author David Mohrhardt (https://github.com/DavidMohrhardt/Savanna)
 * @brief TODO @David.Mohrhardt Document
 * @version 0.1
 * @date 2023-10-07
 *
 * @copyright Copyright (c) 2023
 *
 */
#pragma once

#include "SavannaVk2.h"

#include "VkPhysicalDeviceScoringUtils.h"

namespace savanna::Gfx::Vk2::Utils
{
    // TODO needs a refactor
    inline bool TrySelectPhysicalDevice(
        VkInstance instance,
        VkPhysicalDevice& outPhysicalDevice,
        const se_VkPhysicalDeviceCreateArgs_t& physicalDeviceCreateArgs)
    {
        SAVANNA_INSERT_SCOPED_PROFILER(savanna::Gfx::Vk2::Utils::TrySelectPhysicalDevice);
        uint32 physicalDeviceCount = 0;
        VK_MUST_SUCCEED_RETURN(vkEnumeratePhysicalDevices(instance, &physicalDeviceCount, nullptr),
            "Failed to enumerate physical devices.",
            false);

        if (physicalDeviceCount == 0)
        {
            SAVANNA_LOG("No physical devices found.");
            return false;
        }
        // just return the first device if there's only one
        else if (physicalDeviceCount == 1)
        {
            VK_MUST_SUCCEED_RETURN(vkEnumeratePhysicalDevices(instance, &physicalDeviceCount, &outPhysicalDevice),
                "Failed to enumerate physical devices.",
                false);
        }
        else
        {
            dynamic_array<VkPhysicalDevice> physicalDevices{ physicalDeviceCount, kSavannaAllocatorKindTemp };
            physicalDevices.resize(physicalDeviceCount);

            VK_MUST_SUCCEED_RETURN(vkEnumeratePhysicalDevices(instance, &physicalDeviceCount, physicalDevices.data()),
                "Failed to enumerate physical devices.",
                false);

            int preferredGraphicsDeviceIndex = ((physicalDeviceCreateArgs.m_PreferredGraphicsDeviceIndex - physicalDeviceCount) < 0)
                ? physicalDeviceCreateArgs.m_PreferredGraphicsDeviceIndex
                : physicalDeviceCount;
            if (preferredGraphicsDeviceIndex < physicalDeviceCount)
            {
                outPhysicalDevice = physicalDevices[preferredGraphicsDeviceIndex];
                return true;
            }

            uint32 highScore = 0;
            for (auto& physicalDevice : physicalDevices)
            {
                uint32 score = ScoreDevice(physicalDevice);
                if (highScore < score)
                {
                    highScore = score;
                    outPhysicalDevice = physicalDevice;
                }
            }
        }

        return outPhysicalDevice != VK_NULL_HANDLE;
    }
} // namespace savanna::Gfx::Vk2::Utils
