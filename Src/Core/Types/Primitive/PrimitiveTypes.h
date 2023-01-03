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

typedef se_byte           s_byte;
typedef se_ubyte          s_ubyte;

typedef se_uint8          s_uint8;
typedef se_uint16         s_uint16;
typedef se_uint32         s_uint32;
typedef se_uint64         s_uint64;
typedef se_int8           s_int8;
typedef se_int16          s_int16;
typedef se_int32          s_int32;
typedef se_int64          s_int64;

typedef se_intptr         s_intptr;
typedef se_uintptr        s_uintptr;

#if __cplusplus
namespace Savanna
{
    // Namespaced typedefs
    typedef se_byte       byte;
    typedef se_ubyte      ubyte;

    typedef se_uint8      uint8;
    typedef se_uint16     uint16;
    typedef se_uint32     uint32;
    typedef se_uint64     uint64;
    typedef se_int8       int8;
    typedef se_int16      int16;
    typedef se_int32      int32;
    typedef se_int64      int64;

    typedef se_intptr     intptr;
    typedef se_uintptr    uintptr;
}
#endif
