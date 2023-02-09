/**
 * @file SavannaAssertions.h
 * @author David Mohrhardt Contains the definitions for all Savanna assertions.
 * @brief
 * @version 0.1
 * @date 2022-08-14
 *
 */
#pragma once

// #define ENABLE_SAVANNA_ASSERTIONS_DEBUGGING 1
#define ENABLE_SAVANNA_ASSERTIONS 1
#define SAVANNA_ENFORCE_MEMORY_SAFETY 1

#include "Utilities/Macros/CppCommonDefs.h"

#if __cplusplus

#include <assert.h>
#include "Types/Exceptions/SavannaException.h"

#else // __cplusplus
#include <cassert>
#endif // end __cplusplus

#if ENABLE_SAVANNA_ASSERTIONS

#define SAVANNA_ASSERT(expression, msg) assert(expression && msg)

#else // ENABLE_SAVANNA_ASSERTIONS

#define SAVANNA_ASSERT(expression, msg)

#endif // end ENABLE_SAVANNA_ASSERTIONS
