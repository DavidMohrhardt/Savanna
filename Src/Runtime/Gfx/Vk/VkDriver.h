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

#include "Types/Containers/Arrays/DynamicArray.h"

#include "Gfx/GfxDriver.h"

#include "VkGpu.h"

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
        // For access to IAllocator::New<VkDriver> and IAllocator::Delete<VkDriver>
        friend class Savanna::IAllocator;
        VkInstance m_Instance = VK_NULL_HANDLE;
        VkSurfaceKHR m_Surface = VK_NULL_HANDLE;

        VkGpu m_Gpu;

        se_AllocatorInterface_t m_AllocatorInterface;
        VkAllocationCallbacks m_AllocationCallbacks;

        VkDriver(const se_GfxDriverCreateInfo_t& createInfo);
        ~VkDriver();

        VkDriver() = delete;
        VkDriver(const VkDriver&) = delete;
        VkDriver(VkDriver&&) = delete;
        VkDriver& operator=(const VkDriver&) = delete;
        VkDriver& operator=(VkDriver&&) = delete;

        static se_GfxErrorCode_t Initialize(const se_GfxDriverCreateInfo_t& createInfo);
        static se_GfxErrorCode_t Destroy();
        static se_GfxDriverHandle_t GetDriverHandle();

    public:
        static void PopulateDriverInterface(se_GfxDriverInterface_t& outDriverInterface);

        void Teardown();

        VkAllocationCallbacks& GetAllocationCallbacks() { return m_AllocationCallbacks; }
    };
} // namespace Savanna::Gfx
