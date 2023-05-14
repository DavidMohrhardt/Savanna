/**
 * @file SavannaCoreConfiguration.h.in
 * @brief Contains the definitions for all Savanna configuration macros. Must match the configuration json file.
 * @version 0.1
 * @date 2022-08-02
 * @author
*/
#pragma once

#define DEBUG_BUILD 1
/* #undef RELEASE_BUILD */

/**
 * @brief
 *
 */
/* #undef SAVANNA_VERSION_MAJOR */

/**
 * @brief
 *
 */
/* #undef SAVANNA_VERSION_MINOR */

/**
 * @brief
 *
 */
/* #undef SAVANNA_VERSION_PATCH */

// Flags
/// Memory Manager
#define SAVANNA_ENABLE_RIGOROUS_MEMORY_VALIDATION 1
#define SAVANNA_ENABLE_BOOT_CONFIG 1

/// Profiling Definitions
/**
 * @brief
 */
#define SAVANNA_DEEP_ENGINE_PROFILING 1

/// Logging
#define SAVANNA_LOG_LEVEL_FATAL 0
#define SAVANNA_LOG_LEVEL_WARNING 1
#define SAVANNA_LOG_LEVEL_DEBUG 2
#define SAVANNA_LOG_LEVEL SAVANNA_LOG_LEVEL_DEBUG

/// Assertions
#define SAVANNA_ASSERTION_LEVEL_NONE 0
#define SAVANNA_ASSERTION_LEVEL_DEBUG 1
#define SAVANNA_ASSERTION_LEVEL SAVANNA_ASSERTION_LEVEL_DEBUG
