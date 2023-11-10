/**
 * @file Optional.h
 * @author David Mohrhardt (https://github.com/DavidMohrhardt/Savanna)
 * @brief TODO @David.Mohrhardt Document
 * @version 0.1
 * @date 2023-10-16
 *
 * @copyright Copyright (c) 2023
 *
 */
#pragma once

#include "Public/ISavannaEngine.hpp"

namespace Savanna
{
    template<typename T>
    class Optional
    {
    private:
        bool m_HasValue = false;
        T m_Value;
    public:
        Optional() = default;
        Optional(const T& value) : m_HasValue(true), m_Value(value) {}
        Optional(T&& value) : m_HasValue(true), m_Value(std::move(value)) {}
        Optional(const Optional& other) : m_HasValue(other.m_HasValue), m_Value(other.m_Value) {}
        Optional(Optional&& other) : m_HasValue(other.m_HasValue), m_Value(std::move(other.m_Value)) {}
        ~Optional() = default;

        Optional& operator=(const T& value)
        {
            m_HasValue = true;
            m_Value = value;
            return *this;
        }

        Optional& operator=(T&& value)
        {
            m_HasValue = true;
            m_Value = std::move(value);
            return *this;
        }

        Optional& operator=(const Optional& other)
        {
            m_HasValue = other.m_HasValue;
            m_Value = other.m_Value;
            return *this;
        }

        Optional& operator=(Optional&& other)
        {
            m_HasValue = other.m_HasValue;
            m_Value = std::move(other.m_Value);
            return *this;
        }

        bool HasValue() const { return m_HasValue; }
        const T& Value() const { return m_Value; }

        operator bool() const { return m_HasValue; }
        operator T&() { return m_Value; }
    };

    template<>
    class Optional<void>
    {
    private:
        bool m_HasValue = false;
    };

    using OptionalInt8 = Optional<int8>;
    using OptionalInt16 = Optional<int16>;
    using OptionalInt32 = Optional<int32>;
    using OptionalInt64 = Optional<int64>;
    using OptionalUint8 = Optional<uint8>;
    using OptionalUint16 = Optional<uint16>;
    using OptionalUint32 = Optional<uint32>;
    using OptionalUint64 = Optional<uint64>;
    using OptionalFloat16 = Optional<float16>;
    using OptionalFloat32 = Optional<float32>;
    using OptionalFloat64 = Optional<float64>;
    using OptionalHalfWord = Optional<half_word>;
    using OptionalUHalfWord = Optional<uhalf_word>;
    using OptionalWord = Optional<word>;
    using OptionalUWord = Optional<uword>;
    using OptionalDoubleWord = Optional<double_word>;
    using OptionalUDoubleWord = Optional<udouble_word>;
    using OptionalQuadWord = Optional<quad_word>;
    using OptionalUQuadWord = Optional<uquad_word>;
    using OptionalIntPtr = Optional<intptr>;
    using OptionalUintPtr = Optional<uintptr>;
} // namespace Savanna
