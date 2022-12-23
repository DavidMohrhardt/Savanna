/**
 * @file VulkanRenderer.h
 * @author David Mohrhardt (https://github.com/DavidMohrhardt/Savanna)
 * @brief
 * @version 0.1
 * @date 2022-08-20
 */
#pragma once

// Savanna Core Includes
#include <Utilities/SavannaCoding.h>

// Vulkan Includes
#include <vulkan/vulkan.h>

// Savanna Vulkan includes
#include <VulkanInstance.h>
#include <VulkanPhysicalDevice.h>
#include <VulkanGraphicsDevice.h>
#include <VulkanQueueFamilyIndices.h>
#include <VulkanRendererCreateInfo.h>

namespace Savanna::Rendering::Vulkan
{
    class VulkanRenderer
    {
    private:
        VulkanInstance m_Instance;
        VulkanPhysicalDevice m_PhysicalDevice;
        VulkanGraphicsDevice m_GraphicsDevice;

        VulkanQueueFamilyIndices m_QueueFamilyIndices;
        VkQueue m_GraphicsQueue;
        VkQueue m_PresentQueue;
        VkQueue m_ComputeQueue;
        VkQueue m_TransferQueue;
        // VkQueue m_SparseBindingQueue;

        VkSurfaceKHR m_DisplaySurface;
        VkSwapchainKHR m_Swapchain;

        // VkExtent2D m_Extent;
        VkSurfaceFormatKHR m_SurfaceFormat;
        // VkImageView m_ImageView;
        // VkImage m_Image;

    public:
        VulkanRenderer() = default;
        VulkanRenderer(const VulkanRendererCreateInfo* const pCreateInfo);

        ~VulkanRenderer();

        VulkanRenderer(const VulkanRenderer& other) = delete;
        VulkanRenderer(VulkanRenderer&& other);

    public:
        // operators
        VulkanRenderer& operator=(const VulkanRenderer& other) = delete;
        VulkanRenderer& operator=(VulkanRenderer&& other);

    private:
        /**
         * @brief
         *
         * @param physicalDevice
         * @param device
         * @param queueIndex
         * @param outQueue
         */
        static void QueryVulkanQueue(
            const VulkanPhysicalDevice& physicalDevice,
            const VulkanGraphicsDevice& device,
            const uint32 queueIndex,
            VkQueue* outQueue);

        static void SelectPhysicalDevice(
            const VulkanInstance& instance,
            const VulkanRendererCreateInfo* pCreateInfo,
            VulkanPhysicalDevice* outPhysicalDevice);

        static void SetupVkQueueCreateInfos(
            const VulkanPhysicalDevice& physicalDevice,
            const VulkanGraphicsDevice& device);

    private:
        void CreateLogicalDevice(
            const VulkanPhysicalDevice& physicalDevice,
            const VulkanRendererCreateInfo* pCreateInfo,
            VulkanGraphicsDevice &outGfxDevice);

        void GetAvailableQueues();

        void CreateSwapchain();

    public:
        void Initialize(const VulkanRendererCreateInfo* const pCreateInfo);
        bool TryCreateDisplaySurface(const VulkanSurfaceCreateInfoUnion& displaySurfaceInfo);

        const VulkanInstance& GetVulkanInstance() { return m_Instance; }
        const VulkanPhysicalDevice& GetVulkanPhysicalDevice() { return m_PhysicalDevice; }
        const VulkanGraphicsDevice& GetVulkanGraphicsDevice() { return m_GraphicsDevice; }

        const VulkanQueueFamilyIndices& GetVulkanQueueFamilyIndices() const { return m_QueueFamilyIndices; }

        VkQueue GetGraphicsQueue() const { return m_GraphicsQueue; }

        VkSurfaceKHR GetDisplaySurface() const { return m_DisplaySurface; }

        VkSwapchainKHR GetSwapchain() const { return m_Swapchain; }
    };
} // namespace Savanna::Rendering::Vulkan
