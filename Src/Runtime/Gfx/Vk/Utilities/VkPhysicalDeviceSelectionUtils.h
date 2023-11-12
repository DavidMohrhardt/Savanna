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

namespace Savanna::Gfx::Vk2::Utils
{
    inline bool TrySelectPhysicalDevice(
        VkInstance instance,
        VkPhysicalDevice& outPhysicalDevice,
        const se_VkPhysicalDeviceCreateArgs_t& physicalDeviceCreateArgs)
    {
        // TODO @DavidMohrhardt: It appears that VkEnumeratePhysicalDevices
        // has a memory leak in the implementation. It seemingly allocates
        // memory for something but never ends up freeing them. Using
        // a custom allocator bypasses the problem but it'll still be reported.
        SAVANNA_INSERT_SCOPED_PROFILER(Savanna::Gfx::Vk2::Utils::TrySelectPhysicalDevice);
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

            // TODO @DavidMohrhardt: Use a scratch allocator to allocate this array since it's short lived
            // Savanna::MemoryManager::GetScratchAllocator().Allocate(physicalDeviceCount * sizeof(VkPhysicalDevice), 1, 1);
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
} // namespace Savanna::Gfx::Vk2::Utils
