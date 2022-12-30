/**
 * @file VkGfxDevice.cpp
 * @author David Mohrhardt (https://github.com/DavidMohrhardt/Savanna)
 * @brief
 * @version 0.1
 * @date 2022-08-20
 *
 */
#include "VkGfxDevice.h"

#include <Profiling/Profiler.h>

#include "VkRendererCreateInfo.h"
#include "Utilities/VkRendererInitializationCache.h"
#include "Utilities/VkPhysicalDeviceUtils.h"
#include "Utilities/VkMacros.h"

#include <vector>
#include <set>

namespace Savanna::Gfx::Vk
{
    GfxDevice::GfxDevice()
        : m_PhysicalDevice(VK_NULL_HANDLE)
        , m_LogicalDevice(VK_NULL_HANDLE)
    {}

    GfxDevice::GfxDevice(const GfxDevice &other)
        : m_PhysicalDevice(other.m_PhysicalDevice)
        , m_LogicalDevice(other.m_LogicalDevice)
    {}

    GfxDevice::GfxDevice(GfxDevice&& other)
        : m_PhysicalDevice(other.m_PhysicalDevice)
        , m_LogicalDevice(other.m_LogicalDevice)
    {
        other.m_PhysicalDevice = VK_NULL_HANDLE;
        other.m_LogicalDevice = VK_NULL_HANDLE;
    }

    GfxDevice::GfxDevice(const RendererCreateInfo *const pCreateInfo, const Context &context)
        : GfxDevice()
    {
        CreatePhysicalDevice(pCreateInfo, context);
        // CreateLogicalDevice(pCreateInfo, context);
    }

    GfxDevice::GfxDevice(
        const RendererCreateInfo *const pCreateInfo,
        const Context &context,
        const VkPhysicalDevice &physicalDevice)
        : m_PhysicalDevice(physicalDevice)
        , m_LogicalDevice(VK_NULL_HANDLE)
    {
        // CreateLogicalDevice(pCreateInfo, context);
    }

    GfxDevice::~GfxDevice()
    {
        if (m_LogicalDevice != VK_NULL_HANDLE)
        {
            vkDestroyDevice(m_LogicalDevice, nullptr);
        }

        m_LogicalDevice = VK_NULL_HANDLE;
        m_PhysicalDevice = VK_NULL_HANDLE;
    }

    VkSwapchainKHR GfxDevice::CreateSwapchain(
        const Context &context,
        const VkSurfaceKHR &surface,
        const VkSurfaceFormatKHR &surfaceFormat,
        const VkExtent2D &extent,
        const VkPresentModeKHR &presentMode,
        const VkSurfaceCapabilitiesKHR &surfaceCapabilities)
    {
        SAVANNA_INSERT_SCOPED_PROFILER(GfxDevice::CreateSwapchain());
        throw std::runtime_error("Not implemented yet");
    }

