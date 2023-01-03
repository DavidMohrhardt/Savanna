#pragma once

#include "Enumeration.h"

#if defined(__cplusplus)

#include <bitset>

#define DECLARE_NAMESPACED_FLAGS_WRAPPER(__nameSpace, __cName, __cppName, __enumType, __type) \
    namespace __nameSpace { using __cppName = Savanna::FlagWrapperEnumeration<__cName, __enumType, __type>; }

#define DECLARE_NAMESPACED_FLAGS(__nameSpace, __cName, __cppName, __type, ...) \
    typedef enum __cName : __type { __VA_ARGS__ } __cName; \
    DECLARE_NAMESPACED_FLAGS_WRAPPER(__nameSpace, __cName, __cppName, __type)

namespace Savanna
{
    template <typename T, typename U>
    struct FlagEnumeration : public Enumeration<T, U>
    {
        FlagEnumeration() = default;
        FlagEnumeration(const T& value) : Enumeration<T, U>(value) {}
        FlagEnumeration(const U& value) : Enumeration<T, U>(value) {}
        FlagEnumeration(const Enumeration<T, U>& other) : Enumeration<T, U>(other) {}
        FlagEnumeration(Enumeration<T, U>&& other) : Enumeration<T, U>(std::move(other)) {}
        ~FlagEnumeration() = default;

        bool HasFlag(const T& value) const { return static_cast<U>(this->m_Value) & static_cast<U>(value); }
        bool HasFlag(const U& value) const { return static_cast<U>(this->m_Value) & value; }
        bool Any(const T& value) const { return static_cast<U>(this->m_Value) & static_cast<U>(value); }
        bool Any(const U& value) const { return static_cast<U>(this->m_Value) & value; }
        bool All(const T& value) const { return (static_cast<U>(this->m_Value) & static_cast<U>(value)) == static_cast<U>(value); }
        bool All(const U& value) const { return (static_cast<U>(this->m_Value) & value) == value; }

        uint8 GetFlagCount() const { return static_cast<uint8>(std::bitset<sizeof(U) * 8>(static_cast<U>(this->m_Value)).count()); }

        static bool HasFlag(const T& value, const T& flag) { return static_cast<U>(value) & static_cast<U>(flag); }
        static bool Any(const T& value, const T& flag) { return static_cast<U>(value) & static_cast<U>(flag); }
        static bool All(const T& value, const T& flag) { return (static_cast<U>(value) & static_cast<U>(flag)) == static_cast<U>(flag); }
    };

    template <typename T, typename U, typename V>
    concept FlagWrapperEnumReq = EnumReq<T> && EnumBackingTypeReq<U> && std::is_convertible_v<T, V> && std::is_convertible_v<U, V> && std::is_integral_v<V>;

    template <typename T, typename U, typename V>
    requires FlagWrapperEnumReq<T, U, V>
    struct FlagWrapperEnumeration : public FlagEnumeration<U, V>
    {
        // Wraps an enumeration as a flag enumeration and uses a backing type of V.
        FlagWrapperEnumeration() = default;
        FlagWrapperEnumeration(const T& value) : FlagEnumeration<U, V>(static_cast<V>(value)) {}
        FlagWrapperEnumeration(const U& value) : FlagEnumeration<U, V>(value) {}
        FlagWrapperEnumeration(const V& value) : FlagEnumeration<U, V>(value) {}
        FlagWrapperEnumeration(const FlagEnumeration<U, V>& other) : FlagEnumeration<U, V>(other) {}
        FlagWrapperEnumeration(FlagEnumeration<U, V>&& other) : FlagEnumeration<U, V>(std::move(other)) {}
        ~FlagWrapperEnumeration() = default;

        bool HasFlag(const T& value) const { return static_cast<V>(this->m_Value) & static_cast<V>(value); }
        bool Any(const T& value) const { return static_cast<V>(this->m_Value) & static_cast<V>(value); }
        bool All(const T& value) const { return (static_cast<V>(this->m_Value) & static_cast<V>(value)) == static_cast<V>(value); }

        static bool HasFlag(const T& value, const T& flag) { return static_cast<V>(value) & static_cast<V>(flag); }
        static bool Any(const T& value, const T& flag) { return static_cast<V>(value) & static_cast<V>(flag); }
        static bool All(const T& value, const T& flag) { return (static_cast<V>(value) & static_cast<V>(flag)) == static_cast<V>(flag); }

        FlagWrapperEnumeration& operator=(const T& value) { this->m_Value = static_cast<U>(value); return *this; }
        FlagWrapperEnumeration& operator=(const U& value) { this->m_Value = value; return *this; }
        FlagWrapperEnumeration& operator=(const FlagEnumeration<U, V>& other) { this->m_Value = other.m_Value; return *this; }
        FlagWrapperEnumeration& operator=(FlagEnumeration<U, V>&& other) { this->m_Value = std::move(other.m_Value); return *this; }
    };
} // namespace Savanna

#else

#define DECLARE_NAMESPACED_FLAGS_WRAPPER(__nameSpace, __cName, __cppName, __type)

#define DECLARE_NAMESPACED_FLAGS(__cName, __cppName, __type, ...) \
    DECLARE_NAMESPACED_ENUMERATION(__cName, __cppName, __type, __VA_ARGS__)

#endif // end __cplusplus

#define DECLARE_SAVANNA_FLAGS(__cName, __cppName, __type, ...) \
    DECLARE_NAMESPACED_FLAGS(Savanna, __cName, __cppName, __type, __VA_ARGS__)

#define DECLARE_SAVANNA_MODULE_FLAGS(__moduleNamespace, __cName, __cppName, __type, ...) \
    DECLARE_NAMESPACED_FLAGS(Savanna::__moduleNamespace, __cName, __cppName, __type, __VA_ARGS__)
