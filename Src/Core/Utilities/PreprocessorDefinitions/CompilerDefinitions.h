#pragma once

#if defined(__clang__) // using clang
        #define SAVANNA_COMPILER_CLANG  1
        #define SAVANNA_COMPILER_GCC    0
        #define SAVANNA_COMPILER_MSVC   0
#elif defined(__GNUG__) // using g++
    // clang actually defines __GNUG__ as well so this check must come
    // after the clang check
    #define SAVANNA_COMPILER_GCC    1
    #define SAVANNA_COMPILER_CLANG  0
    #define SAVANNA_COMPILER_MSVC   0
#elif defined(_MSC_VER) // Using Visual Studio Compiler
    #define SAVANNA_COMPILER_MSVC   1
    #define SAVANNA_COMPILER_CLANG  0
    #define SAVANNA_COMPILER_GCC    0
#else
    #error "Unrecognized compiler!  Please compile using Clang, G++, or MSVC"
#endif