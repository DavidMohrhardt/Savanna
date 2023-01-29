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

// IO
#include <FileStream.h>

#include "VkRendererCreateInfo.h"

#include "Utilities/VkExtensionUtils.h"
#include "Utilities/VkMacros.h"
#include "Utilities/VkPhysicalDeviceUtils.h"
#include "Utilities/VkRendererCreateUtils.h"

namespace Savanna::Gfx::Vk
{
    /**
     * @brief Default constructor. Sets the physical and logical device to VK_NULL_HANDLE.
     *
     */
    GfxDevice::GfxDevice()
        : m_PhysicalDevice(VK_NULL_HANDLE)
        , m_LogicalDevice(VK_NULL_HANDLE)
    {}

    /**
     * @brief Move constructor.
     *
     * @param other The other GfxDevice object.
     */
    GfxDevice::GfxDevice(GfxDevice&& other)
        : m_PhysicalDevice(other.m_PhysicalDevice)
        , m_LogicalDevice(other.m_LogicalDevice)
    {
        other.m_PhysicalDevice = VK_NULL_HANDLE;
        other.m_LogicalDevice = VK_NULL_HANDLE;
    }

    /**
     * @brief Construct a new GfxDevice object by selecting a physical device and creating
     * a logical device.
     *
     * @param pRendererCreateInfo Universal renderer create info.
     * @param context The Vulkan rendering context.
     * @param pDisplaySurface A pointer to the display surface. If nullptr, no surface has
     * been created and presentation may be unavailable.
     */
    GfxDevice::GfxDevice(
        const RendererCreateInfo *const pRendererCreateInfo,
        const Context &context,
        const DisplaySurface* const pDisplaySurface)
        : GfxDevice()
    {
        CreatePhysicalDevice(pRendererCreateInfo, context, pDisplaySurface, m_QueueFamilyIndices);
        CreateLogicalDevice(pRendererCreateInfo, context, m_QueueFamilyIndices);
        ConfigureQueues(pRendererCreateInfo->m_QueueFlags);
    }

    /**
     * @brief Construct a new GfxDevice object with a preselected physical device.
     *
     * @param pRendererCreateInfo Universal renderer create info.
     * @param context The Vulkan rendering context
     * @param pDisplaySurface A pointer to the display surface. If nullptr, no
     * surface has been created and presentation may be unavailable.
     * @param physicalDevice The preselected physical device.
     */
    GfxDevice::GfxDevice(
        const RendererCreateInfo *const pRendererCreateInfo,
        const Context &context,
        const DisplaySurface* const pDisplaySurface,
        const VkPhysicalDevice &physicalDevice)
        : m_PhysicalDevice(physicalDevice)
        , m_LogicalDevice(VK_NULL_HANDLE)
        , m_QueueFamilyIndices()
    {
        VkSurfaceKHR surface = pDisplaySurface != nullptr ? pDisplaySurface->GetSurface() : VK_NULL_HANDLE;
        m_QueueFamilyIndices.PopulateQueueFamilyIndices(m_PhysicalDevice, &surface);
        CreateLogicalDevice(pRendererCreateInfo, context, m_QueueFamilyIndices);
        ConfigureQueues(pRendererCreateInfo->m_QueueFlags);
    }

    /**
     * @brief Destroy the Gfx Device by destroying the logical device.
     *
     */
    GfxDevice::~GfxDevice()
    {
        if (m_LogicalDevice != VK_NULL_HANDLE)
        {
            vkDestroyDevice(m_LogicalDevice, nullptr);
        }

        m_LogicalDevice = VK_NULL_HANDLE;
        m_PhysicalDevice = VK_NULL_HANDLE;
    }

    /**
     * @brief Creates the physical device and populates the queue family indices.
     *
     * @param pRendererCreateInfo Universal renderer create info.
     * @param context The Vulkan rendering context.
     * @param pDisplaySurface A pointer to the display surface. If null, no surface has been created.
     * @param outQueueFamilyIndices The output queue family indices. Populated by this function.
     */
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

    /**
     * @brief Creates the logical device based on the physical device.
     *
     * @param pRendererCreateInfo Universal renderer create info.
     * @param context The Vulkan rendering context.
     * @param queueFamilyIndices The queue family indices for the physical device.
     */
    void GfxDevice::CreateLogicalDevice(
        const RendererCreateInfo* const pRendererCreateInfo,
        const Context& context,
        const QueueFamilyIndices& queueFamilyIndices)
    {
        SAVANNA_INSERT_SCOPED_PROFILER(GfxDevice::CreateLogicalDevice());

        uint32 numUniqueQueueFamilies = 0;
        Utils::GetUniqueQueueFamilies(nullptr, numUniqueQueueFamilies, queueFamilyIndices);

        float queuePriority = 1.0f;
        std::vector<VkDeviceQueueCreateInfo> queueCreateInfos(numUniqueQueueFamilies);
        Utils::GetUniqueQueueFamilies(queueCreateInfos.data(), numUniqueQueueFamilies, queueFamilyIndices, &queuePriority);

        VkPhysicalDeviceFeatures deviceFeatures = {0};
        VkDeviceCreateInfo createInfo = {};
        Utils::PopulateVkDeviceCreateInfo(*pRendererCreateInfo, queueCreateInfos.data(), numUniqueQueueFamilies, createInfo);

#if SAVANNA_VULKAN_DEBUGGING
        Utils::ValidateDeviceExtensions(m_PhysicalDevice, createInfo.ppEnabledExtensionNames, createInfo.enabledExtensionCount);
#endif

        createInfo.flags = 0;
        createInfo.pNext = nullptr;
        // createInfo.pEnabledFeatures = &deviceFeatures;

        if (vkCreateDevice(m_PhysicalDevice, &createInfo, nullptr, &m_LogicalDevice) != VK_SUCCESS)
        {
            throw std::runtime_error("failed to create logical device!");
        }
    }

