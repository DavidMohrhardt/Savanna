#pragma once

#include "Types/Primitive/PrimitiveTypes.h"

#include "Utilities/Macros/CppTypeDefs.h"

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
DECLARE_SAVANNA_NAMESPACED_CPP_TYPE_DEF(SavannaAllocatorKind, AllocatorKind);
