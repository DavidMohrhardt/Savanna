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

using namespace savanna::Gfx::Vk;

class VulkanApplication
{
public:
    VulkanApplication(int argc, char** argvs);
    ~VulkanApplication();

    void Run();

private:
    void CreateRenderer();

    GLFWWindowWrapper m_Window;
    Renderer m_Renderer;
};
