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

#include "Public/ISavannaEngine.h"
#include "Utilities/Macros/CppCommonDefs.h"

namespace Savanna
{
    template<size_t N>
    SAVANNA_CONSTEXPR_FUNCTION(char) (&StringLengthHelper(const char (&str)[N]))[N]
    {
        return str;
    }

    template<size_t N>
    SAVANNA_CONSTEXPR_FUNCTION(const char*) LiteralString(const char (&str)[N])
    {
        return str;
    }
} // namespace Savanna
