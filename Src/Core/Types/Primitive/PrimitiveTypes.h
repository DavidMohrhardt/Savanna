#pragma once

#include <stdint.h>

// Global namespace typedefs
typedef char                __se_byte;
typedef unsigned char       __se_ubyte;

typedef uint8_t             __se_uint8;
typedef uint16_t            __se_uint16;
typedef uint32_t            __se_uint32;
typedef uint64_t            __se_uint64;
typedef int8_t              __se_int8;
typedef int16_t             __se_int16;
typedef int32_t             __se_int32;
typedef int64_t             __se_int64;
typedef intptr_t            __se_intptr;
typedef uintptr_t           __se_uintptr;

typedef __se_byte           sbyte;
typedef __se_ubyte          subyte;

typedef __se_uint8          suint8;
typedef __se_uint16         suint16;
typedef __se_uint32         suint32;
typedef __se_uint64         suint64;
typedef __se_int8           sint8;
typedef __se_int16          sint16;
typedef __se_int32          sint32;
typedef __se_int64          sint64;

typedef __se_intptr         sintptr;
typedef __se_uintptr        suintptr;

#if __cplusplus
namespace Savanna
{
    // Namespaced typedefs
    typedef __se_byte       byte;
    typedef __se_ubyte      ubyte;

    typedef __se_uint8      uint8;
    typedef __se_uint16     uint16;
    typedef __se_uint32     uint32;
    typedef __se_uint64     uint64;
    typedef __se_int8       int8;
    typedef __se_int16      int16;
    typedef __se_int32      int32;
    typedef __se_int64      int64;

    typedef __se_intptr     intptr;
    typedef __se_uintptr    uintptr;
}
#endif