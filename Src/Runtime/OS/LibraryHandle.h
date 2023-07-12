#pragma once

// Core Engine Includes
#include <SavannaEngine.h>
#include <Utilities/SavannaCoding.h>

namespace Savanna::OS
{

#if defined(SAVANNA_WINDOWS)
    #define WINDOWS_LEAN_AND_MEAN 1
    #include <Windows.h>
#endif

// Used from https://github.com/GameTechDev/IntroductionToVulkan/blob/master/Project/Common/OperatingSystem.h
#if defined(SAVANNA_WINDOWS)
    typedef HMODULE LibraryHandle;
    constexpr LibraryHandle k_InvalidLibraryHandle = nullptr;
#elif defined(SAVANNA_MACOS) || defined(SAVANNA_LINUX) || defined(SAVANNA_ANDROID)
    typedef void* LibraryHandle;
    constexpr LibraryHandle k_InvalidLibraryHandle = nullptr;
#endif
} // namespace Savanna::OS
