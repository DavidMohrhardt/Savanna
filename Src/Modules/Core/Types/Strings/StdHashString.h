/**
 * @file StdHashString.h
 * @author David Mohrhardt (https://github.com/DavidMohrhardt/Savanna)
 * @brief
 * @version 0.1
 * @date 2023-01-30
 *
 * @copyright Copyright (c) 2023
 *
 */
#pragma once

#include "Types/Primitive/PrimitiveTypes.h"
#include "Types/Hashables/IHashable.h"
#include "Types/Strings/IHashString.h"
#include "Types/Strings/CRC32HashString.h"

#include <string>
#include <unordered_map>

namespace Savanna
{
    // Specialize IHashString for std string and char*
    class StdHashString : public IHashString
    {
    private:
        std::string m_String;

    public:
        StdHashString(const std::string& str)
            : m_String(str)
        {}

        StdHashString(const char* str)
            : m_String(str)
        {}

        SAVANNA_NO_DISCARD virtual const int32 GetHash() const SAVANNA_OVERRIDE
        {
            return std::hash<std::string>(m_String);
        }

        SAVANNA_NO_DISCARD const std::string& GetString() const
        {
            return m_String;
        }
    };
} // namespace Savanna
