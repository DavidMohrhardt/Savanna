/**
 * @file AllocatorUtils.h
 * @author David Mohrhardt (david.mohrhardt@gmail.com)
 * @brief Provides allocator implementation utilities.
 * @version 0.1
 * @date 2022-08-02
 *
 * @copyright Copyright (c) 2022
 *
 */
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
        return (reinterpret_cast<uintptr>(ptr) & (alignment - 1u));
    }

    SAVANNA_NO_DISCARD inline void* GetForwardAlignedPtr(const void* const ptr, const size_t & alignment)
    {
        // As per https://en.wikipedia.org/wiki/Modulo_operation
        // For powers of 2 modulo can be implemented as x % 2n == x & (2n - 1)
        return reinterpret_cast<void*>(reinterpret_cast<uintptr>(ptr) + (reinterpret_cast<uintptr>(ptr) & (alignment - 1u)));
    }

    template<typename T>
    SAVANNA_NO_DISCARD inline T* AlignAndCastPtr(const T* const ptr, const size_t & alignment)
    {
        return static_cast<T*>(GetForwardAlignedPtr(reinterpret_cast<const void* const>(ptr), alignment));
    }

    SAVANNA_NO_DISCARD inline void* PointerAdd(const void* const ptr, const size_t & alignment)
    {
        // As per https://en.wikipedia.org/wiki/Modulo_operation
        // For powers of 2 modulo can be implemented as x % 2n == x & (2n - 1)
        return reinterpret_cast<void*>(reinterpret_cast<uintptr>(ptr) + (reinterpret_cast<uintptr>(ptr) & (alignment - 1u)));
    }
} // namespace Savanna

#else

#endif // end __cplusplus