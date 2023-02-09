/**
 * @file MathHelpers.h
 * @author David Mohrhardt (https://github.com/DavidMohrhardt/Savanna)
 * @brief TODO @DavidMohrhardt Document
 * @version 0.1
 * @date 2022-08-03
 *
 */
#ifndef __SAVANNA_MATH_HELPERS_H__
#define __SAVANNA_MATH_HELPERS_H__
#pragma once

#include "Utilities/SavannaCoding.h"

#if defined(__cplusplus)
#include <assert.h>
#else
#include <assert.h>
#endif

inline bool IsPowerOfTwo(const se_int64 val)
{
    return val && !(val & (val - 1));
}

inline se_int64 NextPowerOfTwo(const se_int64 val)
{
    return (val & ~(val - 1)) << 1;
}

/**
 * @brief Provides the modulus of a value and a power of two.
 *
 * @param x The value to be modulated.
 * @param pow2Mod The power of two to modulate by.
 * @return The result of x % pow2Mod.
 */
// SAVANNA_NO_DISCARD inline constexpr
inline se_int64 ModByPowerOfTwo(const se_int64 x, const se_int64 pow2Mod)
{
    // As per https://en.wikipedia.org/wiki/Modulo_operation
    // For powers of 2 modulo can be implemented as x % 2n == x & (2n - 1)
    SAVANNA_ASSERT(IsPowerOfTwo(pow2Mod), "pow2Mod must be a power of two");
    return (x & (pow2Mod - 1));
}

/**
 * @brief Get the Required Length To Fill Union object
 *
 * @param largestTypeInUnion
 * @param otherTypeInUnion
 * @return constexpr size_t
 */
inline consteval size_t GetRequiredLengthToFillUnion(size_t largestTypeInUnion, size_t otherTypeInUnion)
{
    return largestTypeInUnion >= otherTypeInUnion
        ? (largestTypeInUnion / otherTypeInUnion)
        : (otherTypeInUnion / largestTypeInUnion);
}
#endif
