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
    typedef uint32_t EntityID;
    typedef struct Entity
    {
        EntityID Id;
    } Entity;
} // namespace Savanna
