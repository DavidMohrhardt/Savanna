#pragma once

#include "SavannaEngine.h"

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>
#define GLFW_EXPOSE_NATIVE_WIN32
#include <GLFW/glfw3native.h>

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
