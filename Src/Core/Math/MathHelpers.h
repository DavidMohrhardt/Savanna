/**
 * @file MathHelpers.h
 * @author David Mohrhardt (https://github.com/DavidMohrhardt/Savanna)
 * @brief TODO @DavidMohrhardt Document
 * @version 0.1
 * @date 2022-08-03
 *
 */

#pragma once

#include "Utilities/SavannaCoding.h"
#include "Types/Exceptions/SavannaException.h"

#include <cassert>

SAVANNA_NO_DISCARD inline constexpr bool IsPowerOfTwo(const auto& val)
{
    return val && !(val & (val - 1));
}

#define IMPLEMENT_FAST_MODULO_POWER_OF_2_FOR_TYPE(__type) \
    SAVANNA_NO_DISCARD inline constexpr __type ModByPowerOfTwo(const __type x, const __type pow2Mod) \
    { \
        assert(IsPowerOfTwo(pow2Mod), "pow2Mod must be a power of two"); \
        return (x & (pow2Mod - 1)); \
    }

/**
 * @brief Provides the modulus of a value and a power of two.
 *
 * @param x The value to be modulated.
 * @param pow2Mod The power of two to modulate by.
 * @return The result of x % pow2Mod.
 */
SAVANNA_NO_DISCARD inline constexpr auto ModByPowerOfTwo(const auto x, const auto pow2Mod)
{
    // As per https://en.wikipedia.org/wiki/Modulo_operation
    // For powers of 2 modulo can be implemented as x % 2n == x & (2n - 1)
    assert(IsPowerOfTwo(pow2Mod) && "pow2Mod must be a power of two");
    return (x & (pow2Mod - 1));
}

#undef IMPLEMENT_FAST_MODULO_POWER_OF_2_FOR_TYPE
