#include "VkRenderer.h"

// Core includes
#include "Profiling/Profiler.h"

// Utilities includes
#include "Utilities/VkPhysicalDeviceChooser.h"

#include <iostream>
#include <stdexcept>
#include <functional>
#include <set>
#include <vector>

namespace Savanna::Gfx::Vulkan
{
    const std::vector<const char*> k_DefaultValidationLayers = {
        "VK_LAYER_KHRONOS_validation"
    };
    const std::vector<const char*> k_SwapchainExtension = {
        VK_KHR_SWAPCHAIN_EXTENSION_NAME
    };

    VkRenderer::VkRenderer()
        : m_Instance(VK_NULL_HANDLE)
        , m_PhysicalDevice(VK_NULL_HANDLE)
        , m_Device(VK_NULL_HANDLE)
        , m_GraphicsQueue(VK_NULL_HANDLE)
        , m_PresentQueue(VK_NULL_HANDLE)
        , m_ComputeQueue(VK_NULL_HANDLE)
        , m_TransferQueue(VK_NULL_HANDLE)
        , m_CommandPool(VK_NULL_HANDLE)
        , m_CommandBuffer(VK_NULL_HANDLE)
        , m_Surface(VK_NULL_HANDLE)
        , m_Swapchain(VK_NULL_HANDLE)
        , m_DebugMessenger()
        , m_QueueFamilyIndices()
    {
    }

    VkRenderer::~VkRenderer()
    {
        Shutdown();
    }

    void VkRenderer::Initialize(const VkRendererCreateInfo& rendererCreateInfo)
    {
        SAVANNA_INSERT_SCOPED_PROFILER(VkRenderer::Initialize);
        CreateInstance(rendererCreateInfo);

        if (VkRendererCreateFlags::HasFlag(rendererCreateInfo.m_CreateFlags, se_VkRendererCreateFlagsEnableSurface))
            CreateSurface(rendererCreateInfo);

        SelectPhysicalDevice(rendererCreateInfo);
        CreateLogicalDevice(rendererCreateInfo);

        // SVK_CHECK_FLAG_AND_CALL(EnableSwapchain, CreateSwapChain);
        // SVK_CHECK_FLAG_AND_CALL(EnableImageViews, CreateImageViews);
        // SVK_CHECK_FLAG_AND_CALL(EnableRenderPass, CreateRenderPass);
        // SVK_CHECK_FLAG_AND_CALL(EnableDescriptorSetLayout, CreateDescriptorSetLayout);
        // SVK_CHECK_FLAG_AND_CALL(EnableGraphicsPipeline, CreateGraphicsPipeline);
        // SVK_CHECK_FLAG_AND_CALL(EnableCommandPool, CreateCommandPool);
        // SVK_CHECK_FLAG_AND_CALL(EnableColorResource, CreateColorResource);
        // SVK_CHECK_FLAG_AND_CALL(EnableDepthResource, CreateDepthResource);
        // SVK_CHECK_FLAG_AND_CALL(EnableFramebuffers, CreateFramebuffers);

        // CreateSwapChain(rendererCreateInfo);
        // CreateImageViews(rendererCreateInfo);
        // CreateRenderPass(rendererCreateInfo);
        // CreateDescriptorSetLayout(rendererCreateInfo);
        // CreateGraphicsPipeline(rendererCreateInfo);
        // CreateCommandPool(rendererCreateInfo);
        // CreateColorResource(rendererCreateInfo);
        // CreateDepthResource(rendererCreateInfo);
        // CreateFramebuffers(rendererCreateInfo);
    }

    void VkRenderer::Shutdown()
    {
        SAVANNA_INSERT_SCOPED_PROFILER(VkRenderer::Shutdown);
        if (m_Device != VK_NULL_HANDLE)
        {
            vkDestroyDevice(m_Device, nullptr);
            m_Device = VK_NULL_HANDLE;
        }

        if (m_Surface != VK_NULL_HANDLE)
        {
            vkDestroySurfaceKHR(m_Instance, m_Surface, nullptr);
            m_Surface = VK_NULL_HANDLE;
        }

        if (m_Instance != VK_NULL_HANDLE)
        {
            vkDestroyInstance(m_Instance, nullptr);
            m_Instance = VK_NULL_HANDLE;
        }
    }

