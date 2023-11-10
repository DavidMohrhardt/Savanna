/**
 * @file VkSwapchain.cpp
 * @author David Mohrhardt (https://github.com/DavidMohrhardt/Savanna)
 * @brief TODO @David.Mohrhardt Document
 * @version 0.1
 * @date 2023-10-23
 *
 * @copyright Copyright (c) 2023
 *
 */
#include "VkSwapchain.h"

#include "VkAllocator.h"

#include "Utilities/VkSwapchainUtils.h"

namespace Savanna::Gfx::Vk2
{
    inline static constexpr VkImageViewCreateInfo k_DefaultSwapchainImageViewCreateInfo {
            // VkStructureType            sType;
            VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO,
            // const void*                pNext;
            nullptr,
            // VkImageViewCreateFlags     flags;
            0,
            // VkImage                    image;
            VK_NULL_HANDLE,
            // VkImageViewType            viewType;
            VK_IMAGE_VIEW_TYPE_2D,
            // VkFormat                   format;
            VK_FORMAT_R8G8B8A8_SRGB,
            // VkComponentMapping         components;
            {
                VK_COMPONENT_SWIZZLE_IDENTITY, // r
                VK_COMPONENT_SWIZZLE_IDENTITY, // g
                VK_COMPONENT_SWIZZLE_IDENTITY, // b
                VK_COMPONENT_SWIZZLE_IDENTITY  // a
            },
            // VkImageSubresourceRange    subresourceRange;
            {
                // VkImageAspectFlags    aspectMask;
                VK_IMAGE_ASPECT_COLOR_BIT,
                // uint32_t              baseMipLevel;
                0,
                // uint32_t              levelCount;
                1,
                // uint32_t              baseArrayLayer;
                0,
                // uint32_t              layerCount;
                1
            }
        };

    se_GfxErrorCode_t VkSwapchain::Initialize(
        const se_GfxSwapchainCreateInfo_t& createInfo,
        const VkGpu& gpu,
        const VkSurfaceKHR& surface)
    {
        SAVANNA_INSERT_SCOPED_PROFILER(VkSwapchain::Initialize);

        Utils::SwapchainSupportDetails supportDetails;
        Utils::PopulateSwapchainSupportDetails(supportDetails, gpu, surface);

        // TODO @DavidMohrhardt: Submit the format requested by the user.
        m_SurfaceFormat = Utils::ChooseSwapSurfaceFormat(supportDetails.m_Formats);
        m_PresentMode = Utils::ChooseSwapPresentMode(supportDetails.m_PresentModes);
        m_Extent = Utils::ChooseSwapExtent(supportDetails.m_Capabilities, createInfo.m_Width, createInfo.m_Height);

        VkSwapchainCreateInfoKHR swapchainCreateInfo{};
        swapchainCreateInfo.sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR;
        swapchainCreateInfo.surface = surface;
        swapchainCreateInfo.minImageCount = supportDetails.m_Capabilities.minImageCount + 1;
        swapchainCreateInfo.imageFormat = m_SurfaceFormat.format;
        swapchainCreateInfo.imageColorSpace = m_SurfaceFormat.colorSpace;
        swapchainCreateInfo.imageExtent = m_Extent;
        swapchainCreateInfo.imageArrayLayers = 1;
        swapchainCreateInfo.imageUsage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;

        const VkQueueFamilyIndices& indices = gpu.GetQueueFamilyIndices();
        uint32_t queueFamilyIndices[] = { indices.GetGraphicsQueueFamilyIndex(), indices.GetPresentQueueFamilyIndex() };

        if (indices.GetGraphicsQueueFamilyIndex() != indices.GetPresentQueueFamilyIndex())
        {
            swapchainCreateInfo.imageSharingMode = VK_SHARING_MODE_CONCURRENT;
            swapchainCreateInfo.queueFamilyIndexCount = 2;
            swapchainCreateInfo.pQueueFamilyIndices = queueFamilyIndices;
        }
        else
        {
            swapchainCreateInfo.imageSharingMode = VK_SHARING_MODE_EXCLUSIVE;
            swapchainCreateInfo.queueFamilyIndexCount = 0; // Optional
            swapchainCreateInfo.pQueueFamilyIndices = nullptr; // Optional
        }

        swapchainCreateInfo.preTransform = supportDetails.m_Capabilities.currentTransform;
        swapchainCreateInfo.compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR;
        swapchainCreateInfo.presentMode = m_PresentMode;
        swapchainCreateInfo.clipped = VK_TRUE;

        if (vkCreateSwapchainKHR(gpu, &swapchainCreateInfo, VkAllocator::Get(), &m_Swapchain) != VK_SUCCESS)
        {
            return kSavannaGfxErrorCodeFailure;
        }

        try
        {
            ConfigureSwapchainImageViews(gpu);
        }
        catch (const std::exception& e)
        {
            SAVANNA_FATAL_LOG("Failed to configure swapchain image views: {}", e.what());
            return kSavannaGfxErrorCodeFailure;
        }

        return kSavannaGfxErrorCodeSuccess;
    }

    void VkSwapchain::Reset(const VkGpu& gpu)
    {

    }

    void VkSwapchain::Destroy(const VkGpu &gpu)
    {
        SAVANNA_INSERT_SCOPED_PROFILER(VkSwapchain::Destroy())
        const VkDevice& device = gpu;

        ReleaseImageViews(device);

        if (m_Swapchain != VK_NULL_HANDLE)
        {
            vkDestroySwapchainKHR(device, m_Swapchain, VkAllocator::Get());
            m_Swapchain = VK_NULL_HANDLE;
        }
    }

    void VkSwapchain::ConfigureSwapchainImageViews(const VkGpu& gpu)
    {
        SAVANNA_INSERT_SCOPED_PROFILER(VkSwapchain::ConfigureSwapchainImageViews())
        const VkDevice& device = gpu;

        ReleaseImageViews(device);

        uint32 swapchainImageCount;
        vkGetSwapchainImagesKHR(device, m_Swapchain, &swapchainImageCount, nullptr);
        if (swapchainImageCount != m_SwapchainImages.size())
        {
            m_SwapchainImages.resize(swapchainImageCount);
            m_SwapchainImageViews.resize(swapchainImageCount);
        }
        vkGetSwapchainImagesKHR(device, m_Swapchain, &swapchainImageCount, m_SwapchainImages.data());

        // TODO @DavidMohrhardt - Enable setting up stereoscopic image views for VR rendering.
        VkImageViewCreateInfo imageViewCreateInfo = k_DefaultSwapchainImageViewCreateInfo;
        imageViewCreateInfo.format = m_SurfaceFormat.format;
        for (int i = 0; i < swapchainImageCount; ++i)
        {
            imageViewCreateInfo.image = m_SwapchainImages[i];
            VK_MUST_SUCCEED(vkCreateImageView(device, &imageViewCreateInfo, VkAllocator::Get(), &m_SwapchainImageViews[i]), "Unable to create image view for swapchain image.");
        }
    }

    inline void VkSwapchain::ReleaseImageViews(const VkDevice& device)
    {
        if (device != VK_NULL_HANDLE)
        {
            for (const auto& imageView : m_SwapchainImageViews)
            {
                if (imageView != VK_NULL_HANDLE)
                {
                    vkDestroyImageView(device, imageView, VkAllocator::Get());
                }
            }
        }
    }
} // namespace Savanna::Gfx::Vk2
