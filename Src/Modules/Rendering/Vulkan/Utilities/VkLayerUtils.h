#pragma once

#include "SavannaVk.h"

namespace Savanna::Gfx::Vk::Utils
{
    const char* k_DefaultValidationLayerName = "VK_LAYER_KHRONOS_validation";

    inline bool IsLayerSupported(const char *layerName, const std::vector<VkLayerProperties>& supportedLayers)
    {
        for (const auto& layer : supportedLayers)
        {
            if (strcmp(layer.layerName, layerName) == 0)
            {
                return true;
            }
        }
        return false;
    }

    inline void ValidateLayers(const char** ppLayers, uint32 layerCount)
    {
        uint32 supportedLayerCount = 0;
        vkEnumerateInstanceLayerProperties(&supportedLayerCount, nullptr);
        std::vector<VkLayerProperties> supportedLayers(supportedLayerCount);
        vkEnumerateInstanceLayerProperties(&supportedLayerCount, supportedLayers.data());

        for (uint32 i = 0; i < layerCount; ++i)
        {
            if (!IsLayerSupported(ppLayers[i], supportedLayers))
            {
                throw Savanna::RuntimeErrorException("Layer requested layer is not supported");
            }
        }
    }
} // namespace Savanna::Gfx::Vk::Utils
