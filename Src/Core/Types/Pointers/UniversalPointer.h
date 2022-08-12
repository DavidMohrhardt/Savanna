/**
 * @file FloatingPointUnions.h
 * @author David Mohrhardt (david.mohrhardt@gmail.com)
 * @brief This is a file that is used to define unions of various common pointer types to make type casting simpler.
 * @version 0.1
 * @date 2022-08-10
 *
 * @copyright Copyright (c) 2022
 *
 */
#pragma once

#include "Types/Primitive/PrimitiveTypes.h"

#include "PointerUtilities.h"

#include "Utilities/Macros/CppTypeDefs.h"
#include "Utilities/Macros/CppCommonDefs.h"
#include "Utilities/Macros/CppAttributes.h"

/**
 * @brief TODO @DavidMohrhardt Document
 *
 */
typedef union __se_universalptr
{
    void* m_Ptr;
    __se_intptr m_IntPtr;
    __se_intptr m_UIntPtr;
    __se_byte m_Bytes[sizeof(__se_intptr)];
} __se_universalptr;

/**
 * @brief Simplify writing the universal pointer union.
 */
typedef __se_universalptr s_universalptr;

inline __se_universalptr Add(const __se_universalptr& ptr, const __se_intptr& offset)
{
    return (s_universalptr){ .m_UIntPtr = ptr.m_UIntPtr + offset };
}

inline __se_universalptr Sub(const __se_universalptr& ptr, const __se_intptr& offset)
{
    return (__se_universalptr){ .m_UIntPtr = ptr.m_UIntPtr - offset };
}

#if __cplusplus

#define DECLARE_UNIVERSAL_PTR_OPERATOR_OVERLOADS(__forType, __member) \
    auto operator<=>(const __forType& other) const \
    { \
        *this <=> (UniversalPointer)other; \
    } \
    void operator=(__forType& other) SAVANNA_NO_EXCEPT \
    { \
        m_UniversalPointer.__member = other; \
    } \

namespace Savanna
{
    struct UniversalPointer
    {
        s_universalptr m_UniversalPointer;

        UniversalPointer() : m_UniversalPointer{nullptr} {};

        UniversalPointer(const UniversalPointer& other) : m_UniversalPointer{other.m_UniversalPointer} {};
        UniversalPointer(UniversalPointer&& other) : m_UniversalPointer{other.m_UniversalPointer} {};

        UniversalPointer(const s_universalptr& other) { m_UniversalPointer = other; }
        UniversalPointer(s_universalptr&& other) { m_UniversalPointer = other; }

        UniversalPointer(void* other) { m_UniversalPointer.m_Ptr = other; }

        UniversalPointer(const __se_intptr& other) { m_UniversalPointer.m_IntPtr = other; }
        UniversalPointer(__se_intptr&& other) { m_UniversalPointer.m_IntPtr = other; }

        UniversalPointer(const __se_uintptr& other) { m_UniversalPointer.m_UIntPtr = other; }
        UniversalPointer(__se_uintptr&& other) { m_UniversalPointer.m_UIntPtr = other; }

        ~UniversalPointer() = default;

        auto operator<=>(UniversalPointer const& rhs) const
        {
            if (m_UniversalPointer.m_IntPtr < rhs.m_UniversalPointer.m_IntPtr)
            {
                return -1;
            }
            else if (m_UniversalPointer.m_IntPtr > rhs.m_UniversalPointer.m_IntPtr)
            {
                return 1;
            }
            else
            {
                return 0;
            }
        }

        auto operator<=>(void* const other) const SAVANNA_NO_EXCEPT
        {
            return *this <=> UniversalPointer(other);
        }

        void operator=(void* other) SAVANNA_NO_EXCEPT
        {
            m_UniversalPointer.m_Ptr = other;
        }

        DECLARE_UNIVERSAL_PTR_OPERATOR_OVERLOADS(intptr, m_IntPtr);
        DECLARE_UNIVERSAL_PTR_OPERATOR_OVERLOADS(uintptr, m_UIntPtr);
    };

    /**
     * @brief A handle is a pointer unioned with a UniversalPointer. This allows the handle to strongly typed is compatible with the UniversalPointer.
     *
     * @tparam T The type of the handle.
     */
    template <typename T>
    struct TypeHandle
    {
        union
        {
            T* m_DataPtr;
            UniversalPointer m_UniversalPtr;
        };

        TypeHandle() : m_UniversalPtr{nullptr} {};

        TypeHandle(const TypeHandle& other) : m_UniversalPtr{other.m_UniversalPtr} {};
        TypeHandle(TypeHandle&& other) : m_UniversalPtr{other.m_UniversalPtr} {};

        TypeHandle(T* other) { m_DataPtr = other; }

        // Delete the following explicitly to ensure type safety.

        TypeHandle(const UniversalPointer& other) = delete;
        TypeHandle(UniversalPointer&& other) = delete;

        TypeHandle(const __se_intptr& other) = delete;
        TypeHandle(__se_intptr&& other) = delete;

        TypeHandle(const __se_uintptr& other) = delete;
        TypeHandle(__se_uintptr&& other) = delete;

        ~TypeHandle() = default;

        operator T*() const { return m_DataPtr; }

        operator UniversalPointer() const { return m_UniversalPtr; }
        operator UniversalPointer&() { return m_UniversalPtr; }
        operator UniversalPointer&&() { return std::move(m_UniversalPtr); }

        auto operator<=>(TypeHandle const& rhs) const
        {
            return m_UniversalPtr <=> rhs.m_UniversalPtr;
        }
    };
} // namespace Savanna

#undef DECLARE_UNIVERSAL_PTR_OPERATOR_OVERLOADS

#endif // end __cplusplus
