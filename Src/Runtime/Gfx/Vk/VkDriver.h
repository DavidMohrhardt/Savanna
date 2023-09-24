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

#include "Types/Containers/Arrays/DynamicArray.h"

#include "Gfx/GfxDriver.h"

#include "VkAllocationCallback.h"

#include <vulkan/vulkan.h>

namespace Savanna::Gfx::Vk2
{
    /**
     * @brief Provides the acquisition interface for the Vulkan api.
     * Intended to handle the minutia of creating and binding resources
     * for the higher level GfxDriver.
     */
    class VkDriver final : public GfxDriverBase<kSavannaGfxApiVulkan>
    {
    private:
        VkInstance m_Instance;
        VkAllocationCallbacks m_AllocationCallbacks;
        VkPhysicalDevice m_PhysicalDevice;
        VkDevice m_LogicalDevice;

    public:
        VkDriver() = default;
        ~VkDriver() = default;

        void Create(const se_GfxDriverCreateInfo_t& createInfo) override;
        void Destroy() override;

        // GfxRenderSurface* CreateRenderSurface(const GfxRenderSurfaceCreateInfo& createInfo) override;
    };
} // namespace Savanna::Gfx
