/**
 * @file VkDriver.h
 * @author David Mohrhardt (https://github.com/DavidMohrhardt/Savanna)
 * @brief
 * @version 0.1
 * @date 2023-09-16
 *
 * @copyright Copyright (c) 2023
 *
 */
#pragma once

#include <SavannaEngine.h>

#include "Types/Containers/Arrays/DynamicArray.h"

#include "Gfx/GfxDriver.h"

#include "VkAllocationCallback.h"

#include <vulkan/vulkan.h>

namespace Savanna::Gfx::Vk
{
    class VkDriver final //: public IGfxDriver
    {
    private:
        VkInstance m_Instance;
        VkPhysicalDevice m_PhysicalDevice;
        VkDevice m_Device;
        VkQueue m_GraphicsQueue;
        VkQueue m_PresentQueue;
        VkSurfaceKHR m_Surface;
        VkSwapchainKHR m_Swapchain;
        VkFormat m_SwapchainImageFormat;
        VkExtent2D m_SwapchainExtent;
        DynamicArray<VkImage> m_SwapchainImages;
        DynamicArray<VkImageView> m_SwapchainImageViews;
        DynamicArray<VkFramebuffer> m_SwapchainFramebuffers;
        VkRenderPass m_RenderPass;
        VkPipelineLayout m_PipelineLayout;
        VkPipeline m_GraphicsPipeline;
        DynamicArray<VkCommandBuffer> m_CommandBuffers;
        DynamicArray<VkSemaphore> m_ImageAvailableSemaphores;
        DynamicArray<VkSemaphore> m_RenderFinishedSemaphores;
        DynamicArray<VkFence> m_InFlightFences;
        DynamicArray<VkFence> m_ImagesInFlight;
        size m_CurrentFrame;

        void CreateInstance();
        void CreateSurface();
        void PickPhysicalDevice();
        void CreateLogicalDevice();
        void CreateSwapchain();
        void CreateImageViews();
        void CreateRenderPass();
        void CreateGraphicsPipeline();
        void CreateFramebuffers();
        void CreateCommandBuffers();
        void CreateSyncObjects();

        void DestroyInstance();

        void CleanupSwapchain();
    };
} // namespace Savanna::Gfx::Vk
