/**
 * @file Entity.h
 * @author David Mohrhardt (https://github.com/DavidMohrhardt/Savanna)
 * @brief TODO @DavidMohrhardt Document
 * @version 0.1
 * @date 2022-08-02
 *
 */

#pragma once

#include <Public/ISavannaEngine.hpp>

namespace Savanna
{
    using EntityHandle = uint64;
    struct Entity
    {
        EntityHandle Id;
    };
} // namespace Savanna
