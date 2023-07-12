#pragma once

#ifndef __EXPERIMENTAL_GEN_MACROS__
#define __EXPERIMENTAL_GEN_MACROS__

#define __GEN_COMMON_DEFAULT_CONSTRUCTOR_AND_DESTRUCTOR(__cppName) \
    __cppName() = default; \
    ~__cppName() = default;

#define __GEN_COMMON_COPY_CONSTRUCTORS(__cppName) \
    __cppName(const __cppName& other) = default; \
    __cppName& operator=(const __cppName& other) = default;

#define __GEN_COMMON_MOVE_CONSTRUCTORS(__cppName) \
    __cppName(__cppName&& other) = default; \
    __cppName& operator=(__cppName&& other) = default;

#define __GEN_COMMON_COPY_ASSIGNMENT_OPERATORS(__cppName) \
    __cppName& operator=(const __cppName& other) = default;

#define __GEN_COMMON_MOVE_ASSIGNMENT_OPERATORS(__cppName) \
    __cppName& operator=(__cppName&& other) = default;

#define __GEN_WRAPPER_TYPE_OPERATORS(__cppName, __cName, __dataMemberName) \
    operator __cName&() { return __dataMemberName; } \
    operator const __cName&() const { return __dataMemberName; } \
    operator __cName*() { return &__dataMemberName; } \
    operator const __cName*() const { return &__dataMemberName;

#define __GEN_WRAPPER_EQUALITY_OPERATORS(__cppName, __cName, __dataMemberName) \
    auto operator<=>(const __cppName& other) const { return __dataMemberName <=> other.__dataMemberName; } \
    auto operator<=>(const __cName& other) const { return __dataMemberName <=> other; }

#define __GEN_COMMON_DEFAULT_CONSTRUCTOR_AND_DESTRUCTOR(__cppName) \
    __cppName() = default; \
    ~__cppName() = default;

#undef __GEN_COMMON_COPY_CONSTRUCTORS
#define __GEN_COMMON_COPY_CONSTRUCTORS(__cppName) \
    __cppName(const __cppName& other) = default; \
    __cppName& operator=(const __cppName& other) = default;

#define __GEN_COMMON_MOVE_CONSTRUCTORS(__cppName) \
    __cppName(__cppName&& other) = default; \
    __cppName& operator=(__cppName&& other) = default;

#define __GEN_COMMON_COPY_ASSIGNMENT_OPERATORS(__cppName) \
    __cppName& operator=(const __cppName& other) = default;

#define __GEN_COMMON_MOVE_ASSIGNMENT_OPERATORS(__cppName) \
    __cppName& operator=(__cppName&& other) = default;

#define __GEN_WRAPPER_TYPE_OPERATORS(__cppName, __cName, __dataMemberName) \
    operator __cName&() { return __dataMemberName; } \
    operator const __cName&() const { return __dataMemberName; } \
    operator __cName*() { return &__dataMemberName; } \
    operator const __cName*() const { return &__dataMemberName;

#define __GEN_WRAPPER_EQUALITY_OPERATORS(__cppName, __cName, __dataMemberName) \
    auto operator<=>(const __cppName& other) const { return __dataMemberName <=> other.__dataMemberName; } \
    auto operator<=>(const __cName& other) const { return __dataMemberName <=> other; }

#define __GEN_WRAPPER_CLASS(__cName, __cppName) \
    class __cppName \
    { \
    public: \
        __cName m_Data; \
        __cppName(const __cName& data) : m_Data(data) {} \
        __cppName(__cName&& data) : m_Data(std::move(data)) {} \
        __GEN_COMMON_DEFAULT_CONSTRUCTOR_AND_DESTRUCTOR(__cppName) \
        __GEN_COMMON_COPY_CONSTRUCTORS(__cppName) \
        __GEN_COMMON_MOVE_CONSTRUCTORS(__cppName) \
        __GEN_COMMON_COPY_ASSIGNMENT_OPERATORS(__cppName) \
        __GEN_COMMON_MOVE_ASSIGNMENT_OPERATORS(__cppName) \
        __GEN_WRAPPER_TYPE_OPERATORS(__cppName, __cName, m_Data) \
        __GEN_WRAPPER_EQUALITY_OPERATORS(__cppName, __cName, m_Data) \
    };

#define IMPLEMENT_SAVANNA_NAMESPACED_CPP_CLASS_WRAPPER(__cName, __cppName) \
    namespace Savanna \
    { \
        __GEN_WRAPPER_CLASS(__cName, __cppName) \
    }

#define IMPLEMENT_SAVANNA_EXTENDED_NAMESPACED_CPP_CLASS_WRAPPER(__namespace, __cName, __cppName) \
    namespace Savanna::__namespace \
    { \
        __GEN_WRAPPER_CLASS(__cName, __cppName) \
    }
#endif // __EXPERIMENTAL_GEN_MACROS__
