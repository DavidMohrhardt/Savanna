#ifndef VK_INCLUDES_H
#define VK_INCLUDES_H

#define VK_NO_PROTOTYPES
#include <vulkan/vulkan.h>

#include <SavannaEngine.h>
#include "OS/LibraryLoader.h"

#define SVK_PHASE_0(MACRO) \
    MACRO(vkCreateInstance) \
    MACRO(vkGetInstanceProcAddr) \
    MACRO(vkEnumerateInstanceExtensionProperties)

#define SVK_PHASE_1_MAIN(MACRO) \
    MACRO(vkDestroyInstance) \
    MACRO(vkGetDeviceProcAddr) \
    MACRO(vkEnumeratePhysicalDevices) \
    MACRO(vkGetPhysicalDeviceProperties) \
    MACRO(vkGetPhysicalDeviceFeatures) \
    MACRO(vkGetPhysicalDeviceMemoryProperties) \
    MACRO(vkGetPhysicalDeviceFormatProperties) \
    MACRO(vkEnumerateDeviceExtensionProperties) \
    MACRO(vkGetPhysicalDeviceQueueFamilyProperties)

#define SVK_PHASE_1_EXT(MACRO) \
    MACRO(vkGetPhysicalDeviceSurfaceCapabilitiesKHR) \
    MACRO(vkGetPhysicalDeviceSurfaceFormatsKHR) \
    MACRO(vkGetPhysicalDeviceSurfacePresentModesKHR) \
    MACRO(vkGetPhysicalDeviceSurfaceSupportKHR) \
    MACRO(vkDestroySurfaceKHR) \

#define SVK_PHASE_1(MACRO) \
    SVK_PHASE_1_MAIN(MACRO) \
    SVK_PHASE_1_EXT(MACRO)

#define SVK_PHASE_2_MAIN(MACRO) \
    MACRO(vkCreateDevice)

#define SVK_PHASE_2_EXT(MACRO)

#define SVK_PHASE_2(MACRO) \
    SVK_PHASE_2_MAIN(MACRO) \
    SVK_PHASE_2_EXT(MACRO)

#define SVK_PHASE_3_MAIN(MACRO) \
    MACRO(vkDestroyDevice) \
    MACRO(vkCreateImageView) \
    MACRO(vkDestroyImageView) \
    MACRO(vkCreateShaderModule) \
    MACRO(vkDestroyShaderModule) \
    MACRO(vkCreateSemaphore) \
    MACRO(vkDestroySemaphore)

#define SVK_PHASE_3_EXT(MACRO) \
    MACRO(vkCreateSwapchainKHR) \
    MACRO(vkDestroySwapchainKHR) \
    MACRO(vkGetSwapchainImagesKHR)

#define SVK_PHASE_3(MACRO) \
    SVK_PHASE_3_MAIN(MACRO) \
    SVK_PHASE_3_EXT(MACRO)

#define SVK_ALL_PHASES(MACRO) \
    SVK_PHASE_0(MACRO) \
    SVK_PHASE_1(MACRO) \
    SVK_PHASE_2(MACRO) \
    SVK_PHASE_3(MACRO) \

namespace savanna::gfx::vk
{
#ifdef VK_NO_PROTOTYPES
    OS::LibraryHandle GetLibraryHandle();
    bool TryLoadVulkanLibrary();
    void UnloadVulkanLibrary();
#else
    inline bool TryLoadVulkanLibrary() { return true; }
    inline void UnloadVulkanLibrary() {  }
    inline OS::LibraryHandle GetLibraryHandle() { return OS::kInvalidLibraryHandle; }
#endif
}

#ifdef VK_NO_PROTOTYPES
#   define DECL_SVK_FUNC(x) extern PFN_##x x;
#else
#   define DECL_SVK_FUNC(x)
#endif // VK_NO_PROTOTYPES

namespace svk
{
    SVK_ALL_PHASES(DECL_SVK_FUNC);
} // namespace svk

namespace savanna::gfx::vk
{
#ifdef VK_NO_PROTOTYPES
    void LoadLibraryPhase0();
    void LoadLibraryPhase1(VkInstance instance);
    void LoadLibraryPhase2(VkInstance instance);
    void LoadLibraryPhase3(VkDevice Device);
#else
    inline void LoadLibraryPhase0() {}
    inline void LoadLibraryPhase1(VkInstance instance) {}
    inline void LoadLibraryPhase2(VkInstance instance) {}
    inline void LoadLibraryPhase3(VkDevice Device) {}
#endif
}

#ifdef VK_NO_PROTOTYPES
using namespace svk;
#endif // VK_NO_PROTOTYPES

#endif // VK_INCLUDES_H
