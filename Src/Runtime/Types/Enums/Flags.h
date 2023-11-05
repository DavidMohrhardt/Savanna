/**
 * @file Flags.h
 * @author David Mohrhardt (https://github.com/DavidMohrhardt/Savanna)
 * @brief
 * @version 0.1
 * @date 2023-01-08
 *
 * @copyright Copyright (c) 2023
 *
 */
#pragma once

#include "Enumeration.h"

#define DECLARE_CPP_FLAG_ENUMERATION(__cppName, __cName, __value_type)
#define DECLARE_NAMESPACED_CPP_FLAG_ENUMERATION(__nameSpace, __cppName, __cName, __value_type)
#define DECLARE_NAMESPACED_EXTERNAL_CPP_FLAG_ENUMERATION(__nameSpace, __cppName, __cName, __external_enum, __value_type)

/**
 * @brief Defines a C flag enumeration.
 *
 * Example:
 * @code{.c}
 * DEFINE_C_FLAG_ENUMERATION(se_GfxBackend_t, se_uint32,
 *    kSavannaGfxApiNone = 0,
 *    kSavannaGfxApiVulkan,
 * );
 * // Becomes:
 * // typedef enum se_GfxBackend_t : se_uint32 { kSavannaGfxApiNone = 0, kSavannaGfxApiVulkan } se_GfxBackend_t;
 * @endcode
 *
 * @arg __cName The name of the C enumeration.
 * @arg __value_type The underlying type of the C enumeration.
 * @arg ... The enumeration values.
 *
 */
#define DEFINE_C_FLAG_ENUMERATION(__cName, __value_type, ...) \
    typedef enum __cName : __value_type { __VA_ARGS__ } __cName;

#if defined(__cplusplus)

#include <bitset>
#include <concepts>
#include <type_traits>

/**
 * @brief Declares a C++ flag enumeration wrapper for a C enumeration.
 *
 * Example:
 * @code{.cpp}
 * DECLARE_CPP_FLAG_ENUMERATION(GfxApi, se_GfxBackend_t, se_uint32);
 * // Becomes:
 * // using GfxApi = Savanna::FlagEnumeration<se_GfxBackend_t, se_uint32>;
 * @endcode
 *
 * @arg __cppName The name of the C++ enumeration.
 * @arg __cName The name of the C enumeration.
 * @arg __value_type The underlying type of the C enumeration.
 *
 * @note This macro is empty when compiling for C.
 *
 */
#undef DECLARE_CPP_FLAG_ENUMERATION
#define DECLARE_CPP_FLAG_ENUMERATION(__cppName, __cName, __value_type) \
    using __cppName = Savanna::FlagEnumeration<__cName, __value_type>

/**
 * @brief Declares a namespaced C++ flag enumeration wrapper for a C enumeration.
 * Uses the DECLARE_CPP_FLAG_ENUMERATION macro internally.
 *
 * Example:
 * @code{.cpp}
 * DECLARE_NAMESPACED_CPP_FLAG_ENUMERATION(Savanna::Gfx, GfxApi, se_GfxBackend_t, se_uint32);
 * // Becomes:
 * // namespace Savanna::Gfx { using GfxApi = Savanna::FlagEnumeration<se_GfxBackend_t, se_uint32>; }
 * @endcode
 *
 * @arg __nameSpace The namespace to declare the enumeration in.
 * @arg __cppName The name of the C++ enumeration.
 * @arg __cName The name of the C enumeration.
 * @arg __value_type The underlying type of the C enumeration.
 *
 * @note This macro is empty when compiling for C.
 *
 */
#undef DECLARE_NAMESPACED_CPP_FLAG_ENUMERATION
#define DECLARE_NAMESPACED_CPP_FLAG_ENUMERATION(__nameSpace, __cppName, __cName, __value_type) \
    namespace __nameSpace { DECLARE_CPP_FLAG_ENUMERATION(__cppName, __cName, __value_type); }

/**
 * @brief Declares a namespaced C++ flag enumeration wrapper for an externally defined
 * enumeration. This is useful for wrapping enumerations from external libraries, particularly
 * when the external library is written in C as it will provide a C++ wrapper and default
 * conversion operators for the external flag enumeration.
 *
 * Example:
 * @code{.cpp}
 * DECLARE_NAMESPACED_EXTERNAL_CPP_FLAG_ENUMERATION(Savanna::Gfx, GfxApi, se_GfxBackend_t, se_GfxBackend_t, se_uint32);
 * // Becomes:
 * // namespace Savanna::Gfx { using GfxApi = Savanna::ExternalFlagEnumeration<se_GfxBackend_t, se_GfxBackend_t, se_uint32>; }
 * @endcode
 *
 * @arg __nameSpace The namespace to declare the enumeration in.
 * @arg __cppName The name of the C++ enumeration.
 * @arg __cName The name of the C enumeration.
 * @arg __external_enum The name of the external enumeration.
 * @arg __value_type The underlying type of the C enumeration.
 *
 */
#undef DECLARE_NAMESPACED_EXTERNAL_CPP_FLAG_ENUMERATION
#define DECLARE_NAMESPACED_EXTERNAL_CPP_FLAG_ENUMERATION(__nameSpace, __cppName, __cName, __external_enum, __value_type) \
    namespace __nameSpace { using __cppName = Savanna::ExternalFlagEnumeration<__cName, __external_enum, __value_type>; }

#endif // defined(__cplusplus)

