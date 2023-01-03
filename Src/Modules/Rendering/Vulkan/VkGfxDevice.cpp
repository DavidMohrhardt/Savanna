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

#include "Utilities/VkExtensionUtils.h"
#include "Utilities/VkMacros.h"
#include "Utilities/VkPhysicalDeviceUtils.h"
#include "Utilities/VkRendererCreateUtils.h"

namespace Savanna::Gfx::Vk
{
    GfxDevice::GfxDevice()
        : m_PhysicalDevice(VK_NULL_HANDLE)
        , m_LogicalDevice(VK_NULL_HANDLE)
    {}

    GfxDevice::GfxDevice(GfxDevice&& other)
        : m_PhysicalDevice(other.m_PhysicalDevice)
        , m_LogicalDevice(other.m_LogicalDevice)
    {
        other.m_PhysicalDevice = VK_NULL_HANDLE;
        other.m_LogicalDevice = VK_NULL_HANDLE;
    }

    GfxDevice::GfxDevice(
        const RendererCreateInfo *const pRendererCreateInfo,
        const Context &context,
        const DisplaySurface* const pDisplaySurface)
        : GfxDevice()
    {
        QueueFamilyIndices selectedDeviceIndices;
        CreatePhysicalDevice(pRendererCreateInfo, context, pDisplaySurface, selectedDeviceIndices);
        CreateLogicalDevice(pRendererCreateInfo, context, selectedDeviceIndices);
        ConfigureQueues(pRendererCreateInfo->m_QueueFlags);
    }

    GfxDevice::GfxDevice(
        const RendererCreateInfo *const pRendererCreateInfo,
        const Context &context,
        const DisplaySurface* const pDisplaySurface,
        const VkPhysicalDevice &physicalDevice)
        : m_PhysicalDevice(physicalDevice)
        , m_LogicalDevice(VK_NULL_HANDLE)
    {
        VkSurfaceKHR surface = pDisplaySurface != nullptr ? pDisplaySurface->GetSurface() : VK_NULL_HANDLE;
        QueueFamilyIndices indices(m_PhysicalDevice, &surface);
        CreateLogicalDevice(pRendererCreateInfo, context, indices);
        ConfigureQueues(pRendererCreateInfo->m_QueueFlags);
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

    void GfxDevice::CreatePhysicalDevice(
        const RendererCreateInfo* const pRendererCreateInfo,
        const Context &context,
        const DisplaySurface* const pDisplaySurface,
        QueueFamilyIndices& outQueueFamilyIndices)
    {
        SAVANNA_INSERT_SCOPED_PROFILER(GfxDevice::CreatePhysicalDevice());
        uint32 numDevices;
        Utils::EnumeratePhysicalDeviceDescriptors(context, nullptr, numDevices);
        std::vector<PhysicalDeviceDescriptor> physicalDeviceDescriptors(numDevices);
        Utils::EnumeratePhysicalDeviceDescriptors(context, physicalDeviceDescriptors.data(), numDevices);

        if (!Utils::TryChooseVulkanDevice(physicalDeviceDescriptors.data(), physicalDeviceDescriptors.size(), &m_PhysicalDevice, nullptr))
        {
            throw std::runtime_error("No suitable physical device found");
        }

        VkSurfaceKHR surface = pDisplaySurface != nullptr ? pDisplaySurface->GetSurface() : VK_NULL_HANDLE;
        outQueueFamilyIndices.PopulateQueueFamilyIndices(m_PhysicalDevice, &surface);
    }

    void GfxDevice::CreateLogicalDevice(
        const RendererCreateInfo* const pRendererCreateInfo,
        const Context& context,
        const QueueFamilyIndices& queueFamilyIndices)
    {
        SAVANNA_INSERT_SCOPED_PROFILER(GfxDevice::CreateLogicalDevice());
        uint32 numUniqueQueueFamilies = 0;
        float queuePriority = 1.0f;
        Utils::GetUniqueQueueFamilies(nullptr, &numUniqueQueueFamilies, pRendererCreateInfo, queueFamilyIndices, &queuePriority);
        std::vector<VkDeviceQueueCreateInfo> queueCreateInfos(numUniqueQueueFamilies);
        Utils::GetUniqueQueueFamilies(queueCreateInfos.data(), &numUniqueQueueFamilies, pRendererCreateInfo, queueFamilyIndices, &queuePriority);

        VkPhysicalDeviceFeatures deviceFeatures = {};
        VkDeviceCreateInfo createInfo = {};
        Utils::PopulateVkDeviceCreateInfo(&createInfo, pRendererCreateInfo, queueCreateInfos.data(), numUniqueQueueFamilies);//, &deviceFeatures);

#if SAVANNA_VULKAN_DEBUGGING
        Utils::ValidateDeviceExtensions(m_PhysicalDevice, createInfo.ppEnabledExtensionNames, createInfo.enabledExtensionCount);
#endif

        if (vkCreateDevice(m_PhysicalDevice, &createInfo, nullptr, &m_LogicalDevice) != VK_SUCCESS)
        {
            throw std::runtime_error("failed to create logical device!");
        }
    }

    void GfxDevice::ConfigureQueues(const QueueFlagBits &queueFlags)
    {
        SAVANNA_INSERT_SCOPED_PROFILER(GfxDevice::ConfigureQueues());

        if (!m_GraphicsQueue.has_value()
            && queueFlags.HasFlag(SE_VK_QUEUE_GRAPHICS_BIT)
            && m_QueueFamilyIndices.HasGraphicsQueueFamilyIndex())
        {
            VkQueue queue;
            vkGetDeviceQueue(m_LogicalDevice, m_QueueFamilyIndices.m_GraphicsQueueFamilyIndex.value(), 0, &queue);
            m_GraphicsQueue = queue;
        }

        if (!m_PresentQueue.has_value()
            && queueFlags.HasFlag(SE_VK_QUEUE_PRESENT_BIT)
            && m_QueueFamilyIndices.HasPresentQueueFamilyIndex())
        {
            VkQueue queue;
            vkGetDeviceQueue(m_LogicalDevice, m_QueueFamilyIndices.m_PresentQueueFamilyIndex.value(), 0, &queue);
            m_PresentQueue = queue;
        }

        if (!m_TransferQueue.has_value()
            && queueFlags.HasFlag(SE_VK_QUEUE_TRANSFER_BIT)
            && m_QueueFamilyIndices.HasTransferQueueFamilyIndex())
        {
            VkQueue queue;
            vkGetDeviceQueue(m_LogicalDevice, m_QueueFamilyIndices.m_TransferQueueFamilyIndex.value(), 0, &queue);
            m_TransferQueue = queue;
        }

        if (!m_ComputeQueue.has_value()
            && queueFlags.HasFlag(SE_VK_QUEUE_COMPUTE_BIT)
            && m_QueueFamilyIndices.HasComputeQueueFamilyIndex())
        {
            VkQueue queue;
            vkGetDeviceQueue(m_LogicalDevice, m_QueueFamilyIndices.m_ComputeQueueFamilyIndex.value(), 0, &queue);
            m_ComputeQueue = queue;
        }

        if (!m_SparseBindingQueue.has_value()
            && queueFlags.HasFlag(SE_VK_QUEUE_SPARSE_BINDING_BIT)
            && m_QueueFamilyIndices.HasSparseBindingQueueFamilyIndex())
        {
            VkQueue queue;
            vkGetDeviceQueue(m_LogicalDevice, m_QueueFamilyIndices.m_SparseBindingQueueFamilyIndex.value(), 0, &queue);
            m_SparseBindingQueue = queue;
        }
    }

} // namespace Savanna::Gfx::Vk
