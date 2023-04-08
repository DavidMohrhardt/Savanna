/**
 * @file Renderer.h
 * @author David Mohrhardt (https://github.com/DavidMohrhardt/Savanna)
 * @brief
 * @version 0.1
 * @date 2022-08-20
 */
#pragma once

// Savanna Core Includes
#include <Utilities/SavannaCoding.h>

// Vulkan Includes
#include <vulkan/vulkan.h>

// Savanna Vulkan includes
#include <VkContext.h>
#include <VkDisplaySurface.h>
#include <VkGfxDevice.h>
#include <VkQueueFamilyIndices.h>
#include <VkRendererCreateInfo.h>
#include <VkSwapchain.h>

namespace Savanna::Gfx::Vk
{
    class Renderer
    {
    private:
        Context m_Context;
        DisplaySurface m_DisplaySurface;
        GfxDevice m_GfxDevice;
        Swapchain m_Swapchain;

    public:
        Renderer() = default;
        Renderer(const RendererCreateInfo* const pCreateInfo);

        ~Renderer();

        Renderer(const Renderer& other) = delete;
        Renderer(Renderer&& other);

    public:
        // operators
        Renderer& operator=(const Renderer& other) = delete;
        Renderer& operator=(Renderer&& other);

    public:
        void Create(const RendererCreateInfo* const pCreateInfo);
        void Destroy();

        const Context& GetContext() { return m_Context; }
        const GfxDevice& GetGfxDevice() { return m_GfxDevice; }
    };
} // namespace Savanna::Gfx::Vk
