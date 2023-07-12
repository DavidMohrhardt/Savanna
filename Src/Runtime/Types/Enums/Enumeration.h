/**
 * @file Enumeration.h
 * @author David Mohrhardt (https://github.com/DavidMohrhardt/Savanna)
 * @brief
 * @version 0.1
 * @date 2023-01-08
 *
 * @copyright Copyright (c) 2023
 *
 */
#pragma once

#if defined(__cplusplus)

#include <concepts>
#include <type_traits>

#define DECLARE_NAMESPACED_ENUM_WRAPPER(__nameSpace, __cName, __cppName, __type) \
    namespace __nameSpace { using __cppName = Savanna::Enumeration<__cName, __type>; }

#define DECLARE_NAMESPACED_ENUMERATION(__nameSpace, __cName, __cppName, __type, ...) \
    typedef enum __cName : __type { __VA_ARGS__ } __cName; \
    DECLARE_NAMESPACED_ENUM_WRAPPER(__nameSpace, __cName, __cppName, __type)

#else

#define DECLARE_NAMESPACED_ENUM_WRAPPER(__nameSpace, __cName, __cppName, __type)

#define DECLARE_NAMESPACED_ENUMERATION(__cName, __cppName, __type, ...) \
    typedef enum __cName : __type { __VA_ARGS__ } __cName;

#endif // end __cplusplus

#if defined(__cplusplus)
namespace Savanna
{
    template <typename enum_type>
    concept EnumReq = std::is_enum_v<enum_type>;

    template <typename enum_type>
    concept StrongEnumReq = std::is_enum_v<enum_type> && std::is_class_v<enum_type>;

    template <typename enum_type>
    concept EnumBackingTypeReq = std::is_integral_v<enum_type>;// || std::is_enum_v<enum_type>;

    template <typename enum_type, typename value_type>
    requires EnumReq<enum_type> && EnumBackingTypeReq<value_type>
    struct Enumeration
    {
        static_assert(std::is_convertible<enum_type, value_type>());

        constexpr Enumeration() = default;
        constexpr Enumeration(const enum_type& value) : m_Value(value) {}
        constexpr Enumeration(const value_type& value) : m_Value(static_cast<enum_type>(value)) {}
        constexpr Enumeration(const Enumeration& other) : m_Value(other.m_Value) {}
        constexpr Enumeration(Enumeration&& other) : m_Value(std::move(other.m_Value)) {}
        ~Enumeration() = default;

        constexpr Enumeration& operator=(const enum_type& value)
        {
            m_Value = value;
            return *this;
        }

        constexpr Enumeration& operator=(const value_type& value)
        {
            m_Value = static_cast<enum_type>(value);
            return *this;
        }

        constexpr Enumeration& operator=(const Enumeration& other)
        {
            m_Value = other.m_Value;
            return *this;
        }

        constexpr Enumeration& operator=(Enumeration&& other)
        {
            m_Value = std::move(other.m_Value);
            return *this;
        }

        // Conversion operators
        constexpr operator enum_type() const { return m_Value; }
        constexpr operator value_type() const { return static_cast<value_type>(m_Value); }

        // Comparison operators
        constexpr auto operator<=>(const enum_type& value) const { return m_Value <=> value; }
        constexpr auto operator<=>(const value_type& value) const { return static_cast<value_type>(m_Value) <=> value; }

        // Bitwise operators
        constexpr Enumeration operator&(const enum_type& value) const { return static_cast<value_type>(m_Value) & static_cast<value_type>(value); }
        constexpr Enumeration operator&(const value_type& value) const { return static_cast<value_type>(m_Value) & value; }
        constexpr Enumeration operator|(const enum_type& value) const { return static_cast<value_type>(m_Value) | static_cast<value_type>(value); }
        constexpr Enumeration operator|(const value_type& value) const { return static_cast<value_type>(m_Value) | value; }
        constexpr Enumeration operator^(const enum_type& value) const { return static_cast<value_type>(m_Value) ^ static_cast<value_type>(value); }
        constexpr Enumeration operator^(const value_type& value) const { return static_cast<value_type>(m_Value) ^ value; }
        constexpr Enumeration operator~() const { return ~static_cast<value_type>(m_Value); }
        constexpr Enumeration operator<<(const enum_type& value) const { return static_cast<value_type>(m_Value) << static_cast<value_type>(value); }
        constexpr Enumeration operator<<(const value_type& value) const { return static_cast<value_type>(m_Value) << value; }
        constexpr Enumeration operator>>(const enum_type& value) const { return static_cast<value_type>(m_Value) >> static_cast<value_type>(value); }
        constexpr Enumeration operator>>(const value_type& value) const { return static_cast<value_type>(m_Value) >> value; }
        constexpr Enumeration& operator&=(const enum_type& value) { m_Value = static_cast<enum_type>(static_cast<value_type>(m_Value) & static_cast<value_type>(value)); return *this; }
        constexpr Enumeration& operator&=(const value_type& value) { m_Value = static_cast<enum_type>(static_cast<value_type>(m_Value) & value); return *this; }
        constexpr Enumeration& operator|=(const enum_type& value) { m_Value = static_cast<enum_type>(static_cast<value_type>(m_Value) | static_cast<value_type>(value)); return *this; }
        constexpr Enumeration& operator|=(const value_type& value) { m_Value = static_cast<enum_type>(static_cast<value_type>(m_Value) | value); return *this; }
        constexpr Enumeration& operator^=(const enum_type& value) { m_Value = static_cast<enum_type>(static_cast<value_type>(m_Value) ^ static_cast<value_type>(value)); return *this; }
        constexpr Enumeration& operator^=(const value_type& value) { m_Value = static_cast<enum_type>(static_cast<value_type>(m_Value) ^ value); return *this; }
        constexpr Enumeration& operator<<=(const enum_type& value) { m_Value = static_cast<enum_type>(static_cast<value_type>(m_Value) << static_cast<value_type>(value)); return *this; }
        constexpr Enumeration& operator<<=(const value_type& value) { m_Value = static_cast<enum_type>(static_cast<value_type>(m_Value) << value); return *this; }
        constexpr Enumeration& operator>>=(const enum_type& value) { m_Value = static_cast<enum_type>(static_cast<value_type>(m_Value) >> static_cast<value_type>(value)); return *this; }
        constexpr Enumeration& operator>>=(const value_type& value) { m_Value = static_cast<enum_type>(static_cast<value_type>(m_Value) >> value); return *this; }

        union
        {
            enum_type m_Value;
            value_type m_BackingValue;
        };
    };
} // namespace Savanna
#endif // defined(__cplusplus)

#define DEFINE_SAVANNA_ENUM(__nameSpace, __cName, __cppName, __type, ...) \
    DECLARE_NAMESPACED_ENUMERATION(__nameSpace, __cName, __cppName, __type, __VA_ARGS__)
