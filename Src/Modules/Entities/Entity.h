/**
 * @file Entity.h
 * @author David Mohrhardt (david.mohrhardt@gmail.com)
 * @brief TODO @DavidMohrhardt Document
 * @version 0.1
 * @date 2022-08-02
 *
 * @copyright Copyright (c) 2022
 *
 */

#pragma once

#include <Types/Primitive/PrimitiveTypes.h>

namespace Savanna
{
    struct Entity
    {
        int64 m_EntityHandle;
        intptr m_ComponentReferenceBag;
    };
} // namespace Savanna