    void VkRenderer::CreateInstance(const VkRendererCreateInfo& rendererCreateInfo)
    {
        SAVANNA_INSERT_SCOPED_PROFILER(VkRenderer::CreateInstance);
        if (m_Instance != VK_NULL_HANDLE)
        {
            SAVANNA_WARNING_LOG("Renderer already initialized");
            return;
        }

        VkApplicationInfo appInfo = {};
        appInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
        appInfo.pApplicationName = "Savanna";
        appInfo.applicationVersion = VK_MAKE_VERSION(1, 0, 0);
        appInfo.pEngineName = "No Engine";
        appInfo.engineVersion = VK_MAKE_VERSION(1, 0, 0);
        appInfo.apiVersion = VK_API_VERSION_1_0;

        VkInstanceCreateInfo instanceCreateInfo = {};
        instanceCreateInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
        instanceCreateInfo.pApplicationInfo = &appInfo;

        instanceCreateInfo.enabledExtensionCount = rendererCreateInfo.m_InstanceExtensionCount;
        instanceCreateInfo.ppEnabledExtensionNames = rendererCreateInfo.m_ppInstanceExtensions;

        PopulateAdditionalInstanceInfo(instanceCreateInfo, rendererCreateInfo);

        VkResult result = vkCreateInstance(&instanceCreateInfo, rendererCreateInfo.m_pAllocationCallbacks, &m_Instance);
        if (result != VK_SUCCESS)
        {
            throw std::runtime_error("Failed to create VkInstance");
        }

        if (instanceCreateInfo.enabledLayerCount > 0)
        {
            CreateDebugMessenger(rendererCreateInfo);
        }
    }

    void VkRenderer::CreateDebugMessenger(const VkRendererCreateInfo& rendererCreateInfo)
    {
        SAVANNA_INSERT_SCOPED_PROFILER(VkRenderer::CreateDebugMessenger);
        VkDebugUtilsMessengerCreateInfoEXT debugMessengerCreateInfo = {};
        VkRendererDebugMessenger::PopulateCreateInfo(&debugMessengerCreateInfo);
        m_DebugMessenger = VkRendererDebugMessenger(
            m_Instance,
            &debugMessengerCreateInfo,
            rendererCreateInfo.m_pAllocationCallbacks);
    }

    void VkRenderer::CreateSurface(const VkRendererCreateInfo& rendererCreateInfo)
    {
        SAVANNA_INSERT_SCOPED_PROFILER(VkRenderer::CreateSurface);
        if (m_Surface != VK_NULL_HANDLE)
        {
            SAVANNA_WARNING_LOG("Surface already created");
            return;
        }

        if (rendererCreateInfo.m_SurfaceCreateInfo.m_SurfaceCreateFunction == nullptr)
        {
            throw std::runtime_error("Surface creation function is null");
        }

        if (rendererCreateInfo.m_SurfaceCreateInfo.m_pWindow == nullptr)
        {
            throw std::runtime_error("Window is null");
        }

        VkResult result = rendererCreateInfo.m_SurfaceCreateInfo.m_SurfaceCreateFunction(
            m_Instance,
            rendererCreateInfo.m_SurfaceCreateInfo.m_pWindow,
            rendererCreateInfo.m_pAllocationCallbacks,
            &m_Surface);

        if (result != VK_SUCCESS)
        {
            throw std::runtime_error("Failed to create VkSurfaceKHR");
        }
    }

