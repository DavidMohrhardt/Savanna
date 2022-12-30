/**
 * @file GfxDeviceCapabilityFlagsFlags.h
 * @author David Mohrhardt (https://github.com/DavidMohrhardt/Savanna)
 * @brief TODO @DavidMohrhardt Document
 * @version 0.1
 * @date 2022-08-02
 *
 */

#pragma once

#include <Types/Primitive/PrimitiveTypes.h>
#include <Utilities/Macros/CppTypeDefs.h>

typedef enum __se_GfxDeviceCapabilityFlagsFlags
{
    /**
     * @brief Indicates that the device does not support any of the capabilities listed below.
     */
    None = 0x0,

    /**
     * @brief Indicates that the device supports rendering.
     *
     */
    Rendering = 0x1,

    /**
     * @brief Indicates that the device supports compute operations.
     */
    Compute = 0x1 << 1,

    /**
     * @brief Indicates that the device supports displaying to a screen.
     */
    Display = 0x1 << 2,

    /**
     * @brief Indicates that the device supports transfer operations.
     */
    Transfer = 0x1 << 3,

    /**
     * @brief Indicates that the device supports High Dynamic Range. This does not mean the display supports
     * HDR, but that the device can perform HDR operations.
     */
    HDR = 0x1 << 4,

    /**
     * @brief An flag value indicating there are further platform capabilities not specified by
     * this enumeration. Requires querying from TODO
     */
    PlatformSpecificCapabilities = 0x1 << 31,

    /**
     * @brief Indicates that the device supports all of the capabilities listed below.
     */
    All = 0xFFFFFFFF
} __se_GfxDeviceCapabilityFlagsFlags;

/**
 * @brief When included in a C++ file, this macro declares a namespaced type definition for the type
 * __se_GfxDeviceCapabilityFlagsFlags with the fully justified name Savanna::Gfx::GfxDeviceCapabilityFlagsFlags.
 */
DECLARE_SAVANNA_EXTENDED_NAMESPACED_CPP_TYPE_DEF(Rendering, __se_GfxDeviceCapabilityFlagsFlags, GfxDeviceCapabilityFlagsFlags);
