/**
 * @file ISavannaEngine.h
 * @author David Mohrhardt (https://github.com/DavidMohrhardt/Savanna)
 * @brief The primary public header for the Savanna game engine.
 * @version 0.1
 * @date 2023-09-30
 *
 * @copyright Copyright (c) 2023
 *
 * @note This file is included by all other public headers and the vast majority of source files.
 *
 */
#ifndef I_SAVANNA_ENGINE_H
#define I_SAVANNA_ENGINE_H

// Required
#include "ISavannaDefinitions.h"
#include <stdint.h>

// Use C99 bool type
#include <stdbool.h>

// Global namespace typedefs
typedef bool                se_bool;
typedef char                se_byte;
typedef unsigned char       se_ubyte;

typedef uint8_t             se_uint8;
typedef uint16_t            se_uint16;
typedef uint32_t            se_uint32;
typedef uint64_t            se_uint64;
typedef int8_t              se_int8;
typedef int16_t             se_int16;
typedef int32_t             se_int32;
typedef int64_t             se_int64;
typedef intptr_t            se_intptr;
typedef uintptr_t           se_uintptr;

typedef size_t              se_size;

typedef se_int16            se_half_word;
typedef se_uint16           se_uhalf_word;
typedef se_int32            se_word;
typedef se_uint32           se_uword;
typedef se_int64            se_double_word;
typedef se_uint64           se_udouble_word;

// Union quadword
typedef union se_quadword
{
    se_byte         bytes[16];
    se_ubyte        ubytes[16];

    se_half_word    halfwords[8];
    se_uhalf_word   uhalfwords[8];

    se_word         words[4];
    se_uword        uwords[4];

    se_double_word  dwords[2];
    se_udouble_word udwords[2];
} se_quadword;

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

/**
 * @brief
 *
 */
typedef struct se_PreAndPostCallbacks_t
{
    void (*m_pfnBefore)(void* pUserData);
    void (*m_pfnAfter)(void* pUserData);
} se_BeforeAfterCallbacks_t;

/**
 * @brief Initializes the Savanna engine. Initializes all subsystems.
 *
 */
SAVANNA_EXPORT(void) SavannaInitialize();

/**
 * @brief Starts the Savanna engine.
 *
 * @note The calling thread for this function will be considered main thread for the engine.
 *
 */
SAVANNA_EXPORT(void) SavannaStart();

/**
 * @brief Stops the Savanna engine.
 *
 * @note The calling thread must be the main thread for the engine.
 *
 */
SAVANNA_EXPORT(void) SavannaStop();

/**
 * @brief Shuts down the Savanna engine.
 *
 * @note Implicitly calls SavannaStop().
 *
 */
SAVANNA_EXPORT(void) SavannaShutdown();

#endif // !I_SAVANNA_ENGINE_H
