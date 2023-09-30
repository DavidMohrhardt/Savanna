/**
 * @file CppVersionDefinitions.h
 * @author David Mohrhardt (https://github.com/DavidMohrhardt/Savanna)
 * @brief
 * @version 0.1
 * @date 2023-01-26
 *
 * @copyright Copyright (c) 2023
 *
 */
#pragma once

#define SAVANNA_CPP98 0
#define SAVANNA_CPP11 0
#define SAVANNA_CPP14 0
#define SAVANNA_CPP17 0
#define SAVANNA_CPP20 0
#define SAVANNA_CPP23 0

#ifdef __cplusplus

// Define C++ version macros from 11 to 23 in reverse order
// (from 23 to 11) to avoid compiler warnings about
// redefinition of macros.

#if __cplusplus >= 202003L
    #define SAVANNA_CPP_VERSION 23
#elif __cplusplus >= 202002L
    #define SAVANNA_CPP_VERSION 20
#elif __cplusplus >= 201703L
    #define SAVANNA_CPP_VERSION 17
#elif __cplusplus >= 201402L
    #define SAVANNA_CPP_VERSION 14
#elif __cplusplus >= 201103L
    #define SAVANNA_CPP_VERSION 11
#else
    #define SAVANNA_CPP_VERSION 98
#endif

#if SAVANNA_CPP_VERSION >= 11
    #undef SAVANNA_CPP11
    #define SAVANNA_CPP11 1
#endif

#if SAVANNA_CPP_VERSION >= 14
    #undef SAVANNA_CPP14
    #define SAVANNA_CPP14 1
#endif

#if SAVANNA_CPP_VERSION >= 17
    #undef SAVANNA_CPP17
    #define SAVANNA_CPP17 1
#endif

#if SAVANNA_CPP_VERSION >= 20
    #undef SAVANNA_CPP20
    #define SAVANNA_CPP20 1
#endif

#if SAVANNA_CPP_VERSION >= 23
    #undef SAVANNA_CPP23
    #define SAVANNA_CPP23 1
#endif

#endif
