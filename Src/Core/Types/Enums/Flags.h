#pragma once

#include "Enumeration.h"

#if defined(__cplusplus)

#define DECLARE_NAMESPACED_FLAGS(__nameSpace, __cName, __cppName, __type, ...) \
    typedef enum __cName : __type { __VA_ARGS__ } __cName; \
    namespace __nameSpace { using __cppName = Savanna::FlagEnumeration<__cName, __type>; }

#else

#define DECLARE_NAMESPACED_FLAGS(__cName, __cppName, __type, ...) \
    DECLARE_NAMESPACED_ENUMERATION(__cName, __cppName, __type, __VA_ARGS__)

#endif // end __cplusplus

#define DECLARE_SAVANNA_FLAGS(__cName, __cppName, __type, ...) \
    DECLARE_NAMESPACED_FLAGS(Savanna, __cName, __cppName, __type, __VA_ARGS__)

#define DECLARE_SAVANNA_MODULE_FLAGS(__moduleNamespace, __cName, __cppName, __type, ...) \
    DECLARE_NAMESPACED_FLAGS(Savanna::__moduleNamespace, __cName, __cppName, __type, __VA_ARGS__)

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
        bool Any(const T& value) const { return static_cast<U>(this->m_Value) & static_cast<U>(value); }
        bool All(const T& value) const { return (static_cast<U>(this->m_Value) & static_cast<U>(value)) == static_cast<U>(value); }

        static bool HasFlag(const T& value, const T& flag) { return static_cast<U>(value) & static_cast<U>(flag); }
        static bool Any(const T& value, const T& flag) { return static_cast<U>(value) & static_cast<U>(flag); }
        static bool All(const T& value, const T& flag) { return (static_cast<U>(value) & static_cast<U>(flag)) == static_cast<U>(flag); }
    };
} // namespace Savanna
