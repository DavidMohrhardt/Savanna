#pragma once

#include "Utilities/PreprocessorDefinitions/PlatformDefinitions.h"

#if SAVANNA_WINDOWS

#define SELECT_PLATFORM_HEADER(__headerName) \
    "Windows/" __headerName

#elif SAVANNA_MACOS

#define SELECT_PLATFORM_HEADER(__headerName) \
    "MacOS/" __headerName

#elif SAVANNA_LINUX

#define SELECT_PLATFORM_HEADER(__headerName) \
    "Linux/" __headerName

#else

#error Unsupported platform.

#define SELECT_PLATFORM_HEADER(__headerName)


#endif //
