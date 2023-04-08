/**
 * @file IHashString.h
 * @author David Mohrhardt (https://github.com/DavidMohrhardt/Savanna)
 * @brief
 * @version 0.1
 * @date 2023-01-30
 *
 * @copyright Copyright (c) 2023
 *
 */
#pragma once

#include "Types/Primitive/PrimitiveTypes.inl"
#include "Types/Hashables/IHashable.h"

namespace Savanna
{
    /**
     * @brief A base class for all HashStrings.
     *
     */
    class IHashString : public IHashable
    {
        using cstr = const char*;
    public:
        static SAVANNA_CONSTEXPR cstr k_None = "<None>";
    };
} // namespace Savanna

namespace std
{
    template<>
    struct hash<Savanna::IHashable>
    {
        size_t operator()(const Savanna::IHashable* pHashable) const
        {
            return pHashable->GetHash();
        }
    };

    template<>
    struct hash<Savanna::IHashString>
    {
        size_t operator()(const Savanna::IHashString* pHashString) const
        {
            return pHashString->GetHash();
        }
    };
}
