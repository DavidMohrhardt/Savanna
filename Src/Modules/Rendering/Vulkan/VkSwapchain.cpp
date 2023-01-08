/**
 * @file VkSwapchain.cpp
 * @author David Mohrhardt (https://github.com/DavidMohrhardt/Savanna)
 * @brief
 * @version 0.1
 * @date 2023-01-03
 *
 * @copyright Copyright (c) 2023
 *
 */
#include "VkSwapchain.h"

#include <Profiling/Profiler.h>

#include "VkQueueFamilyIndices.h"

namespace Savanna::Gfx::Vk
{
    inline static const VkImageViewCreateInfo k_DefaultSwapchainImageViewCreateInfo = {
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
                VK_COMPONENT_SWIZZLE_IDENTITY // a
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

    Swapchain::Swapchain()
        : m_Swapchain(VK_NULL_HANDLE)
        , m_SurfaceFormat({})
        , m_PresentMode(VK_PRESENT_MODE_FIFO_KHR)
        , m_Extent({})
        , m_Device(VK_NULL_HANDLE)
        , m_Surface(VK_NULL_HANDLE)
        , m_SwapchainImages()
        , m_SwapchainImageViews()
    {
    }

    Swapchain::Swapchain(
        const RendererCreateInfo& rendererCreateInfo,
        const GfxDevice& gfxDevice,
        const DisplaySurface& displaySurface)
        : m_Swapchain(VK_NULL_HANDLE)
        , m_SurfaceFormat({})
        , m_PresentMode(VK_PRESENT_MODE_FIFO_KHR)
        , m_Extent({})
        , m_Device(gfxDevice.GetGfxDevice())
        , m_Surface(displaySurface.GetSurface())
        , m_SwapchainImages()
        , m_SwapchainImageViews()
    {
        SAVANNA_INSERT_SCOPED_PROFILER(Swapchain::Swapchain(const RendererCreateInfo& rendererCreateInfo, const GfxDevice& gfxDevice, const DisplaySurface& displaySurface));
        // TODO @DavidMohrhardt: Get extents from renderer create info and pass them to the swapchain creation

        const auto swapchainSupportDetails = Utils::GetSwapchainSupportDetails(gfxDevice, displaySurface);
        const auto surfaceFormat = Utils::ChooseSwapSurfaceFormat(swapchainSupportDetails.formats);
        const auto presentMode = Utils::ChooseSwapPresentMode(swapchainSupportDetails.presentModes);
        const auto extent = Utils::ChooseSwapExtent(swapchainSupportDetails.capabilities, rendererCreateInfo.m_Width, rendererCreateInfo.m_Height);

        uint32_t imageCount = swapchainSupportDetails.capabilities.minImageCount + 1;
        if (swapchainSupportDetails.capabilities.maxImageCount > 0 && imageCount > swapchainSupportDetails.capabilities.maxImageCount)
        {
            imageCount = swapchainSupportDetails.capabilities.maxImageCount;
        }

        VkSwapchainCreateInfoKHR swapchainCreateInfo = {};
        swapchainCreateInfo.sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR;
        swapchainCreateInfo.surface = m_Surface;
        swapchainCreateInfo.minImageCount = imageCount;
        swapchainCreateInfo.imageFormat = surfaceFormat.format;
        swapchainCreateInfo.imageColorSpace = surfaceFormat.colorSpace;
        swapchainCreateInfo.imageExtent = extent;
        swapchainCreateInfo.imageArrayLayers = 1;
        swapchainCreateInfo.imageUsage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;

        const QueueFamilyIndices indices = gfxDevice.GetQueueFamilyIndices();
        uint32_t queueFamilyIndices[] = { indices.m_GraphicsQueueFamilyIndex.value(), indices.m_PresentQueueFamilyIndex.value() };

        if (indices.m_GraphicsQueueFamilyIndex != indices.m_PresentQueueFamilyIndex)
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

        swapchainCreateInfo.preTransform = swapchainSupportDetails.capabilities.currentTransform;
        swapchainCreateInfo.compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR;
        swapchainCreateInfo.presentMode = presentMode;
        swapchainCreateInfo.clipped = VK_TRUE;
        swapchainCreateInfo.oldSwapchain = VK_NULL_HANDLE;

        if (vkCreateSwapchainKHR(m_Device, &swapchainCreateInfo, nullptr, &m_Swapchain) != VK_SUCCESS)
        {
            throw std::runtime_error("Failed to create swap chain!");
        }

        m_SurfaceFormat = surfaceFormat;
        m_Extent = extent;
        m_PresentMode = presentMode;

        ConfigureSwapchainImageViews();
    }

    Swapchain::~Swapchain()
    {
        ReleaseImageViews();
        if (m_Swapchain != VK_NULL_HANDLE && m_Device != VK_NULL_HANDLE)
            vkDestroySwapchainKHR(m_Device, m_Swapchain, nullptr);
    }

    void Swapchain::ConfigureSwapchainImageViews()
    {
        SAVANNA_INSERT_SCOPED_PROFILER(Swapchain::ConfigureSwapchainImageViews())
        ReleaseImageViews();

        uint32 swapchainImageCount;
        vkGetSwapchainImagesKHR(m_Device, m_Swapchain, &swapchainImageCount, nullptr);
        if (swapchainImageCount != m_SwapchainImages.size())
        {
            m_SwapchainImages.resize(swapchainImageCount);
            m_SwapchainImageViews.resize(swapchainImageCount);
        }
        vkGetSwapchainImagesKHR(m_Device, m_Swapchain, &swapchainImageCount, m_SwapchainImages.data());

        // TODO @DavidMohrhardt - Enable setting up stereoscopic image views for VR rendering.
        VkImageViewCreateInfo imageViewCreateInfo = k_DefaultSwapchainImageViewCreateInfo;
        imageViewCreateInfo.format = m_SurfaceFormat.format;
        for (int i = 0; i < swapchainImageCount; ++i)
        {
            imageViewCreateInfo.image = m_SwapchainImages[i];
            if (vkCreateImageView(m_Device, &imageViewCreateInfo, nullptr, &m_SwapchainImageViews[i]) != VK_SUCCESS)
                throw std::runtime_error("Unable to create image view for swapchain image.");
        }
    }

    void Swapchain::ReleaseImageViews()
    {
        if (m_Device != VK_NULL_HANDLE)
        {
            for (const auto& imageView : m_SwapchainImageViews)
            {
                if (imageView != VK_NULL_HANDLE)
                    vkDestroyImageView(m_Device, imageView, nullptr);
            }
        }
    }

    Swapchain &Swapchain::operator=(Swapchain &&other)
    {
        if (this != &other)
        {
            m_Swapchain = other.m_Swapchain;
            m_SurfaceFormat = other.m_SurfaceFormat;
            m_PresentMode = other.m_PresentMode;
            m_Extent = other.m_Extent;
            m_Device = other.m_Device;
            m_Surface = other.m_Surface;
            m_SwapchainImages = std::move(other.m_SwapchainImages);
            m_SwapchainImageViews = std::move(other.m_SwapchainImageViews);

            other.m_Swapchain = VK_NULL_HANDLE;
            other.m_SurfaceFormat = {};
            other.m_PresentMode = VK_PRESENT_MODE_FIFO_KHR;
            other.m_Extent = {};
            other.m_Device = VK_NULL_HANDLE;
            other.m_Surface = VK_NULL_HANDLE;
            other.m_SwapchainImages = std::vector<VkImage>();
            other.m_SwapchainImageViews = std::vector<VkImageView>();
        }
        return *this;
    }

    inline bool Swapchain::operator==(const Swapchain &other) const
    {
        return m_Swapchain == other.m_Swapchain;
    }

    inline bool Swapchain::operator!=(const Swapchain &other) const
    {
        return m_Swapchain != other.m_Swapchain;
    }
} // namespace Savanna::Gfx::Vk
