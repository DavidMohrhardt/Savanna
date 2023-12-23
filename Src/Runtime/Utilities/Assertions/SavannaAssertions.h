/**
 * @file SavannaAssertions.h
 * @author David Mohrhardt Contains the definitions for all Savanna assertions.
 * @brief TODO @David.Mohrhardt Document
 * @version 0.1
 * @date 2022-08-14
 *
 */
#pragma once

#include "SavannaConfiguration.gen.h"
#include "Utilities/Macros/CppCommonDefs.h"

#if __cplusplus

#include <assert.h>


#else // __cplusplus
#include <cassert>
#endif // end __cplusplus

#define SAVANNA_ASSERT(expression, msg) assert(expression && msg)

#if SAVANNA_ASSERTION_LEVEL > 0

#   define SAVANNA_DEBUG_ASSERT(expression, msg) assert(expression && msg)

#else

#   define SAVANNA_DEBUG_ASSERT(expression, msg)

#endif // end SAVANNA_ASSERTION_LEVEL > 0
