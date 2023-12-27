#pragma once

#include "SavannaVk2.h"

#include "Types/Containers/Arrays/dynamic_array.h"

namespace savanna::gfx::vk::utils
{
    void PopulateInstanceExtensions(dynamic_array<const char*>& outExtensions, const char* const* ppExtensions, uint32 extensionCount);

    void PopulateDeviceExtensions(
        dynamic_array<const char*>& outExtensions,
        VkPhysicalDevice physicalDevice,
        const char* const* ppExtensions,
        uint32 extensionCount);
}
