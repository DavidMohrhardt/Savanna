/**
 * @file VkSwapchain.h
 * @author David Mohrhardt (https://github.com/DavidMohrhardt/Savanna)
 * @brief TODO @David.Mohrhardt Document
 * @version 0.1
 * @date 2023-10-22
 *
 * @copyright Copyright (c) 2023
 *
 */
#pragma once

#include "SavannaVk2.h"

#include "VkGpu.h"

namespace savanna::Gfx::Vk2
{
    class Swapchain
    {
    private:
        friend class VkDriver;

        VkSwapchainKHR m_Swapchain = VK_NULL_HANDLE;
        VkSurfaceFormatKHR m_SurfaceFormat;
        VkPresentModeKHR m_PresentMode;
        VkExtent2D m_Extent;

        VkDevice m_Device;
        VkSurfaceKHR m_Surface;

        // Just use 3 max images for now.
        dynamic_array<VkImage> m_SwapchainImages;
        dynamic_array<VkImageView> m_SwapchainImageViews;

        Swapchain();
        ~Swapchain() = default;

        Swapchain(const Swapchain&) = delete;
        Swapchain(Swapchain&&) = delete;

        se_GfxErrorCode_t Initialize(
            const se_GfxSwapchainCreateInfo_t& createInfo,
            const VkGpu& gpu,
            const VkSurfaceKHR& surface);

        void Reset(
            const VkGpu& gpu);

        void Destroy(
            const VkGpu& gpu);

        void Present(
            const VkQueue& queue,
            const VkSemaphore& waitSemaphore);

        void AwaitPresentableImage(
            const VkDevice& device,
            const VkSemaphore& waitSemaphore);

        void ConfigureSwapchainImageViews(const VkGpu& gpu);
        void ReleaseImageViews(const VkDevice& device);

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
    };
} // namespace savanna::Gfx::Vk2
