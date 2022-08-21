/**
 * @file CacheLine.h
 * @author David Mohrhardt (https://github.com/DavidMohrhardt/Savanna)
 * @brief TODO @DavidMohrhardt Document
 * @version 0.1
 * @date 2022-08-02
 *
 */

#pragma once

#if __cplusplus

#include <new>

#endif

#include "Math/MathHelpers.h"
#include "Utilities/SavannaCoding.h"

// Constants/Constexpr's
#if __cplusplus && __cpp_lib_hardware_interference_size
    static constexpr size_t __se_hardware_constructive_interference_size
        = std::hardware_constructive_interference_size;
    static constexpr size_t __se_hardware_destructive_interference_size
        = std::hardware_destructive_interference_size;
#else // !__cplusplus || !__cpp_lib_hardware_interference_size
    // 64 bytes on x86_64 | L1_CACHE_BYTES | L1_CACHE_SHIFT | __cacheline_aligned | ...
    static constexpr size_t __se_hardware_constructive_interference_size = 64;
    static constexpr size_t __se_hardware_destructive_interference_size = 64;
#endif // end __cplusplus

/**
 * @brief Estimates the size of a cacheline at compile time.
 *
 * @return constexpr size_t
 * The estimated size of a cacheline at compile time.
 */
constexpr size_t L1CacheLineLength()
{
#if __cplusplus
    return __se_hardware_destructive_interference_size;
#else // !__cplusplus
    return __se_hardware_constructive_interference_size;
#endif // end __cplusplus
}

/**
 * @brief Estimates the number of a cachelines the given number of bytes will need to be stored in at compile time.
 *
 * @param bytesLength
 * @return constexpr size_t
 * The estimated number of a cachelines the given number of bytes will need to be stored in.
 */
constexpr size_t GetL1CacheLineCount(size_t bytesLength)
{
    return (bytesLength + L1CacheLineLength() - 1) / L1CacheLineLength();
}

/**
 * @brief Estimates the number of a cachelines the given number of bytes will need to be stored in at compile time.
 *
 * @param bytesLength
 * @return constexpr size_t
 * The estimated number of a cachelines the given number of bytes will need to be stored in.
 */
constexpr size_t GetLengthInBytesToNextCacheLine(size_t bytesLength)
{
    return GetL1CacheLineCount(bytesLength) * L1CacheLineLength();
}

// Type Definitions
typedef struct alignas(L1CacheLineLength()) __se_L1CacheLine
{
    alignas(L1CacheLineLength()) __se_byte m_CacheLine[L1CacheLineLength()];
} __se_L1CacheLine;

typedef __se_L1CacheLine sCacheLine;

#define ENSURE_UNION_IS_L1_CACHE_ALIGNED \
    __se_L1CacheLine m_EnsureCacheAligned[]

#if __cplusplus
namespace Savanna
{
    typedef __se_L1CacheLine L1CacheLine;

    /// <summary>
    /// Estimates the number of a cachelines the type will need to be stored in at runtime.
    /// </summary>
    /// <returns>
    /// The estimated number of a cachelines the type will need to be stored in.
    /// </returns>
    template<typename T>
    SAVANNA_NO_DISCARD size_t GetL1CacheLineCountForType()
    {
        return GetL1CacheLineCount(sizeof(T));
    }

    /**
     * @brief A templated struct that takes a single instance of 'T' and unions it with a
     * compile time generated linear array of 'L1CacheLine's. This is used to enforce
     *
     * @tparam T
     */
    template<typename T>
    struct alignas(L1CacheLineLength()) L1CacheWidthContainer
    {
    public:
        static inline L1CacheWidthContainer<T> MakeUnifiedCacheContainer(T& instance)
        {
            return L1CacheWidthContainer<T>(instance);
        }

        ~L1CacheWidthContainer()
        {
            ~m_Data();
        }

        inline T& Get() { return m_Data; }

        inline const L1CacheLine* GetCacheLines(int32 &outLength)
        {
            outLength = GetL1CacheLineCount(sizeof(T));
            return &m_CacheLines;
        }

    private:
        L1CacheWidthContainer(T&& instance) : m_Data(std::move(instance)) {}

        union
        {
            T m_Data;
            L1CacheLine m_CacheLines[GetL1CacheLineCount(sizeof(T))];
        };
    };

} // namespace Savanna
#endif // end __cplusplus
