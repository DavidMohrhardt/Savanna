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

#define DECLARE_NAMESPACED_CPP_FLAG_ENUMERATION(__cppName, __cName, __value_type)
#define DECLARE_NAMESPACED_EXTERNAL_CPP_FLAG_ENUMERATION(__nameSpace, __cppName, __cName, __external_enum, __value_type)

#define DEFINE_C_FLAG_ENUMERATION(__cName, __value_type, ...) \
    typedef enum __cName : __value_type { __VA_ARGS__ } __cName;

#if defined(__cplusplus)

#include <bitset>
#include <concepts>
#include <type_traits>

#undef DECLARE_NAMESPACED_CPP_FLAG_ENUMERATION
#define DECLARE_NAMESPACED_CPP_FLAG_ENUMERATION(__cppName, __cName, __value_type) \
    namespace __nameSpace { using __cppName = Savanna::FlagEnumeration<__cName, __value_type>; }

#undef DECLARE_NAMESPACED_EXTERNAL_CPP_FLAG_ENUMERATION
#define DECLARE_NAMESPACED_EXTERNAL_CPP_FLAG_ENUMERATION(__nameSpace, __cppName, __cName, __external_enum, __value_type) \
    namespace __nameSpace { using __cppName = Savanna::ExternalFlagEnumeration<__cName, __external_enum, __value_type>; }

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


#define DEFINE_SAVANNA_FLAG_ENUMERATION(__cppName, __cName, __value_type, ...) \
    DEFINE_C_FLAG_ENUMERATION(__cName, __value_type, __VA_ARGS__) \
    DECLARE_NAMESPACED_CPP_FLAG_ENUMERATION(__cppName, __cName, __value_type)

#define DEFINE_SAVANNA_EXTERNAL_FLAG_ENUMERATION(__nameSpace, __cppName, __cName, __external_enum, __value_type, ...) \
    DEFINE_C_FLAG_ENUMERATION(__cName, __value_type, __VA_ARGS__) \
    DECLARE_NAMESPACED_EXTERNAL_CPP_FLAG_ENUMERATION(__nameSpace, __cppName, __cName, __external_enum, __value_type)
