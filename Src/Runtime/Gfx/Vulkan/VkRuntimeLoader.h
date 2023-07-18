#pragma once

#if SAVANNA_VK_DYNAMIC_INCLUDE
#include "SavannaVk.h"

#include "OS/DynamicLibraryManager.h"

namespace Savanna::Gfx::Vk
{
    static bool IsVulkanAvailable()
    {
        static auto isAvailable = []() -> bool
        {
            using namespace OS;
            return  DynamicLibraryManager::TryLoadLibrary("vulkan-1.dll") &&
                    DynamicLibraryManager::TryGetProcAddressForFunc("vulkan-1.dll", "vkGetInstanceProcAddr", s_GetInstanceProcAddr);
        }();
        return isAvailable;
    }

    static void LoadVulkanApi()
} // namespace Savanna::Gfx::Vk
#endif // SAVANNA_VK_DYNAMIC_INCLUDE
