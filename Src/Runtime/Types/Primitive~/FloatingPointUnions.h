/**
 * @file FloatingPointUnions.h
 * @author David Mohrhardt (https://github.com/DavidMohrhardt/Savanna)
 * @brief This is a file that is used to define unions for definitions for floating point types allowing access to individual bytes as needed.
 * @version 0.1
 * @date 2022-08-10
 *
 */
#pragma once

#include "PrimitiveTypes.inl"

typedef union se_float16
{
    se_uint8   m_UBytes[2];
    se_uint16  m_UInt16;
    se_int8    m_Bytes[2];
    se_int16   m_Int16;

    struct
    {
        se_uint16 m_Sign : 1;
        se_uint16 m_Exponent : 5;
        se_uint16 m_Mantissa : 10;
    };
} se_float16;
typedef se_float16 se_half16;

/**
 * @brief TODO @DavidMohrhardt Document
 *
 */
typedef union se_float32
{
    float       m_SinglePrecision;

    se_int32    m_AsInt;
    se_uint16   m_UHalfs[2];
    se_uint8    m_UBytes[4];

    se_uint32   m_AsUInt;
    se_int16    m_Halfs[2];
    se_int8     m_Bytes[4];

    struct
    {
        se_uint32 m_Sign : 1;
        se_uint32 m_Exponent : 8;
        se_uint32 m_Mantissa : 23;
    };
} se_float32;

/**
 * @brief TODO @DavidMohrhardt Document
 *
 */
typedef union se_float64
{
    double     m_DoublePrecision;
    float      m_SinglePrecision[2];

    se_uint64  m_UInt64;
    se_uint32  m_UInt32s[2];
    se_uint16  m_UInt16[4];
    se_uint8   m_UBytes[8];

    se_int64   m_Int64;
    se_int32   m_Int32s[2];
    se_int16   m_Int16[4];
    se_int8    m_Bytes[8];

    struct
    {
        se_uint64 m_Sign : 1;
        se_uint64 m_Exponent : 11;
        se_uint64 m_Mantissa : 52;
    };
} se_float64;
typedef se_float64 se_double64;

#ifdef __cplusplus

namespace Savanna
{
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

#endif // __cplusplus
