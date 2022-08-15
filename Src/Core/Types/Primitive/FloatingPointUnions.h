/**
 * @file FloatingPointUnions.h
 * @author David Mohrhardt (DavidMohrhardt@github.com)
 * @brief This is a file that is used to define unions for definitions for floating point types allowing access to individual bytes as needed.
 * @version 0.1
 * @date 2022-08-10
 *
 * @copyright Copyright (c) 2022
 *
 */
#pragma once

#include "PrimitiveTypes.h"
#include "Utilities/Macros/CppTypeDefs.h"

/**
 * @brief TODO @DavidMohrhardt Document
 *
 */
typedef union __se_float32
{
    __se_uint8   m_UBytes[4];
    __se_uint16  m_UHalfs[2];
    __se_uint32  m_AsUInt;
    __se_int8    m_Bytes[4];
    __se_int16   m_Halfs[2];
    __se_int32   m_AsInt;
    float   m_SinglePrecision;
} __se_float32;

/**
 * @brief A namespaced type definition for the type __se_float32.
 */
DECLARE_SAVANNA_NAMESPACED_CPP_TYPE_DEF(__se_float32, float32);

/**
 * @brief TODO @DavidMohrhardt Document
 *
 */
typedef union __se_float64
{
    __se_uint8   m_UBytes[8];
    __se_uint16  m_UInt16[4];
    __se_uint32  m_UInt32s[2];
    __se_uint64  m_UInt64;
    __se_int8    m_Bytes[8];
    __se_int16   m_Int16[4];
    __se_int32   m_Int32s[2];
    __se_int64   m_Int64;
    float   m_SinglePrecision[2];
    double  m_DoublePrecision;
} __se_float64;
typedef __se_float64 __se_double64;

DECLARE_SAVANNA_NAMESPACED_CPP_TYPE_DEF(__se_float64, float64);
DECLARE_SAVANNA_NAMESPACED_CPP_TYPE_DEF(__se_double64, double64);
