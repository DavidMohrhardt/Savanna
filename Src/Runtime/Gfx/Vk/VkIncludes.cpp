#include "VkIncludes.h"

#if SAVANNA_WINDOWS
#   define VK_LIBRARY_NAME L"vulkan-1.dll"
#else
#   if SAVANNA_LINUX
#       define VK_LIBRARY_NAME "libvulkan.so.1";
#   else
#       error "Unsupported"
#   endif
#endif

namespace savanna::gfx::vk
{
    using namespace OS;

    static LibraryHandle g_LibraryHandle {};

    LibraryHandle GetLibraryHandle() { return g_LibraryHandle; }

    bool TryLoadVulkanLibrary()
    {
        if (g_LibraryHandle == kInvalidLibraryHandle)
        {
            g_LibraryHandle = LoadPlatformLibrary(VK_LIBRARY_NAME);
        }

        return g_LibraryHandle != kInvalidLibraryHandle;
    }

    void UnloadVulkanLibrary()
    {
        if (g_LibraryHandle != kInvalidLibraryHandle)
        {
            UnloadLibrary(g_LibraryHandle);
            g_LibraryHandle = {};
        }
    }

#ifdef VK_NO_PROTOTYPES
#define LOAD_SVK_FUNC(x) \
    svk::x = OS::LoadSymbol<PFN_##x>(GetLibraryHandle(), #x);

    void LoadLibraryPhase0()
    {
        SVK_PHASE_0(LOAD_SVK_FUNC);
    }

    void LoadLibraryPhase1()
    {
        SVK_PHASE_1(LOAD_SVK_FUNC);
    }

    void LoadLibraryPhase2()
    {
        SVK_PHASE_2(LOAD_SVK_FUNC);
    }

    void LoadLibraryPhase3()
    {
        SVK_PHASE_3(LOAD_SVK_FUNC);
    }
#endif
}

namespace svk
{
#define DEFINE_SVK_FUNCS(x) PFN_##x x = nullptr;
    SVK_ALL_PHASES(DEFINE_SVK_FUNCS);
} // namespace svk
