#include "Renderer.h"

#include <Gfx/Vk/Public/ISavannaGfxVk2.h>

#include <Types/Containers/Arrays/dynamic_array.h>

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>
#define GLFW_EXPOSE_NATIVE_WIN32
#include <GLFW/glfw3native.h>

#include <Concurrency/JobSystem.h>
#include <IO/FileStream.h>
#include <IO/VirtualFileSystem.h>

namespace vk
{
    Renderer::Renderer() {}

    Renderer::~Renderer()
    {
        SAVANNA_INSERT_SCOPED_PROFILER(Renderer::~Renderer);
    }

    bool Renderer::TryInitialize(const seGfxContextCreateInfo* pCreateInfo, void* pWindowHandle)
    {
        SAVANNA_INSERT_SCOPED_PROFILER(Application::TryInitGfx);
        auto pDefaultAllocInterface = savanna::MemoryManager::GetAllocatorInterfaceForAllocatorKindPtr(kSavannaAllocatorKindGeneral);
        seGfxContextCreateInfo gfxContextCreateInfo;
        if (pCreateInfo != nullptr)
        {
            gfxContextCreateInfo = *pCreateInfo;
        }
        else
        {
            gfxContextCreateInfo = {
                .m_pApplicationName = "SavannaVk2",
                .m_pAllocatorInterface = pDefaultAllocInterface,
                .m_pUserData = nullptr,
            };
        }

        if (SAVANNA_GFX_FAILURE(SavannaGfxInit(&gfxContextCreateInfo)))
        {
            SAVANNA_LOG("Failed to initialize graphics context.");
            return false;
        }

        uint32 instanceExtensionCount = 0;
        const char **glfwExtensions =
            glfwGetRequiredInstanceExtensions(&instanceExtensionCount);

        seVkDriverCreateInfo vkDriverCreateInfo{};
        vkDriverCreateInfo.m_pNext = nullptr;
        vkDriverCreateInfo.m_pUserData = nullptr;
        vkDriverCreateInfo.m_EnableValidationLayers = true;
        vkDriverCreateInfo.m_RequestSurface = true;
        vkDriverCreateInfo.m_pWindowHandle =
        #if SAVANNA_WINDOWS
            glfwGetWin32Window((GLFWwindow*)pWindowHandle);
        #else
            nullptr;
        #endif

        vkDriverCreateInfo.m_InstanceCreateArgs.m_ppEnabledInstanceExtensions =
            glfwExtensions;
        vkDriverCreateInfo.m_InstanceCreateArgs.m_EnabledInstanceExtensionCount =
            instanceExtensionCount;

        vkDriverCreateInfo.m_PhysicalDeviceCreateArgs.m_PreferredGraphicsDeviceIndex =
            -1;

        dynamic_array<const char *> enabledDeviceExtensions{1, kSavannaAllocatorKindTemp};
        if (vkDriverCreateInfo.m_RequestSurface)
        {
            enabledDeviceExtensions.push_back(VK_KHR_SWAPCHAIN_EXTENSION_NAME);
        }

        // Some synchonization extensions that may be useful
        enabledDeviceExtensions.push_back(VK_KHR_SYNCHRONIZATION_2_EXTENSION_NAME);
        enabledDeviceExtensions.push_back(VK_KHR_TIMELINE_SEMAPHORE_EXTENSION_NAME);
        // enabledDeviceExtensions.push_back(VK_NV_RAY_TRACING_EXTENSION_NAME);

        vkDriverCreateInfo.m_LogicalDeviceCreateArgs.m_ppEnabledDeviceExtensions =
            enabledDeviceExtensions.data();
        vkDriverCreateInfo.m_LogicalDeviceCreateArgs.m_EnabledDeviceExtensionCount =
            enabledDeviceExtensions.size();

        seGfxDriverCreateInfo gfxDriverCreateInfo{
            .m_RequestedBackendType = kSavannaGfxApiVulkan,
            .m_pAllocationInterface = pDefaultAllocInterface,
            .m_pNext = &vkDriverCreateInfo,
            .m_pUserData = nullptr,
        };

        seGfxDriverCreateInfoList gfxDriverCreateInfoList{
            .m_pDriverCreateInfos = &gfxDriverCreateInfo,
            .m_CreateInfoCount = 1,
            .m_pUserData = nullptr,
        };

        if (SAVANNA_GFX_FAILURE(SavannaGfxContextCreateDriver(&gfxDriverCreateInfoList)))
        {
            SAVANNA_LOG("Failed to create graphics driver.");
            return false;
        }

        seGfxSwapchainCreateInfo gfxSwapchainCreateInfo{};
        gfxSwapchainCreateInfo.m_ImageCount = 3;
        gfxSwapchainCreateInfo.m_Format = k_SavannaGfxTextureFormat_B8G8R8A8_UNORM;
        glfwGetWindowSize((GLFWwindow*)pWindowHandle,
                            (int *)&gfxSwapchainCreateInfo.m_Width,
                            (int *)&gfxSwapchainCreateInfo.m_Height);

        seGfxHandle swapchainHandle;
        if (SAVANNA_GFX_FAILURE(SavannaGfxCreateSwapchain(&gfxSwapchainCreateInfo, &swapchainHandle)))
        {
            SAVANNA_LOG("Failed to request swapchain.");
            return false;
        }

        return true;
    }
} // namespace vk
