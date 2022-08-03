#pragma once

#include "CompilerDefinitions.h"

#if SAVANNA_COMPILER_MSVC

/**
 * @brief Defines interface exporting to be __declspec(dllexport).
 */
#define SAVANNA_INTERFACE_EXPORT __declspec(dllexport)

#else

/**
 * @brief Defines interface exporting to be nothing.
 */
#define SAVANNA_INTERFACE_EXPORT

#endif

/**
 * @brief Defines the common calling convention for interface functions to be __cdecl
 */
#define SAVANNA_INTERFACE_CALL __cdecl

#define SAVANNA_EXPORT(returnType) extern "C" SAVANNA_INTERFACE_EXPORT returnType SAVANNA_CALL