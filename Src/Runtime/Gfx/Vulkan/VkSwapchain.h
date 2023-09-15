/**
 * @file VkSwapchain.h
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

#include "VkGfxDevice.h"
#include "VkRendererCreateInfo.h"

#include "Utilities/VkRendererCreateUtils.h"
#include "Utilities/VkSwapchainUtils.h"

namespace Savanna::Gfx::Vk
{
    class Swapchain
    {
    private:
        VkSwapchainKHR m_Swapchain = VK_NULL_HANDLE;
        VkSurfaceFormatKHR m_SurfaceFormat;
        VkPresentModeKHR m_PresentMode;
        VkExtent2D m_Extent;

        VkDevice m_Device;
        VkSurfaceKHR m_Surface;

        std::vector<VkImage> m_SwapchainImages;
        std::vector<VkImageView> m_SwapchainImageViews;

    public:
        // Constructors and destructors
        Swapchain();

        Swapchain(const RendererCreateInfo &createInfo,
                const GfxDevice& gfxDevice,
                const DisplaySurface& displaySurface);

        Swapchain(const Swapchain &other) = delete;

        Swapchain(Swapchain &&other) SAVANNA_NOEXCEPT;

        ~Swapchain();

    public:
        VkSwapchainKHR GetSwapchain() const { return m_Swapchain; }
        VkSurfaceFormatKHR GetSurfaceFormat() const { return m_SurfaceFormat; }
        VkPresentModeKHR GetPresentMode() const { return m_PresentMode; }
        VkExtent2D GetExtent() const { return m_Extent; }
        VkViewport GetDefaultViewportForSwapchain() const {
            return VkViewport
            {
                0.0f,
                0.0f,
                static_cast<float>(m_Extent.width),
                static_cast<float>(m_Extent.height),
                0.0f,
                1.0f
            };
        }

    private:
        void ConfigureSwapchainImageViews();
        void ReleaseImageViews();

    public:
        // Operators
        VkSwapchainKHR &operator*() { return m_Swapchain; }
        const VkSwapchainKHR &operator*() const { return m_Swapchain; }

        Swapchain& operator=(const Swapchain &other) = delete;
        Swapchain& operator=(Swapchain&& other);

        bool operator==(const Swapchain &other) const;
        bool operator!=(const Swapchain &other) const;
    };
} // namespace Savanna::Gfx::Vk
