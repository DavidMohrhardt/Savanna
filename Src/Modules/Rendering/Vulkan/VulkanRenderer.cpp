/**
 * @file VulkanRenderer.cpp
 * @author David Mohrhardt (https://github.com/DavidMohrhardt/Savanna)
 * @brief
 * @version 0.1
 * @date 2022-08-20
 *
 */

#include "VulkanRenderer.h"

#include <Profiling/Profiler.h>

#include "Utilities/VulkanDeviceChooser.h"

#include <vector>

namespace Savanna::Rendering::Vulkan
{
    void VulkanRenderer::SelectPhysicalDevice(
        const VulkanInstance& instance,
        const VulkanRendererCreateInfo* pCreateInfo,
        VulkanPhysicalDevice* outPhysicalDevice)
    {
        SAVANNA_INSERT_SCOPED_PROFILER("VulkanRenderer::SelectPhysicalDevice()");
        assert(outPhysicalDevice != nullptr && "outPhysicalDevice is nullptr!");

        uint32 numberOfPhysicalDevices =
            VulkanPhysicalDevice::GetPhysicalDeviceCount(instance.GetVkInstance());
        if (numberOfPhysicalDevices == 0)
        {
            throw RuntimeErrorException("No physical devices found!");
        }

        std::vector<VulkanPhysicalDevice> physicalDevices(numberOfPhysicalDevices);
        VulkanPhysicalDevice::EnumeratePhysicalDevices(instance.GetVkInstance(), numberOfPhysicalDevices, physicalDevices.data());
        if (!TryChooseVulkanDevice(physicalDevices.data(), physicalDevices.size(), pCreateInfo, outPhysicalDevice))
        {
            throw RuntimeErrorException("No suitable physical devices found!");
        }
    }

    // TODO @DavidMohrhardt make this function the constructor of the VulkanGraphicsDevice class.
    void VulkanRenderer::CreateLogicalDevice(
        const VulkanPhysicalDevice& physicalDevice,
        const VulkanRendererCreateInfo* pCreateInfo,
        VulkanGraphicsDevice* outGfxDevice)
    {
        SAVANNA_INSERT_SCOPED_PROFILER("VulkanRenderer::CreateLogicalDevice()");
        assert(outGfxDevice != nullptr && "outGfxDevice is nullptr!");

        m_QueueFamilyIndices = VulkanQueueFamilyIndices(m_PhysicalDevice.GetPhysicalDevice(), &m_DisplaySurface);

        std::vector<VkDeviceQueueCreateInfo> queueCreationInfos(2);
        queueCreationInfos.clear();

        uint32 indices[] = {
            m_QueueFamilyIndices.m_PresentQueueFamilyIndex.value(),
            m_QueueFamilyIndices.m_PresentQueueFamilyIndex.value()
        };

        float queuePriority = 1.0f;
        for (int i = 0; i < 2; i++)
        {
            VkDeviceQueueCreateInfo queueCreateInfo {};
            queueCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
            queueCreateInfo.queueFamilyIndex = indices[i];
            queueCreateInfo.queueCount = 1;
            queueCreateInfo.pQueuePriorities = &queuePriority;
            queueCreationInfos.emplace_back(queueCreateInfo);
        }

        VkPhysicalDeviceFeatures deviceFeatures{};
        VkDeviceCreateInfo createInfo{};
        createInfo.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
        createInfo.queueCreateInfoCount = queueCreationInfos.size();
        createInfo.pQueueCreateInfos = queueCreationInfos.data();
        createInfo.pEnabledFeatures = &deviceFeatures;
        createInfo.enabledExtensionCount = pCreateInfo->m_DeviceExtensionsCount;
        createInfo.ppEnabledExtensionNames = pCreateInfo->m_DeviceExtensions;

        *outGfxDevice = VulkanGraphicsDevice(physicalDevice, createInfo);
    }

