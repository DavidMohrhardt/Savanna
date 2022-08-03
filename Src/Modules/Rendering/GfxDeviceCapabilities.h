/**
 * @file GfxDeviceCapabilities.h
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
#include <Utilities/Macros/CppTypeDefs.h>

typedef enum __se_GfxDeviceCapabilities
{
    /**
     * @brief TODO @DavidMohrhardt Document
     *
     */
    Invalid = 0x0,

    /**
     * @brief TODO @DavidMohrhardt Document
     *
     */
    Graphics = 0x1,

    /**
     * @brief TODO @DavidMohrhardt Document
     *
     */
    Present = 0x1 << 1,

    /**
     * @brief TODO @DavidMohrhardt Document
     *
     */
    Compute = 0x1 << 2,

    /**
     * @brief TODO @DavidMohrhardt Document
     *
     */
    Transfer = 0x1 << 3,

    /**
     * @brief TODO @DavidMohrhardt Document
     *
     */
    Encoding = 0x1 << 4,

    /**
     * @brief TODO @DavidMohrhardt Document
     *
     */
    GeometryShading = 0x1 << 5,

    /**
     * @brief TODO @DavidMohrhardt Document
     *
     */
    Tessellation = 0x1 << 6,

    /**
     * @brief TODO @DavidMohrhardt Document
     *
     */
    RayTracing = 0x1 << 7,

    /**
     * @brief An flag value indicating there are further platform capabilities not specified by
     * this enumeration. Requires querying from TODO
     */
    PlatformSpecificCapabilities = 0x1 << 31,
} __se_GfxDeviceCapabilities;

/**
 * @brief TODO @DavidMohrhardt Document Construct a new declare savanna extended namespaced cpp type def object
 *
 */
DECLARE_SAVANNA_EXTENDED_NAMESPACED_CPP_TYPE_DEF(Rendering, __se_GfxDeviceCapabilities, GfxDeviceCapabilities);