    void VkRenderer::CreateLogicalDevice(const VkRendererCreateInfo &rendererCreateInfo)
    {
        SAVANNA_INSERT_SCOPED_PROFILER(VkRenderer::CreateLogicalDevice);
        if (m_Device != VK_NULL_HANDLE)
        {
            SAVANNA_WARNING_LOG("Logical device already created");
            return;
        }

        float queuePriority = 1.0f;
        std::vector<VkDeviceQueueCreateInfo> queueCreateInfos;
        std::set<uint32> uniqueQueueFamilies = { };

        if (VkRendererQueueFlags::HasFlag(rendererCreateInfo.m_QueueFlags, se_VkRendererQueueFlagsGraphics))
        {
            uniqueQueueFamilies.insert(m_QueueFamilyIndices.m_GraphicsQueueFamilyIndex.value());
        }

        if (VkRendererQueueFlags::HasFlag(rendererCreateInfo.m_QueueFlags, se_VkRendererQueueFlagsCompute))
        {
            uniqueQueueFamilies.insert(m_QueueFamilyIndices.m_ComputeQueueFamilyIndex.value());
        }

        if (VkRendererQueueFlags::HasFlag(rendererCreateInfo.m_QueueFlags, se_VkRendererQueueFlagsTransfer))
        {
            uniqueQueueFamilies.insert(m_QueueFamilyIndices.m_TransferQueueFamilyIndex.value());
        }

        if (VkRendererQueueFlags::HasFlag(rendererCreateInfo.m_QueueFlags, se_VkRendererQueueFlagsPresent))
        {
            uniqueQueueFamilies.insert(m_QueueFamilyIndices.m_PresentQueueFamilyIndex.value());
        }

        for (uint32 queueFamily : uniqueQueueFamilies)
        {
            VkDeviceQueueCreateInfo queueCreateInfo = {};
            queueCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
            queueCreateInfo.queueFamilyIndex = queueFamily;
            queueCreateInfo.queueCount = 1;
            queueCreateInfo.pQueuePriorities = &queuePriority;
            queueCreateInfos.push_back(queueCreateInfo);
        }

        VkPhysicalDeviceFeatures deviceFeatures = {};

        VkDeviceCreateInfo deviceCreateInfo = {};
        deviceCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
        deviceCreateInfo.pQueueCreateInfos = queueCreateInfos.data();
        deviceCreateInfo.queueCreateInfoCount = static_cast<uint32>(queueCreateInfos.size());
        deviceCreateInfo.pEnabledFeatures = &deviceFeatures;

        PopulateDeviceExtensionInfo(deviceCreateInfo, rendererCreateInfo);

        VkResult result = vkCreateDevice(m_PhysicalDevice, &deviceCreateInfo, rendererCreateInfo.m_pAllocationCallbacks, &m_Device);
        if (result != VK_SUCCESS)
        {
            throw std::runtime_error("Failed to create logical device");
        }

        GetQueueHandles(rendererCreateInfo.m_QueueFlags);
    }

    void VkRenderer::GetQueueHandles(const VkRendererQueueFlags &requestedQueueFlags)
    {
        SAVANNA_INSERT_SCOPED_PROFILER(VkRenderer::GetQueueHandles);
        if (requestedQueueFlags == se_VkRendererQueueFlagsNone)
        {
            SAVANNA_WARNING_LOG("No queues requested");
            return;
        }

        if (VkRendererQueueFlags::HasFlag(requestedQueueFlags, se_VkRendererQueueFlagsGraphics))
        {
            vkGetDeviceQueue(m_Device, m_QueueFamilyIndices.m_GraphicsQueueFamilyIndex.value(), 0, &m_GraphicsQueue);
        }

        if (VkRendererQueueFlags::HasFlag(requestedQueueFlags, se_VkRendererQueueFlagsCompute))
        {
            vkGetDeviceQueue(m_Device, m_QueueFamilyIndices.m_ComputeQueueFamilyIndex.value(), 0, &m_ComputeQueue);
        }

        if (VkRendererQueueFlags::HasFlag(requestedQueueFlags, se_VkRendererQueueFlagsTransfer))
        {
            vkGetDeviceQueue(m_Device, m_QueueFamilyIndices.m_TransferQueueFamilyIndex.value(), 0, &m_TransferQueue);
        }

        if (VkRendererQueueFlags::HasFlag(requestedQueueFlags, se_VkRendererQueueFlagsPresent))
        {
            vkGetDeviceQueue(m_Device, m_QueueFamilyIndices.m_PresentQueueFamilyIndex.value(), 0, &m_PresentQueue);
        }

        // if (VkRendererQueueFlags::HasFlag(requestedQueueFlags, se_VkRendererQueueFlagsSparseBinding))
        // {
        //     vkGetDeviceQueue(m_Device, m_QueueFamilyIndices.m_SparseBindingQueueFamilyIndex.value(), 0, &m_SparseBindingQueue);
        // }
    }

