/**
 * @file PlatformDefinitions.h
 * @author David Mohrhardt (https://github.com/DavidMohrhardt/Savanna)
 * @brief
 * @version 0.1
 * @date 2023-09-30
 *
 * @copyright Copyright (c) 2023
 *
 */
#pragma once

// Linux and Linux-derived           __linux__
// Android                           __ANDROID__ (implies __linux__)
// Linux (non-Android)               __linux__ && !__ANDROID__
// Darwin (Mac OS X and iOS)         __APPLE__
// Akaros (http://akaros.org)        __ros__
// Windows                           _WIN32
// Windows 64 bit                    _WIN64 (implies _WIN32)
// NaCL                              __native_client__
// AsmJS                             __asmjs__
// Fuschia                           __Fuchsia__

#if _WIN32
    #define SAVANNA_WINDOWS 1
    #define SAVANNA_LINUX 0
    #define SAVANNA_ANDROID 0
    #define SAVANNA_DARWIN 0
#elif __APPLE__
    #define SAVANNA_DARWIN 1
    #define SAVANNA_LINUX 0
    #define SAVANNA_ANDROID 0
    #define SAVANNA_WINDOWS 0
#elif __linux__
    #if __ANDROID__
        #define SAVANNA_ANDROID 1
        #define SAVANNA_LINUX 0
        #define SAVANNA_DARWIN 0
        #define SAVANNA_WINDOWS 0
        #error "Android is not supported."
    #else
        #define SAVANNA_LINUX 1
        #define SAVANNA_ANDROID 0
        #define SAVANNA_DARWIN 0
        #define SAVANNA_WINDOWS 0
    #endif
#endif
