/**
 * @file GfxDeviceDescriptor.h
 * @author David Mohrhardt (https://github.com/DavidMohrhardt/Savanna)
 * @brief TODO @DavidMohrhardt Document
 * @version 0.1
 * @date 2022-08-02
 *
 */

#pragma once

#include "GfxDeviceCapabilityFlags.h"

#include <Types/Primitive/PrimitiveTypes.h>
#include <Utilities/Macros/CppTypeDefs.h>

/**
 * @brief
 */
typedef struct __se_GfxDeviceDescriptor
{
    __se_int64 m_Uid;
    __se_GfxDeviceCapabilityFlagsFlags m_Capabilities;
    __se_intptr m_PlatformSpecificData;
} __se_GfxDeviceDescriptor;
DECLARE_SAVANNA_EXTENDED_NAMESPACED_CPP_TYPE_DEF(Rendering, __se_GfxDeviceDescriptor, GfxDeviceDescriptor);

#if ENABLE_TODOS
/**
 * @brief Acquires the available GfxDeviceDescriptors from the real renderer.
 * The memory is not guaranteed to be valid past the frame boundary and should
 * be released using ReleaseGfxDeviceDescriptors.
 *
 * @param gfxDeviceDescriptors
 * @param deviceCount
 */
void AcquireGfxDeviceDescriptors(__se_GfxDeviceDescriptor** outGfxDeviceDescriptors, sint32 deviceCount);

/**
 * @brief
 *
 * @param gfxDeviceDescriptors
 */
void ReleaseGfxDeviceDescriptors(__se_GfxDeviceDescriptor** gfxDeviceDescriptors);
#endif
