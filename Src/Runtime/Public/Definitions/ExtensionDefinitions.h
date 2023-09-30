/**
 * @file ISavannaExtension.h
 * @author David Mohrhardt (https://github.com/DavidMohrhardt/Savanna)
 * @brief
 * @version 0.1
 * @date 2023-09-30
 *
 * @copyright Copyright (c) 2023
 *
 */
#ifndef I_SAVANNA_EXTENSION_H
#define I_SAVANNA_EXTENSION_H

#ifdef __cplusplus
#   define SAVANNA_EXTERN extern "C"
#else // !__cplusplus
#   define SAVANNA_EXTERN
#endif // __cplusplus

#ifdef SAVANNA_EXPORTS
#   undef SAVANNA_EXPORTS
#   define SAVANNA_EXPORTS 1
#   define SAVANNA_DECL_SPEC_OPTION dllexport
#else // !SAVANNA_EXPORTS
#   define SAVANNA_EXPORTS 0
#   define SAVANNA_DECL_SPEC_OPTION dllimport
#endif // SAVANNA_EXPORTS

// Savanna Interface API
#if SAVANNA_WINDOWS
#   define SAVANNA_INTERFACE_API __stdcall
#   define SAVANNA_INTERFACE __declspec(SAVANNA_DECL_SPEC_OPTION)
#elif SAVANNA_MACOS // !SAVANNA_WINDOWS
#   define SAVANNA_INTERFACE_API
#   define SAVANNA_INTERFACE
#elif SAVANNA_LINUX // !SAVANNA_WINDOWS && !SAVANNA_MACOS
#   define SAVANNA_INTERFACE_API
#   define SAVANNA_INTERFACE
#else // !SAVANNA_WINDOWS && !SAVANNA_MACOS && !SAVANNA_LINUX
#   error "Unsupported platform."
#endif // SAVANNA_WINDOWS

#if SAVANNA_EXPORTS
#   if SAVANNA_WINDOWS
#       define SAVANNA_EXPORT(ret) SAVANNA_EXTERN __declspec(dllexport) ret SAVANNA_INTERFACE_API
#   else // !SAVANNA_WINDOWS
#       define SAVANNA_EXPORT(ret) SAVANNA_EXTERN ret SAVANNA_INTERFACE_API
#   endif // SAVANNA_WINDOWS
#else // SAVANNA_EXPORTS
#   define SAVANNA_EXPORT(ret) SAVANNA_EXTERN ret SAVANNA_INTERFACE_API
#endif // SAVANNA_EXPORTS

#endif // !I_SAVANNA_EXTENSION_H
