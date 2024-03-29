/**
 * @file VkDriver.h
 * @author David Mohrhardt (https://github.com/DavidMohrhardt/Savanna)
 * @brief
 * @version 0.1
 * @date 2023-09-16
 *
 * @copyright Copyright (c) 2023
 *
 */
#pragma once

#include <SavannaEngine.h>
#include <Types/Singleton/Singleton.h>

#include "Types/Containers/Arrays/dynamic_array.h"

#include "Gfx/GfxDriver.h"

#include "VkGpu.h"
#include "VkShaderModuleCache.h"
#include "VkSwapchain.h"

#include <vulkan/vulkan.h>

namespace Savanna::Gfx::Vk2
{
    /**
     * @brief Provides the acquisition interface for the Vulkan api.
     * Intended to handle the minutia of creating and binding resources
     * for the higher level GfxDriver.
     */
    class VkDriver final
    {
    private:
        // For access to Allocator::New<VkDriver> and Allocator::Delete<VkDriver>
        friend class Savanna::Allocator;
        friend class VkAllocator;

        static se_AllocatorInterface_t s_AllocatorInterface;

        VkInstance m_Instance = VK_NULL_HANDLE;
        VkSurfaceKHR m_Surface = VK_NULL_HANDLE;

        // Helper classes
        VkSwapchain m_Swapchain;
        VkGpu m_Gpu;
        VkShaderModuleCache m_ShaderModuleCache;

        VkDriver(const se_GfxDriverCreateInfo_t& createInfo);
        ~VkDriver();

        VkDriver() = delete;
        VkDriver(const VkDriver&) = delete;
        VkDriver(VkDriver&&) = delete;
        VkDriver& operator=(const VkDriver&) = delete;
        VkDriver& operator=(VkDriver&&) = delete;

        // Declares the interface functions for the GfxDriver
        DECL_GFX_DRIVER_INTERFACE_FUNCS();

    public:
        static void PopulateDriverInterface(se_GfxDriverInterface_t& outDriverInterface);

        void Teardown();
    };
} // namespace Savanna::Gfx
