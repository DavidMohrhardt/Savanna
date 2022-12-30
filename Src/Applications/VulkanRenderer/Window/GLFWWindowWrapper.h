#pragma once

#include "SavannaEngine.h"
#include "SavannaVk.h"
#include "Vulkan/VkRenderer.h"

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>
#define GLFW_EXPOSE_NATIVE_WIN32
#include <GLFW/glfw3native.h>

namespace Savanna::Application
{
    class GLFWWindowWrapper
    {
    public:
        GLFWWindowWrapper();
        GLFWWindowWrapper(int width, int height);
        GLFWWindowWrapper(GLFWwindow* window);

        ~GLFWWindowWrapper();

        void PollEvents();

        bool ShouldClose() const { return m_Window == nullptr || glfwWindowShouldClose(m_Window); }

        GLFWwindow* GetWindowPtr() const { return m_Window; }

    private:
        GLFWwindow* m_Window;
    };
} // namespace SavannaVulkan
