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
        VulkanPhysicalDevice* outPhysicalDevice)
    {
        SAVANNA_INSERT_SCOPED_PROFILER("VulkanRenderer::SelectPhysicalDevice()");
        assert(outPhysicalDevice != nullptr && "outPhysicalDevice is nullptr!");

        uint32 physicalDeviceCount = VulkanPhysicalDevice::GetPhysicalDeviceCount(instance.GetVkInstance());

        std::vector<VulkanPhysicalDeviceDescriptor> deviceDescriptors =
            std::vector<VulkanPhysicalDeviceDescriptor>(physicalDeviceCount);

        VulkanPhysicalDevice::GetPhysicalDeviceDescriptors(
            instance.GetVkInstance(), physicalDeviceCount, deviceDescriptors.data());

        VulkanPhysicalDeviceDescriptor selectedDeviceDescriptor{};

        if (!TryChooseVulkanDeviceDescriptor(
            deviceDescriptors.data(), physicalDeviceCount, selectedDeviceDescriptor))
        {
            throw RuntimeErrorException("No suitable Vulkan devices found.");
        }

        *outPhysicalDevice = VulkanPhysicalDevice(selectedDeviceDescriptor);
    }

    void VulkanRenderer::CreateLogicalDevice(const VulkanPhysicalDevice& physicalDevice, VulkanGraphicsDevice* outGfxDevice)
    {
        SAVANNA_INSERT_SCOPED_PROFILER("VulkanRenderer::CreateLogicalDevice()");
        assert(outGfxDevice != nullptr && "outGfxDevice is nullptr!");

        VulkanQueueFamilyIndices indices = physicalDevice.GetQueueFamilyIndices();
        if (!indices.m_GraphicsQueueFamilyIndex.has_value())
        {
            throw RuntimeErrorException("No suitable graphics queues found.");
        }

        VkDeviceQueueCreateInfo queueCreateInfo{};
        queueCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
        queueCreateInfo.queueFamilyIndex = indices.m_GraphicsQueueFamilyIndex.value();
        queueCreateInfo.queueCount = 1;

        const float queuePriority = 1.0f;
        queueCreateInfo.pQueuePriorities = &queuePriority;

        VkPhysicalDeviceFeatures deviceFeatures{};
        VkDeviceCreateInfo createInfo{};
        createInfo.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
        createInfo.queueCreateInfoCount = 1;
        createInfo.pQueueCreateInfos = &queueCreateInfo;
        createInfo.pEnabledFeatures = &deviceFeatures;
        createInfo.enabledExtensionCount = 0;

        *outGfxDevice = VulkanGraphicsDevice(physicalDevice, createInfo);
    }

    void VulkanRenderer::QueryGraphicsQueue(
        const VulkanPhysicalDevice& physicalDevice,
        const VulkanGraphicsDevice& device,
        VkQueue* outGraphicsQueue)
    {
        SAVANNA_INSERT_SCOPED_PROFILER("VulkanRenderer::QueryGraphicsQueue()");
        VulkanQueueFamilyIndices indices = physicalDevice.GetQueueFamilyIndices();
        if (!indices.m_GraphicsQueueFamilyIndex.has_value())
        {
            throw RuntimeErrorException("No suitable graphics queues found.");
        }

        *outGraphicsQueue = device.GetVkQueue(indices.m_GraphicsQueueFamilyIndex.value());
    }

    VulkanRenderer::VulkanRenderer(const VulkanRendererCreateInfo* const createInfoPtr)
        : m_Instance()
        , m_PhysicalDevice()
        , m_GraphicsDevice()
        , m_GraphicsQueue()
        , m_DisplaySurface(VK_NULL_HANDLE)
    {
        SAVANNA_INSERT_SCOPED_PROFILER("VulkanRenderer::VulkanRenderer ctor()");
        assert(createInfoPtr != nullptr && "createInfoPtr is nullptr!");
        m_Instance = VulkanInstance(
            createInfoPtr->m_ApplicationName,
            createInfoPtr->m_EngineName,
            createInfoPtr->m_ActiveExtensions,
            createInfoPtr->m_ActiveExtensionCount);

        if (!m_Instance.IsValid())
        {
            throw RuntimeErrorException("Failed to create Vulkan instance!");
        }

        SelectPhysicalDevice(m_Instance, &m_PhysicalDevice);
        if (TryCreateDisplaySurface(createInfoPtr->m_SurfaceCreateInfo))
        {
            m_PhysicalDevice.ParseQueueFamilyIndices(&m_DisplaySurface);
        }
        else
        {
            m_PhysicalDevice.ParseQueueFamilyIndices(nullptr);
        }

        CreateLogicalDevice(m_PhysicalDevice, &m_GraphicsDevice);
        QueryGraphicsQueue(m_PhysicalDevice, m_GraphicsDevice, &m_GraphicsQueue);
    }

    VulkanRenderer::~VulkanRenderer()
    {
        SAVANNA_INSERT_SCOPED_PROFILER("VulkanRenderer::~VulkanRenderer()");
        if (m_Instance.IsValid())
        {
            if (m_DisplaySurface != VK_NULL_HANDLE)
            {
                vkDestroySurfaceKHR(m_Instance.GetVkInstance(), m_DisplaySurface, nullptr);
            }
        }
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
