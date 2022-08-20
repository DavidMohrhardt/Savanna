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
#include <Types/Strings/HashString.h>

#include <vector>

#include "Window/GLFWWindowWrapper.h"

namespace SavannaVulkan
{
    using namespace Savanna;
    using namespace Savanna::Rendering::Vulkan;

    class VulkanApplication : public Savanna::IApplication
    {
        static constexpr HashString k_MemoryArenaTags[] = {
            HashString("Common"),
            HashString("System"),
            HashString("Graphics")
        };
    public:
        VulkanApplication();
        ~VulkanApplication();

        void Run();

    private:

        GLFWWindowWrapper m_Window;
        VulkanInstance m_Instance;
        VulkanPhysicalDevice m_CurrentPhysicalDevice;

        std::vector<VulkanPhysicalDevice> m_AvailablePhysicalDevices;

    private:
    };
} // namespace SavannaVulkan
