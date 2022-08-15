/**
 * @file AllocatorUtils.h
 * @author David Mohrhardt (DavidMohrhardt@github.com)
 * @brief Provides allocator implementation utilities.
 * @version 0.1
 * @date 2022-08-02
 *
 * @copyright Copyright (c) 2022
 *
 */
#pragma once

#include "Math/MathHelpers.h"
#include "Math/PointerMath.h"
#include "Types/Primitive/PrimitiveTypes.h"
#include "Utilities/Macros/CppCommonDefs.h"

#if __cplusplus

namespace Savanna
{
    /**
     * @brief Aligns a pointer to the next nearest address with the given alignment.
     *
     * @param ptr
     * @param alignment
     * @return Difference between ptr and it's next nearest address with the given alignment
     */
    SAVANNA_NO_DISCARD inline size_t GetForwardAlignment(const void* const ptr, const size_t & alignment)
    {
        // As per https://en.wikipedia.org/wiki/Modulo_operation
        // For powers of 2 modulo can be implemented as x % 2n == x & (2n - 1)
        size_t moduloResult = (reinterpret_cast<uintptr>(ptr) & (alignment - 1u));
        return alignment - (moduloResult == 0 ? alignment : moduloResult);
    }

    template<typename T, typename U>
    SAVANNA_NO_DISCARD inline T* GetForwardAlignedPtr(U* const ptr, const size_t & alignment)
    {
        size_t alignmentOffset = GetForwardAlignment(static_cast<const void* const>(ptr), alignment);
        return reinterpret_cast<T*>(Add(ptr, alignmentOffset));
    }

} // namespace Savanna

#endif // end __cplusplus
