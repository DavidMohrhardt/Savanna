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
#include <SavannaVk.h>

#include <VkRenderer.h>

#include <Application/IApplication.h>
#include <Types/Strings/HashString.h>

#include <vector>

#include "Window/GLFWWindowWrapper.h"

namespace Savanna::Application
{
    using namespace Savanna::Gfx::Vk;

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
        Renderer m_Renderer;

    private:
    };
} // namespace SavannaVk
