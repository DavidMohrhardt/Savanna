#pragma once

#include <SavannaEngine.h>

#if SAVANNA_WINDOWS
#ifndef UNICODE
#   define UNICODE
#endif
#define NOMINMAX
#include <windows.h>
#   define PLATFORM_HANDLE HMODULE
#   define PLATFORM_STRING LPCWSTR
#   define PLATFORM_STRINGIFY(x) L#x".dll"
#else
#   define PLATFORM_HANDLE void*
#   define PLATFORM_STRING const char* const
#   define PLATFORM_STRINGIFY(x) #x
#endif

namespace savanna::OS
{
    using PlatformString = PLATFORM_STRING;
    using LibraryHandle = PLATFORM_HANDLE;

    constexpr LibraryHandle kInvalidLibraryHandle {};

    inline void UnloadLibrary(PLATFORM_HANDLE handle)
    {
#if SAVANNA_WINDOWS
        ::FreeLibrary(handle);
#else
#       error NOT IMPLEMENTED
#endif
    }

    inline LibraryHandle LoadPlatformLibrary(PLATFORM_STRING libName)
    {
#if SAVANNA_WINDOWS
        return LoadLibrary(libName);
#else
#       error NOT IMPLEMENTED
#endif
    }

    template<typename T>
    inline T LoadSymbol(LibraryHandle handle, const char* symbolName)
    {
#if SAVANNA_WINDOWS
        return (T)GetProcAddress(handle, symbolName);
#else
#       error NOT IMPLEMENTED
#endif
    }
}
