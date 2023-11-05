/**
 * @file ISavannaEngine.hpp
 * @author David Mohrhardt (https://github.com/DavidMohrhardt/Savanna)
 * @brief The C++ version of the ISavannaEngine.h header. Provides C++ types and utilities
 * for the engine.
 * @version 0.1
 * @date 2023-09-30
 *
 * @copyright Copyright (c) 2023
 *
 */
#ifndef I_SAVANNA_ENGINE_HPP
#define I_SAVANNA_ENGINE_HPP
#include "ISavannaEngine.h"

namespace Savanna
{
    using byte = se_byte;
    using ubyte = se_ubyte;

    using uint8 = se_uint8;
    using uint16 = se_uint16;
    using uint32 = se_uint32;
    using uint64 = se_uint64;
    using int8 = se_int8;
    using int16 = se_int16;
    using int32 = se_int32;
    using int64 = se_int64;

    using half_word = se_half_word;
    using uhalf_word = se_uhalf_word;

    using word = se_word;
    using uword = se_uword;

    using double_word = se_double_word;
    using udouble_word = se_udouble_word;

    using quad_word = se_quadword;
    using uquad_word = se_quadword;

    using intptr = se_intptr;
    using uintptr = se_uintptr;

    using size = se_size;

    struct float16
    {
        se_float16 m_Value;

        float16() = default;
        float16(const float16&) = default;
        float16(float16&&) = default;
        float16& operator=(const float16&) = default;
        float16& operator=(float16&&) = default;

        constexpr float16(float value)
            : m_Value{ .m_Sign = 0, .m_Exponent = 0, .m_Mantissa = 0 }
        {
            se_float32 temp = { .m_SinglePrecision = value };
            m_Value.m_Sign = temp.m_Sign;
            m_Value.m_Exponent = temp.m_Exponent;
            m_Value.m_Mantissa = temp.m_Mantissa;
        }

        constexpr operator float()
        {
            se_float32 temp = { .m_Sign = m_Value.m_Sign, .m_Exponent = m_Value.m_Exponent, .m_Mantissa = m_Value.m_Mantissa };
            return temp.m_SinglePrecision;
        }

        constexpr operator se_float16&()
        {
            return m_Value;
        }
    };

    struct float32
    {
        se_float32 m_Value;

        float32() = default;
        float32(const float32&) = default;
        float32(float32&&) = default;
        float32& operator=(const float32&) = default;
        float32& operator=(float32&&) = default;

        constexpr float32(float value)
            : m_Value{ .m_SinglePrecision = value }
        {}

        constexpr operator float()
        {
            return m_Value.m_SinglePrecision;
        }

        constexpr operator float&()
        {
            return m_Value.m_SinglePrecision;
        }

        constexpr operator se_float32&()
        {
            return m_Value;
        }
    };

    struct float64
    {
        se_float64 m_Value;

        float64() = default;
        float64(const float64&) = default;
        float64(float64&&) = default;
        float64& operator=(const float64&) = default;
        float64& operator=(float64&&) = default;

        constexpr float64(double value)
            : m_Value{ .m_DoublePrecision = value }
        {}

        constexpr operator double()
        {
            return m_Value.m_DoublePrecision;
        }

        constexpr operator double&()
        {
            return m_Value.m_DoublePrecision;
        }

        constexpr operator se_float64&()
        {
            return m_Value;
        }
    };

    using half = float16;
    using double64 = float64;
}

#endif // !I_SAVANNA_ENGINE_HPP
