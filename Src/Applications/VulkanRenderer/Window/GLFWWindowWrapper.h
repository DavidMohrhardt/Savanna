#pragma once

#include "SavannaEngine.h"
#include <GLFW/glfw3.h>

namespace SavannaVulkan
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

    private:
        GLFWwindow* m_Window;
    };
} // namespace SavannaVulkan
