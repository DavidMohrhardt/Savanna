/**
 * @file GfxDeviceDescriptor.c
 * @author David Mohrhardt (david.mohrhardt@gmail.com)
 * @brief TODO @DavidMohrhardt Document
 * @version 0.1
 * @date 2022-08-02
 *
 * @copyright Copyright (c) 2022
 *
 */

#include "GfxDeviceDescriptor.h"

#if ENABLE_TODOS
void ReleaseGfxDeviceDescriptors(__se_GfxDeviceDescriptor** gfxDeviceDescriptors)
{
    savanna_rendering_ReleaseGfxDeviceDescriptors(*gfxDeviceDescriptors);
}

void AcquireGfxDeviceDescriptors(__se_GfxDeviceDescriptor** outGfxDeviceDescriptors, int32_t& deviceCount)
{
    savanna_rendering_GfxDeviceAcquireDeviceDescs(gfxDeviceDescriptors, deviceCount);
}
#endif