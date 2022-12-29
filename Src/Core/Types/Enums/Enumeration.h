#pragma once

#if defined(__cplusplus)
#include <concepts>
#include <type_traits>
#endif

#if defined(__cplusplus)

#define DECLARE_NAMESPACED_ENUMERATION(__nameSpace, __cName, __cppName, __type, ...) \
    typedef enum __cName : __type { __VA_ARGS__ } __cName; \
    namespace __nameSpace { using __cppName = Savanna::Enumeration<__cName, __type>; }

#else

#define DECLARE_NAMESPACED_ENUMERATION(__cName, __cppName, __type, ...) \
    typedef enum __cName : __type { __VA_ARGS__ } __cName;

#endif // end __cplusplus

#define DECLARE_SAVANNA_ENUM(__cName, __cppName, __type, ...) \
    DECLARE_NAMESPACED_ENUMERATION(Savanna, __cName, __cppName, __type, __VA_ARGS__)

#define DECLARE_SAVANNA_MODULE_ENUM(__moduleNamespace, __cName, __cppName, __type, ...) \
    DECLARE_NAMESPACED_ENUMERATION(Savanna::__moduleNamespace, __cName, __cppName, __type, __VA_ARGS__)

#if defined(__cplusplus)
namespace Savanna
{
    template <typename T>
    concept Enum = std::is_enum_v<T>;

    template <typename T>
    concept StrongEnum = std::is_enum_v<T> && std::is_class_v<T>;

    template <typename T>
    concept EnumBackingType = std::is_integral_v<T>;

    template <typename T, typename U>
    requires Enum<T> && EnumBackingType<U>
    struct Enumeration
    {
        static_assert(std::is_convertible<T, U>());

        using Enum = T;
        using BackingType = U;

        constexpr Enumeration() = default;
        constexpr Enumeration(const Enum& value) : m_Value(value) {}
        constexpr Enumeration(const BackingType& value) : m_Value(static_cast<Enum>(value)) {}
        constexpr Enumeration(const Enumeration& other) : m_Value(other.m_Value) {}
        constexpr Enumeration(Enumeration&& other) : m_Value(std::move(other.m_Value)) {}
        ~Enumeration() = default;

        constexpr Enumeration& operator=(const Enum& value)
        {
            m_Value = value;
            return *this;
        }

        constexpr Enumeration& operator=(const BackingType& value)
        {
            m_Value = static_cast<Enum>(value);
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
        constexpr operator Enum() const { return m_Value; }
        constexpr operator BackingType() const { return static_cast<BackingType>(m_Value); }

        // Comparison operators
        constexpr auto operator<=>(const Enum& value) const { return m_Value <=> value; }
        constexpr auto operator<=>(const BackingType& value) const { return static_cast<BackingType>(m_Value) <=> value; }

        // Bitwise operators
        constexpr Enumeration operator&(const Enum& value) const { return static_cast<BackingType>(m_Value) & static_cast<BackingType>(value); }
        constexpr Enumeration operator&(const BackingType& value) const { return static_cast<BackingType>(m_Value) & value; }
        constexpr Enumeration operator|(const Enum& value) const { return static_cast<BackingType>(m_Value) | static_cast<BackingType>(value); }
        constexpr Enumeration operator|(const BackingType& value) const { return static_cast<BackingType>(m_Value) | value; }
        constexpr Enumeration operator^(const Enum& value) const { return static_cast<BackingType>(m_Value) ^ static_cast<BackingType>(value); }
        constexpr Enumeration operator^(const BackingType& value) const { return static_cast<BackingType>(m_Value) ^ value; }
        constexpr Enumeration operator~() const { return ~static_cast<BackingType>(m_Value); }
        constexpr Enumeration operator<<(const Enum& value) const { return static_cast<BackingType>(m_Value) << static_cast<BackingType>(value); }
        constexpr Enumeration operator<<(const BackingType& value) const { return static_cast<BackingType>(m_Value) << value; }
        constexpr Enumeration operator>>(const Enum& value) const { return static_cast<BackingType>(m_Value) >> static_cast<BackingType>(value); }
        constexpr Enumeration operator>>(const BackingType& value) const { return static_cast<BackingType>(m_Value) >> value; }
        constexpr Enumeration& operator&=(const Enum& value) { m_Value = static_cast<Enum>(static_cast<BackingType>(m_Value) & static_cast<BackingType>(value)); return *this; }
        constexpr Enumeration& operator&=(const BackingType& value) { m_Value = static_cast<Enum>(static_cast<BackingType>(m_Value) & value); return *this; }
        constexpr Enumeration& operator|=(const Enum& value) { m_Value = static_cast<Enum>(static_cast<BackingType>(m_Value) | static_cast<BackingType>(value)); return *this; }
        constexpr Enumeration& operator|=(const BackingType& value) { m_Value = static_cast<Enum>(static_cast<BackingType>(m_Value) | value); return *this; }
        constexpr Enumeration& operator^=(const Enum& value) { m_Value = static_cast<Enum>(static_cast<BackingType>(m_Value) ^ static_cast<BackingType>(value)); return *this; }
        constexpr Enumeration& operator^=(const BackingType& value) { m_Value = static_cast<Enum>(static_cast<BackingType>(m_Value) ^ value); return *this; }
        constexpr Enumeration& operator<<=(const Enum& value) { m_Value = static_cast<Enum>(static_cast<BackingType>(m_Value) << static_cast<BackingType>(value)); return *this; }
        constexpr Enumeration& operator<<=(const BackingType& value) { m_Value = static_cast<Enum>(static_cast<BackingType>(m_Value) << value); return *this; }
        constexpr Enumeration& operator>>=(const Enum& value) { m_Value = static_cast<Enum>(static_cast<BackingType>(m_Value) >> static_cast<BackingType>(value)); return *this; }
        constexpr Enumeration& operator>>=(const BackingType& value) { m_Value = static_cast<Enum>(static_cast<BackingType>(m_Value) >> value); return *this; }

        union
        {
            Enum m_Value;
            BackingType m_BackingValue;
        };
    };
} // namespace Savanna
#endif
