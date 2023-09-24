/**
 * @file GfxContextGfxContextErrorCodes.h
 * @author David Mohrhardt (https://github.com/DavidMohrhardt/Savanna)
 * @brief Provides enumeration values for possible error codes returned by a GfxContext
 * @version 0.1
 * @date 2023-09-17
 *
 * @copyright Copyright (c) 2023
 *
 */
#pragma once

#include <SavannaEngine.h>
#include <Utilities/SavannaCoding.h>

#include "GfxApi.h"

/**
 * @brief A macro that returns true if the given error code indicates success.
 *
 */
#define SE_GFX_CONTEXT_IS_SUCCESS(__errCodeOrCode) \
    ((__errCodeOrCode) == se_GfxContextCode_Success)

/**
 * @brief A macro that returns true if the given error code indicates failure.
 *
 */
#define SE_GFX_CONTEXT_IS_FAILURE(__errCodeOrCode) \
    ((__errCodeOrCode) != se_GfxContextCode_Success)

/**
 * @brief The error code in question. This is a macro that will generate 2 types:
 * - se_GfxContextErrorCode_t: The C-level enum type
 * - Savanna::Gfx::::GfxContextErrorCode: The C++-level enum type
 *
 * @note GfxContextErrorCode is not a simple typedef. It is it's own type
 * with a base class of Enumeration<GfxContextErrorCode>. For more information
 * on this, see the documentation for Enumeration<T>.
 *
 */
DEFINE_SAVANNA_ENUM(Savanna, se_GfxContextErrorCode_t, GfxContextErrorCode, uint32_t,
    se_GfxContextCode_Invalid = 0,
    se_GfxContextCode_Success,

    // Errors
    se_GfxContextCode_NotImplemented,
    se_GfxContextCode_ApiNotSupported,

    se_GfxContextCode_AlreadyInitialized,

    se_GfxContextCode_Unknown,

    // Only count the real error codes
    se_GfxContextCode_Count,

    // These are not real error codes, but are used to indicate success or failure
    se_GfxContextCode_Failure = se_GfxContextCode_NotImplemented,
);

#define SE_GFX_CONTEXT_ERROR_CODE_TO_STRING_CASE(code) \
    case code: return #code;

inline const char* se_GfxContextErrorCodeToString(const se_GfxContextErrorCode_t code)
{
    switch (code)
    {
        SE_GFX_CONTEXT_ERROR_CODE_TO_STRING_CASE(se_GfxContextCode_Invalid)
        SE_GFX_CONTEXT_ERROR_CODE_TO_STRING_CASE(se_GfxContextCode_Success)

        SE_GFX_CONTEXT_ERROR_CODE_TO_STRING_CASE(se_GfxContextCode_NotImplemented)
        SE_GFX_CONTEXT_ERROR_CODE_TO_STRING_CASE(se_GfxContextCode_ApiNotSupported)

        default:
        SE_GFX_CONTEXT_ERROR_CODE_TO_STRING_CASE(se_GfxContextCode_Unknown)
    }
}

#undef SE_GFX_CONTEXT_ERROR_CODE_TO_STRING_CASE
