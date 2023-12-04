#include "VkLibrary.h"

#include <SavannaEngine.h>
#include "OS/LibraryLoader.h"

#if SAVANNA_WINDOWS
#   define VK_LIBRARY_NAME L"vulkan-1.dll"
#else
#   if SAVANNA_LINUX
#       define VK_LIBRARY_NAME "libvulkan.so.1";
#   else
#       error "Unsupported"
#   endif
#endif

namespace Savanna::Gfx::Vk2
{
    OS::LibraryHandle s_LibraryHandle {};
#if VK_NO_PROTOTYPES
    VkFunctionTable s_FunctionTable {};
#endif

namespace Utils
{
    bool TryLoadVulkanLibrary()
    {
        if (s_LibraryHandle == OS::kInvalidLibraryHandle)
        {
            s_LibraryHandle = OS::LoadLibrary(VK_LIBRARY_NAME);
        }

        return s_LibraryHandle != OS::kInvalidLibraryHandle;
    }

    void UnloadVulkanLibrary() {
        if (s_LibraryHandle != OS::kInvalidLibraryHandle)
        {
            OS::UnloadLibrary(s_LibraryHandle);
            s_LibraryHandle = {};
        }
    }
} // namespace Utils
} // namespace Savanna::Gfx::Vk2

