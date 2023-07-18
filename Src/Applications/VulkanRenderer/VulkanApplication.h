/**
 * @file VulkanApplication.h
 * @author David Mohrhardt (https://github.com/DavidMohrhardt/Savanna)
 * @brief TODO @DavidMohrhardt Document
 * @version 0.1
 * @date 2022-08-02
 *
 */

#pragma once

#include "SavannaEngine.h"
#include <SavannaVk.h>

#include <VkRenderer.h>

#include <Types/Strings/FixedString.h>

#include "Window/GLFWWindowWrapper.h"

#include <vector>

namespace Savanna::Application
{
    using namespace Savanna::Gfx::Vk;

    class VulkanApplication
    {
    public:
        VulkanApplication();
        ~VulkanApplication();

        void Run();

    private:
        void CreateRenderer();
        void CreateShaderModules();

        GLFWWindowWrapper m_Window;
        Renderer m_Renderer;

    };
} // namespace SavannaVk