#define DEFINE_FLAG_ENUMERATION(__cppName, __cName, __value_type, ...) \
    DEFINE_C_FLAG_ENUMERATION(__cName, __value_type, __VA_ARGS__) \
    DECLARE_CPP_FLAG_ENUMERATION(__cppName, __cName, __value_type)

#define DEFINE_SAVANNA_FLAG_ENUMERATION(__cppName, __cName, __value_type, ...) \
    DEFINE_C_FLAG_ENUMERATION(__cName, __value_type, __VA_ARGS__) \
    DECLARE_NAMESPACED_CPP_FLAG_ENUMERATION(Savanna, __cppName, __cName, __value_type)

#define DEFINE_NAMESPACED_FLAG_ENUMERATION(__nameSpace, __cppName, __cName, __value_type, ...) \
    DEFINE_C_FLAG_ENUMERATION(__cName, __value_type, __VA_ARGS__) \
    DECLARE_NAMESPACED_CPP_FLAG_ENUMERATION(__nameSpace, __cppName, __cName, __value_type)

#define DEFINE_SAVANNA_EXTERNAL_FLAG_ENUMERATION(__nameSpace, __cppName, __cName, __external_enum, __value_type, ...) \
    DEFINE_C_FLAG_ENUMERATION(__cName, __value_type, __VA_ARGS__) \
    DECLARE_NAMESPACED_EXTERNAL_CPP_FLAG_ENUMERATION(__nameSpace, __cppName, __cName, __external_enum, __value_type)

#if defined(__cplusplus)

namespace Savanna
{
    template <typename enum_type, typename value_type>
    struct FlagEnumeration : public Enumeration<enum_type, value_type>
    {
        FlagEnumeration() = default;
        FlagEnumeration(const enum_type& value) : Enumeration<enum_type, value_type>(value) {}
        FlagEnumeration(const value_type& value) : Enumeration<enum_type, value_type>(value) {}
        FlagEnumeration(const Enumeration<enum_type, value_type>& other) : Enumeration<enum_type, value_type>(other) {}
        FlagEnumeration(Enumeration<enum_type, value_type>&& other) : Enumeration<enum_type, value_type>(std::move(other)) {}
        ~FlagEnumeration() = default;

        bool Any() const { return static_cast<value_type>(this->m_Value) != static_cast<value_type>(0); }

        bool All(const enum_type& value) const { return (static_cast<value_type>(this->m_Value) & static_cast<value_type>(value)) == static_cast<value_type>(value); }
        bool HasFlag(const enum_type& value) const { return static_cast<value_type>(this->m_Value) & static_cast<value_type>(value); }

        bool All(const value_type& value) const { return (static_cast<value_type>(this->m_Value) & value) == value; }
        bool Any(const value_type& value) const { return static_cast<value_type>(this->m_Value) & value; }
        bool HasFlag(const value_type& value) const { return static_cast<value_type>(this->m_Value) & value; }

        uint8 GetFlagCount() const { return static_cast<uint8>(std::bitset<sizeof(value_type) * 8>(static_cast<value_type>(this->m_Value)).count()); }

        static bool All(const enum_type& value, const enum_type& flag) { return (static_cast<value_type>(value) & static_cast<value_type>(flag)) == static_cast<value_type>(flag); }
        static bool All(const value_type& value, const value_type& flag) { return (value & flag) == flag; }

        static bool HasFlag(const enum_type& value, const enum_type& flag) { return static_cast<value_type>(value) & static_cast<value_type>(flag); }
        static bool HasFlag(const value_type& value, const value_type& flag) { return value & flag; }

        static uint8 GetFlagCount(const enum_type& value) { return static_cast<uint8>(std::bitset<sizeof(value_type) * 8>(static_cast<value_type>(value)).count()); }
        static uint8 GetFlagCount(const value_type& value) { return static_cast<uint8>(std::bitset<sizeof(value_type) * 8>(value).count()); }
    };

    template <typename enum_type, typename other_enum_type, typename value_type>
    struct ExternalFlagEnumeration : public FlagEnumeration<enum_type, value_type>
    {
        ExternalFlagEnumeration() = default;
        ExternalFlagEnumeration(const enum_type& value) : FlagEnumeration<enum_type, value_type>(value) {}
        ExternalFlagEnumeration(const other_enum_type& value) : FlagEnumeration<enum_type, value_type>(static_cast<value_type>(value)) {}
        ExternalFlagEnumeration(const value_type& value) : FlagEnumeration<enum_type, value_type>(value) {}
        ExternalFlagEnumeration(const FlagEnumeration<enum_type, value_type>& other) : FlagEnumeration<enum_type, value_type>(other) {}
        ExternalFlagEnumeration(FlagEnumeration<enum_type, value_type>&& other) : FlagEnumeration<enum_type, value_type>(std::move(other)) {}
        ~ExternalFlagEnumeration() = default;

        bool All(const enum_type& value) const { return (static_cast<value_type>(this->m_Value) & static_cast<value_type>(value)) == static_cast<value_type>(value); }
        bool HasFlag(const enum_type& value) const { return static_cast<value_type>(this->m_Value) & static_cast<value_type>(value); }

        static bool All(const enum_type& value, const enum_type& flag) { return (static_cast<value_type>(value) & static_cast<value_type>(flag)) == static_cast<value_type>(flag); }
        static bool HasFlag(const enum_type& value, const enum_type& flag) { return static_cast <value_type>(value) & static_cast<value_type>(flag); }
    };
} // namespace Savanna
#endif // end __cplusplus
