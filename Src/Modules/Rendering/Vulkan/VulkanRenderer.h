/**
 * @file VulkanRenderer.h
 * @author David Mohrhardt (https://github.com/DavidMohrhardt/Savanna)
 * @brief
 * @version 0.1
 * @date 2022-08-20
 */
#pragma once

#include <Utilities/SavannaCoding.h>

#include <vulkan/vulkan.h>

#include <VulkanInstance.h>
#include <VulkanPhysicalDevice.h>
#include <VulkanGraphicsDevice.h>

namespace Savanna::Rendering::Vulkan
{
    class VulkanRenderer
    {
    private:
        static void SelectPhysicalDevice(
            const VulkanInstance& instance,
            VulkanPhysicalDevice* outPhysicalDevice);

        static void CreateLogicalDevice(
            const VulkanPhysicalDevice& physicalDevice,
            VulkanGraphicsDevice* outGfxDevice);

        static void QueryGraphicsQueue(
            const VulkanPhysicalDevice& physicalDevice,
            const VulkanGraphicsDevice& device,
            VkQueue* outGraphicsQueue);

    private:
        VulkanInstance m_Instance;
        VulkanPhysicalDevice m_PhysicalDevice;
        VulkanGraphicsDevice m_GraphicsDevice;

        VkQueue m_GraphicsQueue;

        // VkQueue m_GraphicsQueue;
        // VkQueue m_PresentQueue;

        VkSurfaceKHR m_Surface;

        // VkSwapchainKHR m_Swapchain;
        // VkExtent2D m_Extent;
        // VkFormat m_Format;
        // VkImageView m_ImageView;
        // VkImage m_Image;

    public:
        VulkanRenderer(const FixedString32& applicationName, const FixedString32& engineName);
        VulkanRenderer(VulkanInstance&& instance);
        VulkanRenderer(VulkanInstance&& instance, VulkanPhysicalDevice&& physicalDevice, VulkanGraphicsDevice&& graphicsDevice);

        VulkanRenderer() = default;
        ~VulkanRenderer() = default;

    public:
        // operators
        VulkanRenderer(VulkanRenderer&&) = default;
        VulkanRenderer& operator=(VulkanRenderer&&) = default;

    public:
        void CreateSurface(const VkDisplaySurfaceCreateInfoKHR& displaySurfaceCreateInfo);

        VulkanInstance& GetVulkanInstance() { return m_Instance; }
        VulkanPhysicalDevice& GetVulkanPhysicalDevice() { return m_PhysicalDevice; }
        VulkanGraphicsDevice& GetVulkanGraphicsDevice() { return m_GraphicsDevice; }

        VkQueue GetGraphicsQueue() const { return m_GraphicsQueue; }
    };
} // namespace Savanna::Rendering::Vulkan