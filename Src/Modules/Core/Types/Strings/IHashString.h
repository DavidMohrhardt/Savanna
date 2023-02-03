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

#include "Types/Primitive/PrimitiveTypes.h"
#include "Types/Hashables/IHashable.h"

namespace Savanna
{
    /**
     * @brief A base class for all HashStrings.
     *
     */
    class IHashString : public IHashable
    {
    public:
        static SAVANNA_CONSTEXPR const char* k_None = "<None>";
        static SAVANNA_CONSTEXPR int32 k_NoneHash = 0;
    };
} // namespace Savanna