    void GfxDevice::CreatePhysicalDevice(const RendererCreateInfo* const pCreateInfo, const Context &context)
    {
        SAVANNA_INSERT_SCOPED_PROFILER("Renderer::CreatePhysicalDevice()");
        uint32 numDevices;
        Utils::EnumeratePhysicalDeviceDescriptors(context, nullptr, numDevices);
        std::vector<PhysicalDeviceDescriptor> physicalDeviceDescriptors(numDevices);
        Utils::EnumeratePhysicalDeviceDescriptors(context, physicalDeviceDescriptors.data(), numDevices);

        auto pRendererInitCache = Utils::RendererInitializationCache::Get();
        RendererQueueFlags queueFlags = pCreateInfo->m_QueueFlags;

        std::vector<PhysicalDeviceDescriptor> candidateDeviceDescs {};
        std::vector<QueueFamilyIndices> candidateQueueFamilyIndices {};
        for (const auto& physicalDeviceDescriptor : physicalDeviceDescriptors)
        {
            QueueFamilyIndices queueFamilyIndices;
            queueFamilyIndices.PopulateQueueFamilyIndices(physicalDeviceDescriptor.physicalDevice);

            if ((!queueFlags.HasFlag(SV_VK_QUEUE_GRAPHICS_BIT) || queueFamilyIndices.HasGraphicsQueueFamilyIndex())
                && ((!queueFlags.HasFlag(SV_VK_QUEUE_COMPUTE_BIT) || queueFamilyIndices.HasComputeQueueFamilyIndex())
                && (!queueFlags.HasFlag(SV_VK_QUEUE_TRANSFER_BIT) || queueFamilyIndices.HasTransferQueueFamilyIndex())
                && (!queueFlags.HasFlag(SV_VK_QUEUE_PRESENT_BIT) || queueFamilyIndices.HasPresentQueueFamilyIndex())))
            {
                candidateDeviceDescs.push_back(physicalDeviceDescriptor);
                candidateQueueFamilyIndices.push_back(queueFamilyIndices);
            }
        }

        uint32 selectedDeviceIndex;
        if (!Utils::TryChooseVulkanDevice(candidateDeviceDescs.data(), candidateDeviceDescs.size(), nullptr, &selectedDeviceIndex))
        {
            throw std::runtime_error("No suitable physical device found");
        }

        m_PhysicalDevice = candidateDeviceDescs[selectedDeviceIndex].physicalDevice;
        pRendererInitCache->SetQueueFamilyIndices(candidateQueueFamilyIndices[selectedDeviceIndex]);
        pRendererInitCache->SetVkPhysicalDevice(m_PhysicalDevice);

    }

    void GfxDevice::CreateLogicalDevice(const RendererCreateInfo* const pCreateInfo, const Context& context)
    {
        SAVANNA_INSERT_SCOPED_PROFILER(GfxDevice::CreateLogicalDevice());
        auto pRendererInitCache = Utils::RendererInitializationCache::Get();
        QueueFamilyIndices queueFamilyIndices = pRendererInitCache->GetQueueFamilyIndices();

        std::vector<VkDeviceQueueCreateInfo> queueCreateInfos;
        std::set<uint32> uniqueQueueFamilies = {};
        float queuePriority = 1.0f;

        RendererQueueFlags queueFlags = pCreateInfo->m_QueueFlags;
        if (queueFlags.HasFlag(SV_VK_QUEUE_GRAPHICS_BIT))
        {
            uniqueQueueFamilies.insert(queueFamilyIndices.m_ComputeQueueFamilyIndex.value());
        }
        if (queueFlags.HasFlag(SV_VK_QUEUE_COMPUTE_BIT))
        {
            uniqueQueueFamilies.insert(queueFamilyIndices.m_ComputeQueueFamilyIndex.value());
        }
        if (queueFlags.HasFlag(SV_VK_QUEUE_TRANSFER_BIT))
        {
            uniqueQueueFamilies.insert(queueFamilyIndices.m_TransferQueueFamilyIndex.value());
        }
        if (queueFlags.HasFlag(SV_VK_QUEUE_PRESENT_BIT))
        {
            uniqueQueueFamilies.insert(queueFamilyIndices.m_PresentQueueFamilyIndex.value());
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
        VkDeviceCreateInfo createInfo = {};
        createInfo.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
        createInfo.pQueueCreateInfos = queueCreateInfos.data();
        createInfo.queueCreateInfoCount = static_cast<uint32>(queueCreateInfos.size());
        createInfo.pEnabledFeatures = &deviceFeatures;
        // createInfo.enabledExtensionCount = static_cast<uint32>(deviceExtensions.size());
        // createInfo.ppEnabledExtensionNames = deviceExtensions.data();

        // Don't need device validation layers as there is no distinction between device and instance layers
        createInfo.enabledLayerCount = 0;

        if (vkCreateDevice(m_PhysicalDevice, &createInfo, nullptr, &m_LogicalDevice) != VK_SUCCESS)
        {
            throw std::runtime_error("failed to create logical device!");
        }
    }

} // namespace Savanna::Gfx::Vk
