/**
 * @file AllocatorKind.h
 * @author David Mohrhardt (https://github.com/DavidMohrhardt/Savanna)
 * @brief TODO @DavidMohrhardt Document
 * @version 0.1
 * @date 2022-08-02
 *
 */

#pragma once

#include "Types/Primitive/PrimitiveTypes.h"

#include "Utilities/Macros/CppTypeDefs.h"

/**
 * @brief TODO @DavidMohrhardt Document
 *
 */
enum SavannaAllocatorKind : __se_int32
{
    /**
     * @brief Represents Core Engine allocators.
     */
    Core,

    /**
     * @brief Represents allocations made from scripts.
     */
    Scripting,

    /**
     * @brief Represents long lived memory allocators.
     */
    Persistent,

    /**
     * @brief Value indicates that a temporary allocator is used. Memory allocated via this allocator
     * is only available until the end of a frame.
     */
    Temporary,

    /**
     * @brief Value indicates that a thread-safe temporary allocator will be used. Thread safe temporary
     * allocations are longer lived than typical temporary allocations.
     */
    ThreadTemp,

    /**
     * @brief Enum representing system owned memory. Allows users to use the malloc and free functions.
     */
    System,
};

/**
 * @brief If using C++, provides a simple "Savanna" Namespaced type definition for the SavannaAllocatorKind: AllocatorKind
 */
DECLARE_SAVANNA_NAMESPACED_CPP_TYPE_DEF(SavannaAllocatorKind, AllocatorKind);
