/**
 * @file CppCommonDefs.h
 * @author David Mohrhardt (https://github.com/DavidMohrhardt/Savanna)
 * @brief
 * @version 0.1
 * @date 2023-01-26
 *
 * @copyright Copyright (c) 2023
 *
 */
#pragma once

#include "Utilities/PreprocessorDefinitions/CppVersionDefinitions.h"
#include "Utilities/PreprocessorDefinitions/CompilerDefinitions.h"

#if SAVANNA_CPP11
    #define SAVANNA_NO_EXCEPT noexcept
    #define SAVANNA_OVERRIDE override
    #define SAVANNA_FINAL final
    #define SAVANNA_CONSTEXPR constexpr
#else
    #define SAVANNA_NO_EXCEPT
    #define SAVANNA_OVERRIDE
    #define SAVANNA_FINAL
#endif

#if SAVANNA_CPP14
    #define SAVANNA_CONSTEXPR_FUNCTION(ret) constexpr ret
#else
    #define SAVANNA_CONSTEXPR_FUNCTION(ret) ret
#endif

#if SAVANNA_CPP17
    #define SAVANNA_MAYBE_UNUSED [[maybe_unused]]
    #define SAVANNA_NO_DISCARD [[nodiscard]]
    #define SAVANNA_CONSTEXPR_MEMBER constexpr
#else
    #define SAVANNA_NO_DISCARD
    #define SAVANNA_MAYBE_UNUSED
    #define SAVANNA_CONSTEXPR_MEMBER
#endif

#if SAVANNA_CPP20
    #define SAVANNA_CONSTEVAL consteval
    #define SAVANNA_BRANCH_HINT(__hint) [[ __hint ]]
#else
    #define SAVANNA_CONSTEVAL SAVANNA_CONSTEXPR
    #define SAVANNA_BRANCH_HINT(__hint)
#endif

// Definition uses SAVANNA_BRANCH_HINT macro so safe to define outside of #if SAVANNA_CPP20
#define SAVANNA_BRANCH_LIKELY SAVANNA_BRANCH_HINT(likely)
#define SAVANNA_BRANCH_UNLIKELY SAVANNA_BRANCH_HINT(unlikely)
