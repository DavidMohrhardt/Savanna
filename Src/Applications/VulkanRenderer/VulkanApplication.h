/**
 * @file VulkanApplication.h
 * @author David Mohrhardt (https://github.com/DavidMohrhardt/Savanna)
 * @brief TODO @DavidMohrhardt Document
 * @version 0.1
 * @date 2022-08-02
 *
 */

#pragma once

#include <SavannaEngine.h>
#include <SavannaVulkan.h>

#include <Application/IApplication.h>
#include <Types/Strings/FixedString.h>

#include <vector>

#include "Window/GLFWWindowWrapper.h"

namespace SavannaVulkan
{
    using namespace Savanna::Rendering::Vulkan;

    class VulkanApplication : public Savanna::IApplication
    {
    public:
        VulkanApplication();
        ~VulkanApplication();

        void Run();

    private:
        GLFWWindowWrapper* m_Window;
        bool m_Initialized = false;

        VulkanInstance* m_Instance;
        VulkanPhysicalDevice* m_PhysicalDevice;

    private:
    };
} // namespace SavannaVulkan