    /**
     * @brief Configures VkQueues based on the queue flags.
     *
     * @param queueFlags The flags indicating which queues to configure.
     */
    void GfxDevice::ConfigureQueues(const QueueFlagBits &queueFlags)
    {
        SAVANNA_INSERT_SCOPED_PROFILER(GfxDevice::ConfigureQueues());

        if (!queueFlags.Any())
        {
            return;
        }

        if (!m_GraphicsQueue.has_value()
            && queueFlags.HasFlag(seVkQueueFlagBitsGraphicsBit)
            && m_QueueFamilyIndices.HasGraphicsQueueFamilyIndex())
        {
            VkQueue queue;
            vkGetDeviceQueue(m_LogicalDevice, m_QueueFamilyIndices.m_GraphicsQueueFamilyIndex.value(), 0, &queue);
            m_GraphicsQueue = queue;
        }

        if (!m_PresentQueue.has_value()
            && queueFlags.HasFlag(seVkQueueFlagBitsPresentBit)
            && m_QueueFamilyIndices.HasPresentQueueFamilyIndex())
        {
            VkQueue queue;
            vkGetDeviceQueue(m_LogicalDevice, m_QueueFamilyIndices.m_PresentQueueFamilyIndex.value(), 0, &queue);
            m_PresentQueue = queue;
        }

        if (!m_TransferQueue.has_value()
            && queueFlags.HasFlag(seVkQueueFlagBitsTransferBit)
            && m_QueueFamilyIndices.HasTransferQueueFamilyIndex())
        {
            VkQueue queue;
            vkGetDeviceQueue(m_LogicalDevice, m_QueueFamilyIndices.m_TransferQueueFamilyIndex.value(), 0, &queue);
            m_TransferQueue = queue;
        }

        if (!m_ComputeQueue.has_value()
            && queueFlags.HasFlag(seVkQueueFlagBitsComputeBit)
            && m_QueueFamilyIndices.HasComputeQueueFamilyIndex())
        {
            VkQueue queue;
            vkGetDeviceQueue(m_LogicalDevice, m_QueueFamilyIndices.m_ComputeQueueFamilyIndex.value(), 0, &queue);
            m_ComputeQueue = queue;
        }

        if (!m_SparseBindingQueue.has_value()
            && queueFlags.HasFlag(seVkQueueFlagBitsSparseBindingBit)
            && m_QueueFamilyIndices.HasSparseBindingQueueFamilyIndex())
        {
            VkQueue queue;
            vkGetDeviceQueue(m_LogicalDevice, m_QueueFamilyIndices.m_SparseBindingQueueFamilyIndex.value(), 0, &queue);
            m_SparseBindingQueue = queue;
        }
    }

    bool GfxDevice::TryLoadShaderFromDisk(const char *const shaderPath)
    {
        IO::FileStream shaderStream = IO::FileStream(shaderPath);
        IO::FileStream shaderMetadataStream = IO::FileStream(shaderPath);
        if (shaderStream.IsOpen())
        {
            auto shaderCode = shaderStream.ReadFile();
        }
        return false;
    }

    GfxDevice &GfxDevice::operator=(GfxDevice &&other)
    {
        if (this != &other)
        {
            m_PhysicalDevice = other.m_PhysicalDevice;
            m_LogicalDevice = other.m_LogicalDevice;
            m_GraphicsQueue = other.m_GraphicsQueue;
            m_PresentQueue = other.m_PresentQueue;
            m_TransferQueue = other.m_TransferQueue;
            m_ComputeQueue = other.m_ComputeQueue;
            m_SparseBindingQueue = other.m_SparseBindingQueue;
            m_QueueFamilyIndices = other.m_QueueFamilyIndices;

            other.m_PhysicalDevice = VK_NULL_HANDLE;
            other.m_LogicalDevice = VK_NULL_HANDLE;
            other.m_GraphicsQueue = std::nullopt;
            other.m_PresentQueue = std::nullopt;
            other.m_TransferQueue = std::nullopt;
            other.m_ComputeQueue = std::nullopt;
            other.m_SparseBindingQueue = std::nullopt;
            other.m_QueueFamilyIndices = QueueFamilyIndices();
        }

        return *this;
    }

} // namespace Savanna::Gfx::Vk
