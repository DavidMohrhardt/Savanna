/**
 * @file VkGraphicsDevice.h
 * @author David Mohrhardt (https://github.com/DavidMohrhardt/Savanna)
 * @brief
 * @version 0.1
 * @date 2022-08-19
 *
 */
#pragma once

#include <SavannaEngine.h>

#include <vulkan/vulkan.h>

#include "VkContext.h"

namespace Savanna::Gfx::Vk
{
    class GfxDevice
    {
    private:
        friend class Renderer;

    private:
        VkPhysicalDevice m_PhysicalDevice;
        VkDevice m_LogicalDevice;

    public:
        GfxDevice();
        GfxDevice(const GfxDevice& other);
        GfxDevice(GfxDevice&& other);

        ~GfxDevice();

    private:
        GfxDevice(const RendererCreateInfo* const pCreateInfo, const Context& context);
        GfxDevice(const RendererCreateInfo* const pCreateInfo, const Context& context, const VkPhysicalDevice& physicalDevice);

    private:
        VkSwapchainKHR CreateSwapchain(
            const Context& context,
            const VkSurfaceKHR& surface,
            const VkSurfaceFormatKHR& surfaceFormat,
            const VkExtent2D& extent,
            const VkPresentModeKHR& presentMode,
            const VkSurfaceCapabilitiesKHR& surfaceCapabilities);

        void CreatePhysicalDevice(const RendererCreateInfo* const pCreateInfo, const Context& context);
        void CreateLogicalDevice(const RendererCreateInfo* const pCreateInfo, const Context& context);

        VkPhysicalDevice GetPhysicalDevice() const { return m_PhysicalDevice; }
        VkDevice GetGfxDevice() const { return m_LogicalDevice; }

    public:
        GfxDevice& operator=(GfxDevice&) { return *this; }
        GfxDevice& operator=(GfxDevice&&) { return *this; }

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
