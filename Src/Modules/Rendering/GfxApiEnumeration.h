/**
 * @file GfxApiEnumeration.h
 * @author David Mohrhardt (david.mohrhardt@gmail.com)
 * @brief Provides enumeration values indicating supported/active graphics APIs
 * @version 0.1
 * @date 2022-08-02
 *
 * @copyright Copyright (c) 2022
 *
 */

#pragma once

namespace Savanna
{
    /**
     * @brief TODO @DavidMohrhardt Document
     *
     */
    enum GfxType
    {
        /**
         * @brief TODO @DavidMohrhardt Document
         *
         */
        INVALID = 0x0,

        /**
         * @brief TODO @DavidMohrhardt Document
         *
         */
        VULKAN = 0x1,

        /**
         * @brief TODO @DavidMohrhardt Document
         *
         */
        DX12 = 0x2,

        /**
         * @brief TODO @DavidMohrhardt Document
         *
         */
        DX12ULTIMATE = 0x3,

        /**
         * @brief TODO @DavidMohrhardt Document
         *
         */
        Metal = 0x4,

        /**
         * @brief TODO @DavidMohrhardt Document
         *
         */
        OPENGL = 0x5,
    };
} // namespace Savanna
