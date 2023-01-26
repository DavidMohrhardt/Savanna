#pragma once

// Savanna Interface API
#if defined(__CYGWIN__) || defined(Win32) || defined(_WIN32) || defined(__WIN32__) || defined(__NT__)
    #define SAVANNA_INTERFACE_API __stdcall
    #define SAVANNA_INTERFACE_EXPORT __declspec(dllexport)
    #define SAVANNA_EXPORT(__type) extern "C" __type SAVANNA_INTERFACE_EXPORT SAVANNA_INTERFACE_API

    #define SAVANNA_INTERFACE_IMPORT __declspec(dllimport)
    #define SAVANNA_IMPORT(__type) extern "C" __type SAVANNA_INTERFACE_IMPORT SAVANNA_INTERFACE_API
#else
    #define SAVANNA_INTERFACE_API
    #define SAVANNA_INTERFACE_EXPORT
    #define SAVANNA_EXPORT(__type) extern "C" SAVANNA_INTERFACE_EXPORT __type SAVANNA_INTERFACE_API

    #define SAVANNA_INTERFACE_IMPORT
    #define SAVANNA_IMPORT(__type) extern "C" SAVANNA_INTERFACE_IMPORT __type SAVANNA_INTERFACE_API
#endif
