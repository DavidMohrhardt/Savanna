/**
 * @file VkGraphicsDevice.h
 * @author David Mohrhardt (https://github.com/DavidMohrhardt/Savanna)
 * @brief
 * @version 0.1
 * @date 2022-08-19
 *
 */
#pragma once

#include "SavannaVk.h"

#include "VkContext.h"
#include "VkDisplaySurface.h"
#include "VkQueueFamilyIndices.h"
#include "VkQueueFlags.h"

#include <optional>

namespace Savanna::Gfx::Vk
{
    class GfxDevice
    {
        friend class Renderer;

    private:
        VkPhysicalDevice m_PhysicalDevice;
        VkDevice m_LogicalDevice;

        std::optional<VkQueue> m_GraphicsQueue;
        std::optional<VkQueue> m_PresentQueue;
        std::optional<VkQueue> m_TransferQueue;
        std::optional<VkQueue> m_ComputeQueue;
        std::optional<VkQueue> m_SparseBindingQueue;

        QueueFamilyIndices m_QueueFamilyIndices;

    public:
        GfxDevice();
        GfxDevice(const GfxDevice& other) = delete;
        GfxDevice(GfxDevice&& other);

        ~GfxDevice();

    private:
        GfxDevice(
            const RendererCreateInfo* const pCreateInfo,
            const Context& context,
            const DisplaySurface* const pDisplaySurface = nullptr);

        GfxDevice(
            const RendererCreateInfo* const pCreateInfo,
            const Context& context,
            const DisplaySurface* const pDisplaySurface,
            const VkPhysicalDevice& physicalDevice);

    private:
        void CreatePhysicalDevice(const RendererCreateInfo* const pCreateInfo,
            const Context &context,
            const DisplaySurface* const pDisplaySurface,
            QueueFamilyIndices& outQueueFamilyIndices);

        void CreateLogicalDevice(const RendererCreateInfo* const pCreateInfo,
            const Context &context,
            const QueueFamilyIndices& queueFamilyIndices);

        void ConfigureQueues(const QueueFlagBits& queueFlags);

    public:
        VkPhysicalDevice GetPhysicalDevice() const { return m_PhysicalDevice; }
        VkDevice GetGfxDevice() const { return m_LogicalDevice; }

        VkQueue GetGraphicsQueue() const { return m_GraphicsQueue.value_or(VK_NULL_HANDLE); }
        VkQueue GetPresentQueue() const { return m_PresentQueue.value_or(VK_NULL_HANDLE); }
        VkQueue GetTransferQueue() const { return m_TransferQueue.value_or(VK_NULL_HANDLE); }
        VkQueue GetComputeQueue() const { return m_ComputeQueue.value_or(VK_NULL_HANDLE); }
        VkQueue GetSparseBindingQueue() const { return m_SparseBindingQueue.value_or(VK_NULL_HANDLE); }

        QueueFamilyIndices GetQueueFamilyIndices() const { return m_QueueFamilyIndices; }

    public:
        GfxDevice& operator=(const GfxDevice& other) = delete;
        GfxDevice& operator=(GfxDevice&& other);

        operator VkPhysicalDevice() const { return m_PhysicalDevice; }
        operator VkPhysicalDevice*() { return &m_PhysicalDevice; }
        operator const VkPhysicalDevice*() const { return &m_PhysicalDevice; }
        operator VkPhysicalDevice&() { return m_PhysicalDevice; }
        operator const VkPhysicalDevice&() const { return m_PhysicalDevice; }

        operator VkDevice() const { return m_LogicalDevice; }
        operator VkDevice*() { return &m_LogicalDevice; }
        operator const VkDevice*() const { return &m_LogicalDevice; }
        operator VkDevice&() { return m_LogicalDevice; }
        operator const VkDevice&() const { return m_LogicalDevice; }
    };
} // namespace Savanna::Gfx::Vk
