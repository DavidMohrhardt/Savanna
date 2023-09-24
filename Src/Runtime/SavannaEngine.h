/**
 * @file SavannaEngine.h
 * @author David Mohrhardt (https://github.com/DavidMohrhardt/Savanna)
 * @brief The main header for the Savanna Engine. This header should be included in all other Savanna headers.
 * @version 0.1
 * @date 2023-01-08
 *
 * @copyright Copyright (c) 2023
 *
 * @note This header allows a lean and mean mode to be enabled. This mode disables all non-essential features of the engine.
 * Additionally, should a header file only want to include the lean and mean features but it's source file wants to include
 * the full features, the header can define SE_LEAN_AND_MEAN before including this header. This will allow the source file
 * to undefine SE_LEAN_AND_MEAN and include this header again to get the rest of the features.
 */
#undef SE_LEAN_AND_MEAN_DEFINED
#if defined(SE_LEAN_AND_MEAN)
    #define SE_LEAN_AND_MEAN_DEFINED 1
    #undef SE_LEAN_AND_MEAN
#else
    #define SE_LEAN_AND_MEAN_DEFINED 0
#endif

#ifndef __SAVANNA_ENGINE_H_MINIMAL__
#define __SAVANNA_ENGINE_H_MINIMAL__

    // Common Types
    #include "Types/Primitive/PrimitiveTypes.inl"
    #include "Types/Primitive/FloatingPointUnions.h"
    #include "Utilities/PreprocessorDefinitions/Definitions.h"

    // Required Managers
    #include "Memory/MemoryManager.h"

#endif // ifndef __SAVANNA_ENGINE_H_MINIMAL__

#if SE_LEAN_AND_MEAN_DEFINED

    // Common Utilities
    #include "Utilities/Console.h"

#else // !SE_LEAN_AND_MEAN_DEFINED

    #ifndef SAVANNA_ENGINE_H_EXTRA__
    #define SAVANNA_ENGINE_H_EXTRA__

    // Coding includes
    #include "Utilities/SavannaCoding.h"

    // Profiling
    #include "Profiling/Profiler.h"
    #include "Utilities/Console.h"

    #endif // end SAVANNA_ENGINE_H_EXTRA__

#endif // end SE_LEAN_AND_MEAN_DEFINED

#ifndef __SAVANNA_ENGINE_H__
#define __SAVANNA_ENGINE_H__

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

SAVANNA_EXPORT(void) SavannaInitializeManagers();
SAVANNA_EXPORT(void) SavannaStartManagers();
SAVANNA_EXPORT(void) SavannaStopManagers();
SAVANNA_EXPORT(void) SavannaShutdownManagers();

#ifdef __cplusplus
namespace Savanna
{
    void InitializeManagers();
    void StartManagers();
    void StopManagers();
    void ShutdownManagers();
} // namespace Savanna
#endif // ifdef __cplusplus

#endif // ifndef __SAVANNA_ENGINE_H__
