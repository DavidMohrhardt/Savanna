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

#define NULLIFY_FUNCPTR(x) svk::x = nullptr;

    bool TryLoadVulkanLibrary()
    {
        if (g_LibraryHandle == kInvalidLibraryHandle)
        {
            SVK_ALL_PHASES(NULLIFY_FUNCPTR);
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
            SVK_ALL_PHASES(NULLIFY_FUNCPTR);
        }
    }

#undef NULLIFY_FUNCPTR

#ifdef VK_NO_PROTOTYPES

    void LoadLibraryPhase0()
    {
#define LOAD_SVK_FUNC(x) \
    svk::x = OS::LoadSymbol<PFN_##x>(GetLibraryHandle(), #x);
        SVK_PHASE_0(LOAD_SVK_FUNC);
#undef LOAD_SVK_FUNC
    }

    template <typename TFunc>
    TFunc LoadInstanceFunction(VkInstance instance, const char* pName)
    {
        TFunc pFuncPtr = nullptr;
        if (!!svk::vkGetInstanceProcAddr) SAVANNA_BRANCH_UNLIKELY
        {
            pFuncPtr = OS::LoadSymbol<TFunc>(GetLibraryHandle(), pName);
        }
        else SAVANNA_BRANCH_LIKELY
        {
            pFuncPtr = (TFunc)svk::vkGetInstanceProcAddr(instance, pName);
        }

        if (!pFuncPtr)
        {
            SAVANNA_WARNING_LOG("[SEVK]: Could not load instance function {}", pName);
        }
        return pFuncPtr;
    }

    void LoadLibraryPhase1(VkInstance instance)
    {
#define LOAD_SVK_FUNC(x) \
    svk::x = LoadInstanceFunction<PFN_##x>(instance, #x);
        SVK_PHASE_1(LOAD_SVK_FUNC);
    }

    void LoadLibraryPhase2(VkInstance instance)
    {
        SVK_PHASE_2(LOAD_SVK_FUNC);
#undef LOAD_SVK_FUNC
    }

    template <typename TFunc>
    TFunc LoadDeviceFunction(VkDevice device, const char* pName)
    {
        TFunc pFuncPtr = nullptr;
        if (!!svk::vkGetDeviceProcAddr) SAVANNA_BRANCH_UNLIKELY
        {
            pFuncPtr = OS::LoadSymbol<TFunc>(GetLibraryHandle(), pName);
        }
        else SAVANNA_BRANCH_LIKELY
        {
            pFuncPtr = (TFunc)svk::vkGetDeviceProcAddr(device, pName);
        }

        if (!pFuncPtr)
        {
            SAVANNA_WARNING_LOG("[SEVK]: Could not load device function {}", pName);
        }
        return pFuncPtr;
    }

    void LoadLibraryPhase3(VkDevice device)
    {
#define LOAD_SVK_FUNC(x) \
    svk::x = LoadDeviceFunction<PFN_##x>(device, #x);
        SVK_PHASE_3(LOAD_SVK_FUNC);
#undef LOAD_SVK_FUNC
    }
#endif
}

namespace svk
{
#define DEFINE_SVK_FUNCS(x) PFN_##x x = nullptr;
    SVK_ALL_PHASES(DEFINE_SVK_FUNCS);
} // namespace svk
