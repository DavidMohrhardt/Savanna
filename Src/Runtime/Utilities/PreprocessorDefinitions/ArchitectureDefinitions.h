#pragma once

#include "PlatformDefinitions.h"
#include "CompilerDefinitions.h"

// Architecture references found https://sourceforge.net/p/predef/wiki/Architectures/
#if SAVANNA_WINDOWS
    #if SAVANNA_COMPILER_CLANG // using clang
        #if defined(i386) || defined(__i386) || defined(__i386__)
            #define SAVANNA_ARCH_X86 1
        #endif // X86

        #if (__aarch64__)
            #define SAVANNA_ARCH_ARM64 1
        #endif

        #if defined(__arm__)
            #define SAVANNA_ARCH_ARM 1
        #endif // ARM architecture
    #elif SAVANNA_COMPILER_GCC // using g++
        // clang actually defines __GNUG__ as well so this check must come
        // after the clang check
        #if defined(i386) || defined(__i386) || defined(__i386__)
            #define SAVANNA_ARCH_X86 1
        #endif // X86

        #if (__aarch64__)
            #define SAVANNA_ARCH_ARM64 1
        #endif

        #if defined(__arm__)
            #define SAVANNA_ARCH_ARM 1
        #endif // ARM architecture
    #elif SAVANNA_COMPILER_MSVC // Using MSVC
        #pragma message("Compiling Using MSVC Toolchain")
        #if defined(_M_IX86) || defined(_M_X86)
            #pragma message("Target Architecture is X86")
            #define SAVANNA_ARCH_X86 1
        #endif // X86

        #if defined(_M_IX64) || defined(_M_X64)
            #pragma message("Target Architecture includes X64")
            #define SAVANNA_ARCH_X64 1
        #endif // X64

        #if defined(_M_AMD64)
            #pragma message("Target Architecture includes AMD64")
            #define SAVANNA_ARCH_AMDX64 1
        #endif // AMD X64

        #if defined(_M_ARM)
            #pragma message("Target Architecture is ARM")
            #define SAVANNA_ARCH_ARM 1
        #endif // ARM architecture
    #endif
#elif SAVANNA_DARWIN
    #error "Apple is currently not supported"
    // #if SAVANNA_COMPILER_CLANG // using clang
    // #elif SAVANNA_COMPILER_GCC // using g++

    // #endif
#elif SAVANNA_LINUX
    #if SAVANNA_COMPILER_CLANG // using clang
        #if defined(i386) || defined(__i386) || defined(__i386__)
            #define SAVANNA_ARCH_X86 1
        #endif // X86

        #if (__aarch64__)
            #define SAVANNA_ARCH_ARM64 1
        #endif

        #if defined(__arm__)
            #define SAVANNA_ARCH_ARM 1
        #endif // ARM architecture
    #elif SAVANNA_COMPILER_GCC // using g++
        // clang actually defines __GNUG__ as well so this check must come
        // after the clang check
        #if defined(i386) || defined(__i386) || defined(__i386__)
            #define SAVANNA_ARCH_X86 1
        #endif // X86

        #if (__aarch64__)
            #define SAVANNA_ARCH_ARM64 1
        #endif

        #if defined(__arm__)
            #define SAVANNA_ARCH_ARM 1
        #endif // ARM architecture
    #endif
#else
    #error "Unsupported platform target!"
#endif

// Check unknown architecture
// #if SAVANNA_ARCH_ARM && SAVANNA_ARCH_AMDX64 && SAVANNA_ARCH_X86
//     // Assert that the engine will be able to compile the appropriate inline
//     // assembly by checking that we have a known architecture
//     #error "The current architecture is not known or unsupported!"
// #endif