    void VkRenderer::PopulateDeviceExtensionInfo(VkDeviceCreateInfo &deviceCreateInfo, const VkRendererCreateInfo &rendererCreateInfo)
    {
        SAVANNA_INSERT_SCOPED_PROFILER(VkRenderer::PopulateDeviceExtensionInfo);
        // If the user has specified that they want to define their own device extensions, use those
        // Otherwise, if the user has specified that they want to enable the swapchain, use that
        if (VkRendererCreateFlags::HasFlag(rendererCreateInfo.m_CreateFlags, se_VkRendererCreateFlagsEnableSwapchain))
        {
            deviceCreateInfo.enabledExtensionCount = rendererCreateInfo.m_DeviceExtensionCount;
            deviceCreateInfo.ppEnabledExtensionNames = rendererCreateInfo.m_ppDeviceExtensions;
        }
        else if (VkRendererCreateFlags::HasFlag(rendererCreateInfo.m_CreateFlags, se_VkRendererCreateFlagsEnableSwapchain))
        {
            deviceCreateInfo.enabledExtensionCount = 1;
            deviceCreateInfo.ppEnabledExtensionNames = k_SwapchainExtension.data();
        }
        else
        {
            deviceCreateInfo.enabledExtensionCount = 0;
        }
    }

    void VkRenderer::PopulateAdditionalInstanceInfo(VkInstanceCreateInfo &instanceCreateInfo, const VkRendererCreateInfo &rendererCreateInfo)
    {
        SAVANNA_INSERT_SCOPED_PROFILER(VkRenderer::PopulateAdditionalInstanceInfo);
        // If the user has specified validation layers, use them. Otherwise, use the default validation layer if enabled.
        if (VkRendererCreateFlags::HasFlag(rendererCreateInfo.m_CreateFlags, se_VkRendererCreateFlagsEnableValidationLayers))
        {
            if (rendererCreateInfo.m_ValidationLayerCount > 0)
            {
                instanceCreateInfo.enabledLayerCount = rendererCreateInfo.m_ValidationLayerCount;
                instanceCreateInfo.ppEnabledLayerNames = rendererCreateInfo.m_ppValidationLayers;
            }
            else
            {
                instanceCreateInfo.enabledLayerCount = 1;
                instanceCreateInfo.ppEnabledLayerNames = k_DefaultValidationLayers.data();
            }
        }
        else
        {
            instanceCreateInfo.enabledLayerCount = 0;
        }
    }

    void VkRenderer::SelectPhysicalDevice(const VkRendererCreateInfo& rendererCreateInfo)
    {
        SAVANNA_INSERT_SCOPED_PROFILER(VkRenderer::SelectPhysicalDevice);
        if (m_PhysicalDevice != VK_NULL_HANDLE)
        {
            SAVANNA_WARNING_LOG("Physical device already selected");
            return;
        }

        uint32_t deviceCount = 0;
        vkEnumeratePhysicalDevices(m_Instance, &deviceCount, nullptr);

        if (deviceCount == 0)
        {
            throw std::runtime_error("Failed to find GPUs with Vulkan support");
        }

        std::vector<VkPhysicalDevice> devices(deviceCount);
        vkEnumeratePhysicalDevices(m_Instance, &deviceCount, devices.data());

        Utils::SortPhysicalDevicesByScore(devices, rendererCreateInfo);
        for (const auto& device : devices)
        {
            VkQueueFamilyIndices queueFamilyIndices(device);
            if (VkRendererQueueFlags::HasFlag(rendererCreateInfo.m_QueueFlags, se_VkRendererQueueFlagsGraphics) && !queueFamilyIndices.HasGraphicsQueueFamilyIndex())
            {
                continue;
            }
            if (VkRendererQueueFlags::HasFlag(rendererCreateInfo.m_QueueFlags, se_VkRendererQueueFlagsCompute) && !queueFamilyIndices.HasComputeQueueFamilyIndex())
            {
                continue;
            }
            if (VkRendererQueueFlags::HasFlag(rendererCreateInfo.m_QueueFlags, se_VkRendererQueueFlagsTransfer) && !queueFamilyIndices.HasTransferQueueFamilyIndex())
            {
                continue;
            }
            if (VkRendererQueueFlags::HasFlag(rendererCreateInfo.m_QueueFlags, se_VkRendererQueueFlagsPresent) && !queueFamilyIndices.HasPresentQueueFamilyIndex())
            {
                continue;
            }

            m_QueueFamilyIndices = queueFamilyIndices;
            m_PhysicalDevice = device;
            return;
        }

        throw std::runtime_error("Failed to find a suitable GPU");
    }
} // namespace Savanna::Gfx::Vulkan
