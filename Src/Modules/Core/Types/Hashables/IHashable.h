/**
 * @file IHashable.h
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

namespace Savanna
{
    /**
     * @brief A base class for all Hashables.
     *
     */
    class IHashable
    {
    public:
        SAVANNA_NO_DISCARD virtual const int32 GetHash() const = 0;
    };
} // namespace Savanna
