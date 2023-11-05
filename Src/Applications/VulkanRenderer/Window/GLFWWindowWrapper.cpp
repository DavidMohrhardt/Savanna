#include "GLFWWindowWrapper.h"
#include <Profiling/Profiler.h>

void InitGlfwWindow(GLFWwindow*& outWindow, int width = 1920, int height = 1080)
{
    SAVANNA_INSERT_SCOPED_PROFILER("InitGlfwWindow");
    glfwInit();

    glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
    glfwWindowHint(GLFW_RESIZABLE, GLFW_TRUE);
    outWindow = glfwCreateWindow(width, height, "Savanna Vulkan 2", nullptr, nullptr);
}

GLFWWindowWrapper::GLFWWindowWrapper()
{
    // Create Window
    InitGlfwWindow(m_Window);
}

GLFWWindowWrapper::GLFWWindowWrapper(int width, int height)
{
    // Create Window
    InitGlfwWindow(m_Window, width, height);
}

GLFWWindowWrapper::GLFWWindowWrapper(GLFWwindow* window)
{
    if (window == nullptr)
    {
        InitGlfwWindow(m_Window);
    }
    else
    {
        m_Window = window;
    }
}

GLFWWindowWrapper::~GLFWWindowWrapper()
{
    if (m_Window != nullptr)
    {
        glfwDestroyWindow(m_Window);
        glfwTerminate();
    }
}

void GLFWWindowWrapper::PollEvents()
{
    // SAVANNA_INSERT_SCOPED_PROFILER("GLFWWindowWrapper::PollEvents");
    glfwPollEvents();
}
