/**
 * @file LiteralString.h
 * @author David Mohrhardt (https://github.com/DavidMohrhardt/Savanna)
 * @brief
 * @version 0.1
 * @date 2022-08-15
 *
 * @reference https://stackoverflow.com/questions/13724242/c11-function-that-only-accepts-string-literals
 *
 */

#pragma once

#include "Types/Primitive/PrimitiveTypes.h"

namespace Savanna
{
    template<size_t N>
    constexpr char (&StringLengthHelper(const char (&str)[N]))[N]
    {
        return str;
    }

    template<size_t N>
    constexpr const char* LiteralString(const char (&str)[N])
    {
        return str;
    }
    // // these are used to force constant, literal strings in sqfish binding names
    // // which allows to store/copy just the pointer without having to manage
    // // allocations and memory copies
    // struct _LiteralString
    // {
    //     // these functions are just for easy usage... not needed
    //     // the struct can be empty
    //     bool equal(_LiteralString const *other) { return !strcmp((const char *)this, (const char *)other); }
    //     bool equal(const char *other) { return !strcmp((const char *)this, other); }
    //     constexpr const char *str(void) { return (const char *)this; }
    //     bool empty(void) { return *(const char *)this == 0; }
    // };

    // typedef _LiteralString *LiteralString;

    // constexpr LiteralString operator "" _LIT(const char *s, size_t)
    // {
    //     return (LiteralString)s;
    // }
} // namespace Savanna
