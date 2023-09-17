/**
 * @file SavannaEngine.h
 * @author David Mohrhardt (https://github.com/DavidMohrhardt/Savanna)
 * @brief
 * @version 0.1
 * @date 2023-01-08
 *
 * @copyright Copyright (c) 2023
 *
 */
#pragma once

#undef SE_LEAN_AND_MEAN_DEFINED
#if defined(SE_LEAN_AND_MEAN)
    #define SE_LEAN_AND_MEAN_DEFINED 1
    #undef SE_LEAN_AND_MEAN
#else
    #define SE_LEAN_AND_MEAN_DEFINED 0
#endif

// Common Types
#include "Types/Primitive/PrimitiveTypes.inl"
#include "Types/Primitive/FloatingPointUnions.h"
#include "Utilities/PreprocessorDefinitions/Definitions.h"

// Include generated export header
// #include "SavannaExport.gen.h"

// Required Managers
#include "Memory/MemoryManager.h"

#if SE_LEAN_AND_MEAN_DEFINED

// Common Utilities
#include "Utilities/Console.h"

#else // !SE_LEAN_AND_MEAN_DEFINED

// Coding includes
#include "Utilities/SavannaCoding.h"

// Profiling
#include "Profiling/Profiler.h"
#include "Utilities/Console.h"

#endif // end SE_LEAN_AND_MEAN_DEFINED

/**
 * @brief General error codes for the engine. Modules may define their own additional error codes.
 */
DEFINE_SAVANNA_ENUM(Savanna, se_ErrorCode_t, ErrorCode, uint32_t,
    SavannaErrorCodeFailure = 0,
    SavannaErrorCodeSuccess = 1,
    SavannaErrorCodeInvalidArgument = 2,
    SavannaErrorCodeOutOfMemory = 3,
    SavannaErrorCodeNotImplemented = 4,
    SavannaErrorCodeNotSupported = 5,
    SavannaErrorCodeUnknown = 6,
);

SAVANNA_EXPORT(void) savanna_initialize_managers();
SAVANNA_EXPORT(void) savanna_start_managers();
SAVANNA_EXPORT(void) savanna_stop_managers();
SAVANNA_EXPORT(void) savanna_shutdown_managers();

#ifdef __cplusplus
namespace Savanna
{
    void InitializeManagers();
    void StartManagers();
    void StopManagers();
    void ShutdownManagers();
} // namespace Savanna
#endif // ifdef __cplusplus
