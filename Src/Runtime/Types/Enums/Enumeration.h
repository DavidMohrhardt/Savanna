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

#include "Public/ISavannaEngine.hpp"

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
    /**
     * @brief This concept checks if the given type is an enum.
     *
     * @tparam enum_type The type to check
     */
    template <typename enum_type>
    concept EnumReq = std::is_enum_v<enum_type>;

    /**
     * @brief This concept checks if the given type is a strong enum.
     *
     * @tparam enum_type The type to check
     */
    template <typename enum_type>
    concept StrongEnumReq = std::is_enum_v<enum_type> && std::is_class_v<enum_type>;

    /**
     * @brief This concept checks if the given type is an enum with an integral backing type.
     *
     * @tparam enum_type The type to check
     */
    template <typename enum_type>
    concept EnumBackingTypeReq = std::is_integral_v<enum_type>;// || std::is_enum_v<enum_type>;

    /**
     * @brief This struct defines a templated Enumeration wrapper type that can be used to wrap C enumerations
     * and provide a more type-safe interface to them. It implements all of the operators that are available
     * for enumerations in C++.
     *
     * @tparam enum_type The C enumeration type to wrap.
     * @tparam value_type The value type to use for the backing value. IE uint32_t
     */
    template <typename enum_type, typename value_type>
    requires EnumReq<enum_type> && EnumBackingTypeReq<value_type>
    struct Enumeration
    {
        // Insists that the backing type is trivially convertible to the enum type
        static_assert(std::is_convertible<enum_type, value_type>());

        // Insists that the backing type is the same size as the enum type
        static_assert(sizeof(enum_type) == sizeof(value_type));

        /**
         * @brief Default constructor. Initializes the backing value to 0.
         *
         */
        constexpr Enumeration() = default;

        /**
         * @brief Construct a new Enumeration object from the given enum value.
         *
         * @param value The enum value to use.
         */
        constexpr Enumeration(const enum_type& value) : m_Value(value) {}

        /**
         * @brief Construct a new Enumeration object from the given value.
         *
         * @param value The value to use of the backing type.
         */
        constexpr Enumeration(const value_type& value) : m_Value(static_cast<enum_type>(value)) {}

        /**
         * @brief Construct a new Enumeration object from the given Enumeration object.
         *
         * @param other The Enumeration object to copy.
         */
        constexpr Enumeration(const Enumeration& other) : m_Value(other.m_Value) {}

        /**
         * @brief Construct a new Enumeration object from the given Enumeration object.
         *
         * @param other The Enumeration object to move.
         */
        constexpr Enumeration(Enumeration&& other) : m_Value(std::move(other.m_Value)) {}

        /**
         * @brief Destroy the Enumeration object. This is trivial.
         *
         */
        ~Enumeration() = default;

        /**
         * @brief Copy assignment operator. Copies the value from the given Enumeration object.
         *
         * @param value The Enumeration object to copy.
         * @return constexpr Enumeration& A reference to this object.
         */
        constexpr Enumeration& operator=(const enum_type& value)
        {
            m_Value = value;
            return *this;
        }

        /**
         * @brief Copy assignment operator. Copies the value from the given Enumeration object.
         *
         * @param value The Enumeration object to copy.
         * @return constexpr Enumeration& A reference to this object.
         */
        constexpr Enumeration& operator=(const value_type& value)
        {
            m_Value = static_cast<enum_type>(value);
            return *this;
        }

        /**
         * @brief Copy assignment operator. Copies the value from the given Enumeration object.
         *
         * @param other The Enumeration object to copy.
         * @return constexpr Enumeration& A reference to this object.
         */
        constexpr Enumeration& operator=(const Enumeration& other)
        {
            m_Value = other.m_Value;
            return *this;
        }

        /**
         * @brief Move assignment operator. Moves the value from the given Enumeration object.
         *
         * @param other The Enumeration object to move.
         * @return constexpr Enumeration& A reference to this object.
         */
        constexpr Enumeration& operator=(Enumeration&& other)
        {
            m_Value = std::move(other.m_Value);
            return *this;
        }

        // Conversion operators
        /**
         * @brief The conversion operator to the enum type.
         *
         * @return enum_type The templated C enum type.
         */
        constexpr operator enum_type() const { return m_Value; }

        /**
         * @brief The conversion operator to the value type.
         *
         * @return value_type The templated backing value type.
         */
        constexpr operator value_type() const { return static_cast<value_type>(m_Value); }

        // Comparison operators
        /**
         * @brief Spaceship operator for comparing the enum type.
         *
         * @param value The enum value to compare against.
         * @return constexpr auto The result of the comparison.
         */
        constexpr auto operator<=>(const enum_type& value) const { return m_Value <=> value; }

        /**
         * @brief Spaceship operator for comparing the value type.
         *
         * @param value The value to compare against.
         * @return constexpr auto The result of the comparison.
         */
        constexpr auto operator<=>(const value_type& value) const { return static_cast<value_type>(m_Value) <=> value; }

        // Bitwise operators
        /**
         * @brief Bitwise AND operator for the enum type.
         *
         * @param value The enum value to AND against.
         * @return constexpr Enumeration The result of the AND operation.
         */
        constexpr Enumeration operator&(const enum_type& value) const { return static_cast<value_type>(m_Value) & static_cast<value_type>(value); }

        /**
         * @brief Bitwise AND operator for the value type.
         *
         * @param value The value to AND against.
         * @return constexpr Enumeration The result of the AND operation.
         */
        constexpr Enumeration operator&(const value_type& value) const { return static_cast<value_type>(m_Value) & value; }

        /**
         * @brief Bitwise OR operator for the enum type.
         *
         * @param value The enum value to OR against.
         * @return constexpr Enumeration The result of the OR operation.
         */
        constexpr Enumeration operator|(const enum_type& value) const { return static_cast<value_type>(m_Value) | static_cast<value_type>(value); }

        /**
         * @brief Bitwise OR operator for the value type.
         *
         * @param value The value to OR against.
         * @return constexpr Enumeration The result of the OR operation.
         */
        constexpr Enumeration operator|(const value_type& value) const { return static_cast<value_type>(m_Value) | value; }

        /**
         * @brief Bitwise XOR operator for the enum type.
         *
         * @param value The enum value to XOR against.
         * @return constexpr Enumeration The result of the XOR operation.
         */
        constexpr Enumeration operator^(const enum_type& value) const { return static_cast<value_type>(m_Value) ^ static_cast<value_type>(value); }

        /**
         * @brief Bitwise XOR operator for the value type.
         *
         * @param value The value to XOR against.
         * @return constexpr Enumeration The result of the XOR operation.
         */
        constexpr Enumeration operator^(const value_type& value) const { return static_cast<value_type>(m_Value) ^ value; }

        /**
         * @brief Bitwise NOT operator for the enum type.
         *
         * @return constexpr Enumeration The result of the NOT operation.
         */
        constexpr Enumeration operator~() const { return ~static_cast<value_type>(m_Value); }

        /**
         * @brief Bitwise left shift operator for the enum type.
         *
         * @param value The enum value to shift by.
         * @return constexpr Enumeration The result of the left shift operation.
         */
        constexpr Enumeration operator<<(const enum_type& value) const { return static_cast<value_type>(m_Value) << static_cast<value_type>(value); }

        /**
         * @brief Bitwise left shift operator for the value type.
         *
         * @param value The value to shift by.
         * @return constexpr Enumeration The result of the left shift operation.
         */
        constexpr Enumeration operator<<(const value_type& value) const { return static_cast<value_type>(m_Value) << value; }

        /**
         * @brief Bitwise right shift operator for the enum type.
         *
         * @param value The enum value to shift by.
         * @return constexpr Enumeration The result of the right shift operation.
         */
        constexpr Enumeration operator>>(const enum_type& value) const { return static_cast<value_type>(m_Value) >> static_cast<value_type>(value); }

        /**
         * @brief Bitwise right shift operator for the value type.
         *
         * @param value The value to shift by.
         * @return constexpr Enumeration The result of the right shift operation.
         */
        constexpr Enumeration operator>>(const value_type& value) const { return static_cast<value_type>(m_Value) >> value; }

        // Compound assignment operators
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

        static_assert(sizeof(enum_type) == sizeof(value_type), "FlagEnumeration size mismatch");
    };
} // namespace Savanna
#endif // defined(__cplusplus)

#define DEFINE_SAVANNA_ENUM(__nameSpace, __cName, __cppName, __type, ...) \
    DECLARE_NAMESPACED_ENUMERATION(__nameSpace, __cName, __cppName, __type, __VA_ARGS__)
