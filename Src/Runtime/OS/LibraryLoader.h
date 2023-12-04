#pragma once

#if SAVANNA_WINDOWS
#include <windows.h>
#   define PLATFORM_HANDLE HMODULE
#   define PLATFORM_STRING LPCWSTR
#else
#   define PLATFORM_HANDLE void*
#   define PLATFORM_STRING const char* const
#endif

namespace Savanna::OS
{
    using PlatformString = PLATFORM_STRING;
    using LibraryHandle = PLATFORM_HANDLE;

    constexpr LibraryHandle kInvalidLibraryHandle {};

    inline void UnloadLibrary(LibraryHandle handle)
    {
#if SAVANNA_WINDOWS
        ::FreeLibrary(handle);
#else
#       error NOT IMPLEMENTED
#endif
    }

    inline LibraryHandle LoadLibrary(PlatformString libName)
    {
#if SAVANNA_WINDOWS
        return LoadLibrary(libName);
#else
#       error NOT IMPLEMENTED
#endif
    }
}
