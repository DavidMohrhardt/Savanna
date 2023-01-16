#pragma once

#include "SavannaVk.h"

#include <SavannaEngine.h>

#include "VkRendererCreateInfo.h"
#include "VkQueueFamilyIndices.h"

#include "Utilities/VkRendererDebugMessenger.h"

namespace Savanna::Gfx::Vulkan
{
    class VkRenderer
    {
    private:
        VkInstance m_Instance;
        VkPhysicalDevice m_PhysicalDevice;
        VkDevice m_Device;
        VkQueue m_GraphicsQueue;
        VkQueue m_PresentQueue;
        VkQueue m_ComputeQueue;
        VkQueue m_TransferQueue;
        VkCommandPool m_CommandPool;
        VkCommandBuffer m_CommandBuffer;
        VkSurfaceKHR m_Surface;
        VkSwapchainKHR m_Swapchain;

        VkRendererDebugMessenger m_DebugMessenger;

        VkQueueFamilyIndices m_QueueFamilyIndices;

    public:
        VkRenderer();
        ~VkRenderer();

        void Initialize(const VkRendererCreateInfo& rendererCreateInfo);
        void Shutdown();

        const VkInstance GetInstance() const { return m_Instance; }
        const VkPhysicalDevice GetPhysicalDevice() const { return m_PhysicalDevice; }
        const VkDevice GetDevice() const { return m_Device; }
        const VkQueue GetGraphicsQueue() const { return m_GraphicsQueue; }
        const VkQueue GetPresentQueue() const { return m_PresentQueue; }
        const VkQueue GetComputeQueue() const { return m_ComputeQueue; }

    private:

        void CreateInstance(const VkRendererCreateInfo& rendererCreateInfo);
        void CreateDebugMessenger(const VkRendererCreateInfo& rendererCreateInfo);
        void CreateSurface(const VkRendererCreateInfo& rendererCreateInfo);
        void CreateLogicalDevice(const VkRendererCreateInfo& rendererCreateInfo);
        void CreateSwapChain(const VkRendererCreateInfo& rendererCreateInfo);
        void CreateImageViews(const VkRendererCreateInfo& rendererCreateInfo);
        void CreateRenderPass(const VkRendererCreateInfo& rendererCreateInfo);
        void CreateDescriptorSetLayout(const VkRendererCreateInfo& rendererCreateInfo);
        void CreateGraphicsPipeline(const VkRendererCreateInfo& rendererCreateInfo);
        void CreateCommandPool(const VkRendererCreateInfo& rendererCreateInfo);
        void CreateColorResource(const VkRendererCreateInfo& rendererCreateInfo);
        void CreateDepthResource(const VkRendererCreateInfo& rendererCreateInfo);
        void CreateFramebuffers(const VkRendererCreateInfo& rendererCreateInfo);

        void GetQueueHandles(const VkRendererQueueFlags& requestedQueueFlags);

        void PopulateDeviceExtensionInfo(VkDeviceCreateInfo& deviceCreateInfo, const VkRendererCreateInfo& rendererCreateInfo);
        void PopulateAdditionalInstanceInfo(VkInstanceCreateInfo& instanceCreateInfo, const VkRendererCreateInfo& rendererCreateInfo);

        // TODO @DavidMohrhardt - Add loading of extensions, validation layers, and previous device ID
        // from a config file
        void SelectPhysicalDevice(const VkRendererCreateInfo& rendererCreateInfo);
        // bool TryChangePhysicalDevice(const VkRendererCreateInfo& rendererCreateInfo);
    };

} // namespace Savanna::Gfx::Vulkan
