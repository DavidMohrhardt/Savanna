/**
 * @file SavannaAssertions.h
 * @author David Mohrhardt Contains the definitions for all Savanna assertions.
 * @brief
 * @version 0.1
 * @date 2022-08-14
 *
 * @copyright Copyright (c) 2022
 *
 */
#pragma once

// #define ENABLE_SAVANNA_ASSERTIONS_DEBUGGING 1
#define ENABLE_SAVANNA_ASSERTIONS 1
#define SAVANNA_ENFORCE_MEMORY_SAFETY 1

#include "Utilities/Macros/CppAttributes.h"
#include "Utilities/Macros/CppCommonDefs.h"

#if __cplusplus

#include <cassert>
#include "Types/Exceptions/SavannaException.h"

#else // __cplusplus
#include <assert.h>
#endif // end __cplusplus

#if ENABLE_SAVANNA_ASSERTIONS

#define SAVANNA_ASSERT(expression) assert(expression)

#else // ENABLE_SAVANNA_ASSERTIONS

#define SAVANNA_ASSERT(expression)

#endif // end ENABLE_SAVANNA_ASSERTIONS

#if SAVANNA_ENFORCE_MEMORY_SAFETY

#define SAVANNA_MEMORY_SAFETY_ASSERT(condition, message) assert(condition && "MemorySafetyAssertion::" message);

#else

#define SAVANNA_MEMORY_SAFETY_ASSERT(condition, message)

#endif

#if ENABLE_SAVANNA_ASSERTIONS_DEBUGGING && __cplusplus

#undef SAVANNA_ASSERT
#define SAVANNA_ASSERT(expression) \
    if (!(condition)) \
    { \
        throw Savanna::RuntimeErrorException(); \
    } \
    assert(condition)

#undef SAVANNA_MEMORY_SAFETY_ASSERT
#define SAVANNA_MEMORY_SAFETY_ASSERT(condition, message) \
    if (!(condition)) \
    { \
        throw Savanna::BadAllocationException(); \
    } \
    assert(condition)

#endif