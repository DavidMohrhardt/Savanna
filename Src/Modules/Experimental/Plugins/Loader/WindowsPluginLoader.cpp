#include "Loader/PluginLoader.h"

#if SAVANNA_WINDOWS

// TODO: For some reason the compiler just will not load the Kernel32 library. I have no idea why.
#pragma lib("kernel32.dll")

// #define WINDOWS_LEAN_AND_MEAN 1
#include <libloaderapi.h>

namespace Savanna::Plugin
{
    bool PluginLoader::LoadPlugin(const char* const pPluginName, OS::LibraryHandle& outHandle)
    {
        outHandle = OS::k_InvalidLibraryHandle;

        // Load the plugin
        outHandle = LoadLibrary(pPluginName);
        if (outHandle == OS::k_InvalidLibraryHandle)
        {
            // Console::LogError("Failed to load plugin: %s", pPluginName);
            return false;
        }

        return true;
    }

    bool PluginLoader::UnloadPlugin(const OS::LibraryHandle& handle)
    {
        return handle != OS::k_InvalidLibraryHandle
            && FreeLibrary(handle);
    }
} // namespace Savanna::Plugin

#endif // PLATFORM_WINDOWS
