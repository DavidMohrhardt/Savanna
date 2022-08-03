#pragma once

#include <Types/Primitive/PrimitiveTypes.h>
#include <Utilities/Macros/CppTypeDefs.h>

typedef enum __se_GfxDeviceCapabilities
{
    Invalid = 0x0,
    Graphics = 0x1,
    Present = 0x1 << 1,
    Compute = 0x1 << 2,
    Transfer = 0x1 << 3,
    Encoding = 0x1 << 4,

    GeometryShading = 0x1 << 5,
    Tessellation = 0x1 << 6,

    RayTracing = 0x1 << 7,

    /**
     * @brief An flag value indicating there are further platform capabilities not specified by
     * this enumeration.
     */
    PlatformSpecificCapabilities = 0x1 << 31,
} __se_GfxDeviceCapabilities;
DECLARE_SAVANNA_EXTENDED_NAMESPACED_CPP_TYPE_DEF(Rendering, __se_GfxDeviceCapabilities, GfxDeviceCapabilities);