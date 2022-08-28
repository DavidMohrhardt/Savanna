/**
 * @file Entity.h
 * @author David Mohrhardt (https://github.com/DavidMohrhardt/Savanna)
 * @brief TODO @DavidMohrhardt Document
 * @version 0.1
 * @date 2022-08-02
 *
 */

#pragma once

#include <Types/Primitive/PrimitiveTypes.h>

namespace Savanna
{
    typedef uint64 EntityHandle;
    typedef struct Entity
    {
        EntityHandle Id;
    } Entity;
} // namespace Savanna