    void VulkanRenderer::GetAvailableQueues()
    {
        // Graphics queue if available
        if (m_QueueFamilyIndices.HasGraphicsQueueFamilyIndex())
        {
            QueryVulkanQueue(
                m_PhysicalDevice,
                m_GraphicsDevice,
                m_QueueFamilyIndices.m_GraphicsQueueFamilyIndex.value(),
                &m_GraphicsQueue);
        }

        // Present Queue if available
        if (m_QueueFamilyIndices.HasPresentQueueFamilyIndex())
        {
            QueryVulkanQueue(
                m_PhysicalDevice,
                m_GraphicsDevice,
                m_QueueFamilyIndices.m_PresentQueueFamilyIndex.value(),
                &m_PresentQueue);
        }

        // Compute queue if available
        if (m_QueueFamilyIndices.HasComputeQueueFamilyIndex())
        {
            QueryVulkanQueue(
                m_PhysicalDevice,
                m_GraphicsDevice,
                m_QueueFamilyIndices.m_ComputeQueueFamilyIndex.value(),
                &m_ComputeQueue);
        }

        // Transfer queue if available
        if (m_QueueFamilyIndices.HasTransferQueueFamilyIndex())
        {
            QueryVulkanQueue(
                m_PhysicalDevice,
                m_GraphicsDevice,
                m_QueueFamilyIndices.m_TransferQueueFamilyIndex.value(),
                &m_TransferQueue);
        }
    }

    void VulkanRenderer::QueryVulkanQueue(
        const VulkanPhysicalDevice& physicalDevice,
        const VulkanGraphicsDevice& device,
        const uint32 queueIndex,
        VkQueue* outQueue)
    {
        SAVANNA_INSERT_SCOPED_PROFILER("VulkanRenderer::QueryVulkanQueue()");
        *outQueue = device.GetVkQueue(queueIndex);
    }

    VulkanRenderer::VulkanRenderer(const VulkanRendererCreateInfo* const pCreateInfo)
        : m_Instance()
        , m_PhysicalDevice()
        , m_GraphicsDevice()
        , m_GraphicsQueue()
        , m_DisplaySurface(VK_NULL_HANDLE)
    {
        SAVANNA_INSERT_SCOPED_PROFILER("VulkanRenderer::VulkanRenderer ctor()");
        assert(pCreateInfo != nullptr && "pCreateInfo is nullptr!");
        m_Instance = VulkanInstance(
            pCreateInfo->m_ApplicationName,
            pCreateInfo->m_EngineName,
            pCreateInfo->m_InstanceExtensions,
            pCreateInfo->m_InstanceExtensionsCount);

        if (!m_Instance.IsValid())
        {
            throw RuntimeErrorException("Failed to create Vulkan instance!");
        }

        SelectPhysicalDevice(m_Instance, pCreateInfo, &m_PhysicalDevice);
        if (!TryCreateDisplaySurface(pCreateInfo->m_SurfaceCreateInfo))
        {
            // Rendering to display not supported.
            throw RuntimeErrorException("Failed to create Vulkan display surface!");
        }

        CreateLogicalDevice(
            m_PhysicalDevice,
            pCreateInfo,
            &m_GraphicsDevice);
        GetAvailableQueues();
    }

    VulkanRenderer::VulkanRenderer(VulkanRenderer&& other)
    {
        *this = std::move(other);
    }

    VulkanRenderer::~VulkanRenderer()
    {
        SAVANNA_INSERT_SCOPED_PROFILER("VulkanRenderer::~VulkanRenderer()");
        if (m_DisplaySurface != VK_NULL_HANDLE)
        {
            vkDestroySurfaceKHR(m_Instance.GetVkInstance(), m_DisplaySurface, nullptr);
        }
    }

    VulkanRenderer& VulkanRenderer::operator=(VulkanRenderer&& other)
    {
        if (this != &other)
        {
            m_Instance = std::move(other.m_Instance);
            m_PhysicalDevice = std::move(other.m_PhysicalDevice);
            m_GraphicsDevice = std::move(other.m_GraphicsDevice);
            m_GraphicsQueue = std::move(other.m_GraphicsQueue);
            m_DisplaySurface = other.m_DisplaySurface;
            other.m_DisplaySurface = VK_NULL_HANDLE;
        }
        return *this;
    }

    bool VulkanRenderer::TryCreateDisplaySurface(const VulkanSurfaceCreateInfoUnion& surfaceCreateInfo)
    {
        if (!m_Instance.IsValid())
        {
            return false;
        }

        if (m_DisplaySurface == VK_NULL_HANDLE)
        {
            VkResult result;
#if SAVANNA_WINDOWS
            VK_CALL(
                result,
                vkCreateWin32SurfaceKHR(m_Instance.GetVkInstance(), &surfaceCreateInfo.win32SurfaceCreateInfo, nullptr, &m_DisplaySurface),
                "Failed to create Vulkan surface!");
#endif

            return result == VK_SUCCESS;
        }

        return false;
    }
} // namespace Savanna::Rendering::Vulkan
