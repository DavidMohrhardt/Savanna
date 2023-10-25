/**
 * @file VkSwapchainUtils.cpp
 * @author David Mohrhardt (https://github.com/DavidMohrhardt/Savanna)
 * @brief
 * @version 0.1
 * @date 2023-01-03
 *
 * @copyright Copyright (c) 2023
 *
 */
#include "VkSwapchainUtils.h"

#include <algorithm>

namespace Savanna::Gfx::Vk2::Utils
{
    VkSurfaceFormatKHR ChooseSwapSurfaceFormat(const dynamic_array<VkSurfaceFormatKHR>& availableFormats)
    {
        // TODO @DavidMohrhardt: For the time being prefer HDR formats over SDR formats.
        // This should be configurable in the future.
        for (const auto& availableFormat : availableFormats)
        {
            if (availableFormat.format == VK_FORMAT_R16G16B16A16_SFLOAT && availableFormat.colorSpace == VK_COLOR_SPACE_HDR10_ST2084_EXT)
            {
                return availableFormat;
            }
        }
        return availableFormats[0];
    }

    VkPresentModeKHR ChooseSwapPresentMode(const dynamic_array<VkPresentModeKHR>& availablePresentModes)
    {
        for (const auto& availablePresentMode : availablePresentModes)
        {
            if (availablePresentMode == VK_PRESENT_MODE_MAILBOX_KHR)
            {
                return availablePresentMode;
            }
        }
        return VK_PRESENT_MODE_FIFO_KHR;
    }

    VkExtent2D ChooseSwapExtent(const VkSurfaceCapabilitiesKHR& capabilities, uint32 width, uint32 height)
    {
        if (capabilities.currentExtent.width != UINT32_MAX)
        {
            return capabilities.currentExtent;
        }
        else
        {
            VkExtent2D actualExtent = { width, height };
            return actualExtent;
        }
    }

    void GetSwapchainCapabilities(
        const VkPhysicalDevice& physicalDevice,
        const VkSurfaceKHR& surface,
        VkSurfaceCapabilitiesKHR& surfaceCapabilities)
    {
        vkGetPhysicalDeviceSurfaceCapabilitiesKHR(physicalDevice, surface, &surfaceCapabilities);
    }

    void GetSwapchainFormats(
        const VkPhysicalDevice& physicalDevice,
        const VkSurfaceKHR& surface,
        dynamic_array<VkSurfaceFormatKHR>& surfaceFormats,
        dynamic_array<VkFormatProperties>& formatProperties)
    {
        uint32_t formatCount;
        vkGetPhysicalDeviceSurfaceFormatsKHR(physicalDevice, surface, &formatCount, nullptr);
        if (formatCount != 0)
        {
            surfaceFormats.resize(formatCount);
            vkGetPhysicalDeviceSurfaceFormatsKHR(physicalDevice, surface, &formatCount, surfaceFormats.data());
        }

        for (uint32_t i = 0; i < formatCount; i++)
        {
            formatProperties.resize(formatCount);
            vkGetPhysicalDeviceFormatProperties(physicalDevice, surfaceFormats[i].format, &formatProperties[i]);
        }
    }

    void GetSwapchainPresentModes(
        const VkPhysicalDevice& physicalDevice,
        const VkSurfaceKHR& surface,
        dynamic_array<VkPresentModeKHR> &presentModes)
    {
        uint32_t presentModeCount;
        vkGetPhysicalDeviceSurfacePresentModesKHR(physicalDevice, surface, &presentModeCount, nullptr);
        if (presentModeCount != 0)
        {
            presentModes.resize(presentModeCount);
            vkGetPhysicalDeviceSurfacePresentModesKHR(physicalDevice, surface, &presentModeCount, presentModes.data());
        }
    }

    void PopulateSwapchainSupportDetails(SwapchainSupportDetails& outSupportDetails,const VkPhysicalDevice& physicalDevice, const VkSurfaceKHR& surface)
    {
        GetSwapchainCapabilities(physicalDevice, surface, outSupportDetails.m_Capabilities);
        GetSwapchainFormats(physicalDevice, surface, outSupportDetails.m_Formats, outSupportDetails.m_FormatProperties);
        GetSwapchainPresentModes(physicalDevice, surface, outSupportDetails.m_PresentModes);
    }
}
