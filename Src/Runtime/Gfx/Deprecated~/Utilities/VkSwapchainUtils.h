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

#include "SavannaVk.h"

#include "VkQueueFamilyIndices.h"
#include "VkRendererCreateInfo.h"
#include "VkRendererCreateUtils.h"

namespace Savanna::Gfx::Vk::Utils
{
    struct SwapchainSupportDetails
    {
        VkSurfaceCapabilitiesKHR capabilities;
        std::vector<VkSurfaceFormatKHR> formats;
        std::vector<VkFormatProperties> formatProperties;
        std::vector<VkPresentModeKHR> presentModes;
    };

    VkSurfaceFormatKHR ChooseSwapSurfaceFormat(const std::vector<VkSurfaceFormatKHR>& availableFormats);

    VkPresentModeKHR ChooseSwapPresentMode(const std::vector<VkPresentModeKHR>& availablePresentModes);

    VkExtent2D ChooseSwapExtent(const VkSurfaceCapabilitiesKHR& capabilities, uint32_t width, uint32_t height);

    inline static void GetSwapchainCapabilities(
        const VkPhysicalDevice& physicalDevice,
        const VkSurfaceKHR& surface,
        VkSurfaceCapabilitiesKHR& surfaceCapabilities);

    inline static void GetSwapchainFormats(
        const VkPhysicalDevice& physicalDevice,
        const VkSurfaceKHR& surface,
        std::vector<VkSurfaceFormatKHR>& surfaceFormats,
        std::vector<VkFormatProperties>& formatProperties);

    inline static void GetSwapchainPresentModes(
        const VkPhysicalDevice& physicalDevice,
        const VkSurfaceKHR& surface,
        std::vector<VkPresentModeKHR>& presentModes);

    SwapchainSupportDetails GetSwapchainSupportDetails(
        const VkPhysicalDevice& physicalDevice,
        const VkSurfaceKHR& surface);
}
