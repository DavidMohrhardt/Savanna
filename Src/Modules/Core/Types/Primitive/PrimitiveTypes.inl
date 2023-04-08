#pragma once

#include <stdint.h>

#ifndef __cplusplus

#include <stdbool.h>

#endif

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

#if __cplusplus
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
}
#endif
