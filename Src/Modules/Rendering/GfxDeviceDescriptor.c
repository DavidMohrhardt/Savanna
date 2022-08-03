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