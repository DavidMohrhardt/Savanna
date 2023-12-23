/**
 * @file IHashString.h
 * @author David Mohrhardt (https://github.com/DavidMohrhardt/Savanna)
 * @brief TODO @David.Mohrhardt Document
 * @version 0.1
 * @date 2023-01-30
 *
 * @copyright Copyright (c) 2023
 *
 */
#pragma once

#include "Public/ISavannaEngine.h"
#include "Types/Hashables/IHashable.h"

namespace savanna
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
    struct hash<savanna::IHashable>
    {
        size_t operator()(const savanna::IHashable* pHashable) const
        {
            return pHashable->GetHash();
        }
    };

    template<>
    struct hash<savanna::IHashString>
    {
        size_t operator()(const savanna::IHashString* pHashString) const
        {
            return pHashString->GetHash();
        }
    };
}
