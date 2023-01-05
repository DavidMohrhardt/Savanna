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
    Swapchain::Swapchain()
        : m_Swapchain(VK_NULL_HANDLE)
        , m_SurfaceFormat({})
        , m_PresentMode(VK_PRESENT_MODE_FIFO_KHR)
        , m_Extent({})
        , m_Device(VK_NULL_HANDLE)
        , m_Surface(VK_NULL_HANDLE)
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
    }

    Swapchain::~Swapchain()
    {
        if (m_Swapchain != VK_NULL_HANDLE && m_Device != VK_NULL_HANDLE)
            vkDestroySwapchainKHR(m_Device, m_Swapchain, nullptr);
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

            other.m_Swapchain = VK_NULL_HANDLE;
            other.m_SurfaceFormat = {};
            other.m_PresentMode = VK_PRESENT_MODE_FIFO_KHR;
            other.m_Extent = {};
            other.m_Device = VK_NULL_HANDLE;
            other.m_Surface = VK_NULL_HANDLE;
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
