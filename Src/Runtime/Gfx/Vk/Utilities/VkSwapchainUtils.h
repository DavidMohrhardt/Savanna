/**
 * @file VkSwapchainUtils.h
 * @author David Mohrhardt (https://github.com/DavidMohrhardt/Savanna)
 * @brief
 * @version 0.1
 * @date 2023-01-03
 *
 * @copyright Copyright (c) 2023
 *
 */
#pragma once

#include "SavannaVk2.h"

namespace Savanna::Gfx::Vk2::Utils
{
    struct SwapchainSupportDetails
    {
        VkSurfaceCapabilitiesKHR capabilities;
        dynamic_array<VkSurfaceFormatKHR> formats;
        dynamic_array<VkFormatProperties> formatProperties;
        dynamic_array<VkPresentModeKHR> presentModes;
    };

    VkSurfaceFormatKHR ChooseSwapSurfaceFormat(const dynamic_array<VkSurfaceFormatKHR>& availableFormats);

    VkPresentModeKHR ChooseSwapPresentMode(const dynamic_array<VkPresentModeKHR>& availablePresentModes);

    VkExtent2D ChooseSwapExtent(const VkSurfaceCapabilitiesKHR& capabilities, uint32_t width, uint32_t height);

    inline static void GetSwapchainCapabilities(
        const VkPhysicalDevice& physicalDevice,
        const VkSurfaceKHR& surface,
        VkSurfaceCapabilitiesKHR& surfaceCapabilities);

    inline static void GetSwapchainFormats(
        const VkPhysicalDevice& physicalDevice,
        const VkSurfaceKHR& surface,
        dynamic_array<VkSurfaceFormatKHR>& surfaceFormats,
        dynamic_array<VkFormatProperties>& formatProperties);

    inline static void GetSwapchainPresentModes(
        const VkPhysicalDevice& physicalDevice,
        const VkSurfaceKHR& surface,
        dynamic_array<VkPresentModeKHR>& presentModes);

    void PopulateSwapchainSupportDetails(
        SwapchainSupportDetails& outSupportDetails,
        const VkPhysicalDevice& physicalDevice,
        const VkSurfaceKHR& surface);
}
