/**
 * @file VulkanDeviceSelection.h
 * @author David Mohrhardt (https://github.com/DavidMohrhardt/Savanna)
 * @brief
 * @version 0.1
 * @date 2022-08-19
 *
 */
#pragma once

#include <vector>

#include <Utilities/SavannaCoding.h>

#include <vulkan/vulkan.h>

namespace Savanna::Rendering::Vulkan
{
#if SAVANNA_VULKAN_ENABLE_DEBUGGING
    inline void PrintDeviceProperties(const VkPhysicalDeviceProperties& deviceProperties)
    {
        FixedString512 propertiesString = FixedString256("Vulkan Physical Device\n\t");
        // TODO
    }
#endif

    bool HasGraphicsQueue(const VkPhysicalDevice& device)
    {
        if (device == VK_NULL_HANDLE)
        {
            return false;
        }

        uint32 queueFamilyCount = 0;

        vkGetPhysicalDeviceQueueFamilyProperties(device, &queueFamilyCount, nullptr);

        std::vector<VkQueueFamilyProperties> queueFamilies(queueFamilyCount);
        vkGetPhysicalDeviceQueueFamilyProperties(device, &queueFamilyCount, queueFamilies.data());

        for (const auto& queueFamily : queueFamilies)
        {
            // if (queueFamily.queueFlags.isSet(VK_QUEUE_GRAPHICS_BIT))
            if (queueFamily.queueFlags & VK_QUEUE_GRAPHICS_BIT)
            {
                return true;
            }
        }
        return false;
    }

    uint32 GetPhysicalDeviceScore(
        const VkPhysicalDeviceProperties& properties,
        const VkPhysicalDeviceFeatures& features)
    {
        uint32 score = 0;

        if (properties.deviceType == VK_PHYSICAL_DEVICE_TYPE_DISCRETE_GPU)
        {
            score += 1000;
        }
        else if (properties.deviceType == VK_PHYSICAL_DEVICE_TYPE_INTEGRATED_GPU)
        {
            score += 500;
        }
        else if (properties.deviceType == VK_PHYSICAL_DEVICE_TYPE_VIRTUAL_GPU)
        {
            score += 100;
        }
        else if (properties.deviceType == VK_PHYSICAL_DEVICE_TYPE_CPU)
        {
            score += 50;
        }
        else
        {
            return 0;
        }

        score += properties.limits.maxImageDimension2D;

        if (!features.geometryShader)
        {
            return 0;
        }

        return score;
    }

    // TODO @DavidMohrhardt Remove the function and replace with a way to select the device
    inline bool TryGetSuitableDevices(std::vector<VkPhysicalDevice> availableDevices, VkPhysicalDevice& outDevice)
    {
        int bestScore = 0;
        int bestIndex = 0;
        int currentIndex = 0;
        for (auto& device : availableDevices)
        {
            if (!HasGraphicsQueue(device))
            {
                continue;
            }

            VkPhysicalDeviceProperties deviceProperties;
            vkGetPhysicalDeviceProperties(device, &deviceProperties);

            VkPhysicalDeviceFeatures deviceFeatures;
            vkGetPhysicalDeviceFeatures(device, &deviceFeatures);

#if SAVANNA_VULKAN_ENABLE_DEBUGGING
            PrintDeviceProperties(deviceProperties);
#endif

            int score = GetPhysicalDeviceScore(deviceProperties, deviceFeatures);
            if (score > bestScore)
            {
                bestScore = score;
                bestIndex = currentIndex;
            }
            currentIndex++;
        }

        if (bestScore > 0)
        {
            outDevice = availableDevices[bestIndex];
            return true;
        }
        else
        {
            return false;
        }
    }
} // namespace Savanna::Rendering::Vulkan
